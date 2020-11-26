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
            printf("A Category Change is occuring \n");
        }
    }
    old_value = Cat;

}


/*
 Main function in this file which changes which lights are turned on at different temperatures and
 will turn on the buzzer when it goes below freezing, Hysteresis has also been used as shown in the
 comment section int the main function below
*/
void temperature_function(float temperature_sensor){
    
    /*
        Ranges:      | With Hysteresis:
             x < 0   |      x < -1
         0 < x < 20  |  1 < x < 19
        20 < x < 40  | 21 < x < 39
        40 < x       | 41 < x
    */
    float temperature_values[10]; // Takes in the rolling average
    float Av_Temperature; // Average Temperature
    int Temp_Index;
    float Sum_Temperature;
    short Category_Value;

    memset(temperature_values, 0, size_t(10)); // Sets all elements of the temperature_values array to 0

    for (Temp_Index = 0; Temp_Index < 10; Temp_Index++){
        temperature_values[Temp_Index] = temperature_sensor;
    }

    Sum_Temperature = Sum_of_Temperature(temperature_values);

    Av_Temperature = Sum_Temperature / 10;

    if (Av_Temperature < -1 ){
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
    else if ((1 < Av_Temperature) && (Av_Temperature < 19)){
        Category_Value = 2;
        traf1RedLED = 1;
        traf1YelLED = 0;
        traf1GrnLED = 0;
    }
    else if ((21 < Av_Temperature) && (Av_Temperature < 39)){
        Category_Value = 3;
        traf1RedLED = 0;
        traf1YelLED = 1;
        traf1GrnLED = 0;
    }
    else if (41 < Av_Temperature){
        Category_Value = 4;
        traf1RedLED = 0;
        traf1YelLED = 0;
        traf1GrnLED = 1;
    }
    
    Is_Category_Change(Category_Value);

    printf("Category: %d, Average Temp: %.1f | ", Category_Value, Av_Temperature);
}