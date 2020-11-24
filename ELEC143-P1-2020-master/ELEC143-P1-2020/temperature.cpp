//#include "mbed.h"
#include "../lib/uopmsb/uop_msb_2_0_0.h"
#include "BMP280_SPI.h"

using namespace uop_msb_200;

extern Buzzer buzz;

static DigitalOut traf1RedLED(TRAF_RED1_PIN,1);
static DigitalOut traf1YelLED(TRAF_YEL1_PIN);
static DigitalOut traf1GrnLED(TRAF_GRN1_PIN);

static float Sum_of_Temperature(float *array){
    float Sum_of_Temperature_Array;
    int Sum_Index;

    for (Sum_Index = 0; Sum_Index < 10; Sum_Index++){
        Sum_of_Temperature_Array += array[Sum_Index];
    }


    return Sum_of_Temperature_Array;
}

void temperature_function(float temperature_sensor){
    
    /*
        Ranges:      | With Hysterysis:
        x < 0        | x < 2
        0 < x < 20   | -2 < x < 22
        20 < x < 40  | 18 < x < 42
        40 < x       | 38< x
    */
    float temperature_values[10]; // Takes in the rolling average
    float Av_Temperature; // Average Temperature
    int Temp_Index;
    float Sum_Temperature;

    for (Temp_Index = 0; Temp_Index < 10; Temp_Index++){
        temperature_values[Temp_Index] = temperature_sensor;
    }

    Sum_Temperature = Sum_of_Temperature(temperature_values);

    Av_Temperature = Sum_Temperature / 10;

    if (Av_Temperature < 2.0){

        buzz.playTone("A", Buzzer::MIDDLE_OCTAVE);
        traf1RedLED = 1;
        traf1YelLED = 0;
        traf1GrnLED = 0;
        wait_us(1000000);
        buzz.rest();
        traf1RedLED = 0;
        wait_us(1000000);
    }
    else if (-2.0 < Av_Temperature < 22.0){
        traf1RedLED = 1;
        traf1YelLED = 0;
        traf1GrnLED = 0;
    }
    else if (18.0 < Av_Temperature < 42.0){
        traf1RedLED = 0;
        traf1YelLED = 1;
        traf1GrnLED = 0;
    }
    else if (38.0 < Av_Temperature){
        traf1RedLED = 0;
        traf1YelLED = 0;
        traf1GrnLED = 1;
    }

}