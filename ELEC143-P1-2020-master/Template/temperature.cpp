//#include "mbed.h"
#include "../lib/uopmsb/uop_msb_2_0_0.h"
#include "BMP280_SPI.h"
#include <cstring>


using namespace uop_msb_200;

// Using the External Buzzer
extern Buzzer buzz;


// Initialising the DigitalOut only for this file 
static DigitalOut traf1RedLED(TRAF_RED1_PIN,1);
static DigitalOut traf1YelLED(TRAF_YEL1_PIN);
static DigitalOut traf1GrnLED(TRAF_GRN1_PIN);

//Environmental sensor
extern EnvironmentalSensor sensor;


// This file only function which works out the sum of the array
static float Sum_of_Temperature(float *array){
    float Sum_of_Temperature_Array = 0;
    int Sum_Index;

    for (Sum_Index = 0; Sum_Index < 10; Sum_Index++){
        Sum_of_Temperature_Array += array[Sum_Index];
    }


    return Sum_of_Temperature_Array;
}

// This File only Function which will detect when the category changes and will report it;
static void Is_Category_Change(int Cat){
    static int old_value;
    if (Cat > 0){
        if (Cat != old_value){
            printf("A Category Change is occurring \n");
        }
    }
    old_value = Cat;

}

// This file only Function which will calculate the Average Temperature Value
static float Average_Temperature(){
    
    float Av_Temperature; // Average Temperature
    float temperature_values[10]; // Takes in the rolling average
    int Temp_Index;
    float Sum_Temperature; // Takes the result of the Sum_of_Temperature Function
    float temperature;
    temperature = sensor.getTemperature();

    memset(temperature_values, 0, size_t(10)); // Sets all elements of the temperature_values array to 0

    for (Temp_Index = 0; Temp_Index < 10; Temp_Index++){
        temperature_values[Temp_Index] = temperature;
    }

    Sum_Temperature = Sum_of_Temperature(temperature_values);

    Av_Temperature = Sum_Temperature / 10;

    return Av_Temperature;
}


/*
 Main file only function in this file which changes which lights are turned on at different temperatures and
 will turn on the buzzer when it goes below freezing, Hysteresis has also been used as shown in the
 comment section in the main function below
*/
static void temperature_function(float Average_Temperature_Value){
    
    /*
        Ranges:      | With Hysteresis:
             x < 0   |      x < -1
         0 < x < 20  |  1 < x < 19
        20 < x < 40  | 21 < x < 39
        40 < x       | 41 < x
    */
    static short Category_Value; // Takes the value of the Category is used to detect when it is changed

    if (Average_Temperature_Value < -1 ){
        Category_Value = 1;
        buzz.playTone("A", Buzzer::MIDDLE_OCTAVE);
        traf1RedLED = 1;
        traf1YelLED = 0;
        traf1GrnLED = 0;
        wait_us(1000000);
        buzz.rest();
        traf1RedLED = 0;
        wait_us(1000000);
    }
    else if ((1 < Average_Temperature_Value) && (Average_Temperature_Value < 19)){
        Category_Value = 2;
        traf1RedLED = 1;
        traf1YelLED = 0;
        traf1GrnLED = 0;
    }
    else if ((21 < Average_Temperature_Value) && (Average_Temperature_Value < 39)){
        Category_Value = 3;
        traf1RedLED = 0;
        traf1YelLED = 1;
        traf1GrnLED = 0;
    }
    else if (41 < Average_Temperature_Value){
        Category_Value = 4;
        traf1RedLED = 0;
        traf1YelLED = 0;
        traf1GrnLED = 1;
    }
    
    Is_Category_Change(Category_Value);

    printf("Category: %d, Average Temp: %.2f | ", Category_Value, Average_Temperature_Value); // Displays the Category Value and Average Temperature
}



// This function is the Main function which interacts with the main.cpp file, it will only run the temperature function when there is a change
// detected by the sensor and then display it, if no change is detected then it will put the cpu into a low power mode for 1 second, using the
// wait function, this also stops the Serial bus from being overloaded with information which is unnessesary.
void run_temperature_function(){
    static float Old_Temperature;
    float Av_Temperature;

    Av_Temperature = Average_Temperature();

    if (Av_Temperature != Old_Temperature){
        temperature_function(Av_Temperature);
    }
    else{
        wait_us(1000000); // puts the cpu on hold for 1 second
    }

    Old_Temperature = Av_Temperature;

}
