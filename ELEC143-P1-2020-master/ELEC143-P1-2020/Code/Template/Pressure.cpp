#include "../lib/uopmsb/uop_msb_2_0_0.h"
#include "BMP280_SPI.h"

using namespace uop_msb_200;

LCD_16X2_DISPLAY disp;

extern EnvironmentalSensor sensor;

float pressure;
int front = 0;

void pressurefallcheck(float pressure){
    int dataset = 50;
    float pressurevalues[dataset];

    //creating a list of pressurevalues to 'graph' and find gradient

    for (int n = 0; n < dataset; n++){
        pressure  = sensor.getPressure();
        pressurevalues[n] = pressure;
    }

    int i = 0;
    long sumx = 0;
    long sumy = 0 ;
    long sumxy = 0;
    long sumxx = 0;

    //setting up variables for a regression model

    for(i = 0; i < dataset; i++){
        sumx = sumx + int(pressurevalues[i]);
        sumy = sumy + (i+1);
        sumxy = sumxy + pressurevalues[i] * i;
        sumxx = sumxx + (pressurevalues[i] * pressurevalues[i]);
    }

    //calculating a regression gradient

    double gradient  = (dataset * sumxy - (sumx)*(sumy)) / (dataset * sumxx - (sumx) * (sumx));

    //if the gradient is negative pressure is falling, outputs this to the lcd

    if (gradient < 0){
        disp.printf("Rapid Pressure Fall Detected \n");
    }
}


void pressurefunction(){
    disp.cls();     //clears screen
    pressure  = sensor.getPressure();
    disp.locate(0,0);
    disp.printf("%.1fmBar\n", pressure); //displays presure value on lcd
    pressurefallcheck(pressure);
    wait_us(500000);
} 