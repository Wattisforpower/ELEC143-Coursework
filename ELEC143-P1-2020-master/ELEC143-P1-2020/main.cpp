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

//Light Levels
AnalogIn ldr(AN_LDR_PIN);

//Environmental sensor
EnvironmentalSensor sensor;

//LCD Backlight - consider PwmOut for this :)
DigitalOut backLight(LCD_BKL_PIN);

// External Functions
extern void temperature_function(float temperature_sensor);

int main()
{
    //LCD Backlight ON
    backLight = 1;

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
        disp.printf("LDR: %0.3f", ldr.read());
        float temperature, pressure;
        temperature = sensor.getTemperature();
        pressure = sensor.getPressure();

        temperature_function(temperature);

        printf("%.1fC %.1fmBar\n",temperature,pressure);     
    }
}

