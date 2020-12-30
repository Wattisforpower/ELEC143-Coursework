//#include "mbed.h"
#include "../lib/uopmsb/uop_msb_2_0_0.h"
#include "BMP280_SPI.h"

using namespace uop_msb_200;

/*
    University of Plymouth Group Coursework
    Group:
    - Ethan Barrett
    - Aamer Dib
    - Guy Ludford
*/


//On board LEDs
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);

//On board switch
DigitalIn BlueButton(USER_BUTTON);

//LCD Display
LCD_16X2_DISPLAY disp;

//Buzzer
Buzzer buzz;

//Traffic Lights
DigitalInOut traf2RedLED(TRAF_RED2_PIN, PIN_OUTPUT, OpenDrainNoPull, 0);
DigitalInOut traf2YelLED(TRAF_YEL2_PIN, PIN_OUTPUT, OpenDrainNoPull, 1);
DigitalInOut traf2GrnLED(TRAF_GRN2_PIN, PIN_OUTPUT, OpenDrainNoPull, 1);




//Environmental sensor
EnvironmentalSensor sensor;



// External Functions
extern void run_temperature_function();
extern void LDRfunction();
int main()
{   
    //LCD Backlight ON
    

    while (true) {
        led1 = 1;
        led2 = 1;
        led3 = 1;
        wait_us(500000);
        led1 = 0;
        led2 = 0;
        led3 = 0;  
        wait_us(500000);    
        disp.cls();
        float pressure;
        pressure = sensor.getPressure();

        run_temperature_function();
        LDRfunction();

        printf("%.1fmBar\n",pressure);     
    }
}

