#include "../lib/uopmsb/uop_msb_2_0_0.h"
#include "BMP280_SPI.h"

using namespace uop_msb_200;

LCD_16X2_DISPLAY disp;

extern EnvironmentalSensor sensor;

float pressure;
int front = 0;

float pressurefallcheck(){
    int dataset = 10;
    float pressurevalues[dataset];
    float total_pressure;
    float average_pressure;

    //creating a list of pressurevalues to 'graph' and find gradient

    for (int n = 0; n < dataset; n++){
        pressure  = sensor.getPressure();
        pressurevalues[n] = pressure;
    }

    for (int m = 0; m < dataset; m++){
        total_pressure += pressurevalues[m];
    }

    average_pressure = total_pressure / dataset;

    long sumx = 0;
    long sumy = 0 ;
    long sumxy = 0;
    long sumxx = 0;

    //setting up variables for a regression model
    /*
    for(int i = 0; i < dataset; i++){
        sumx = sumx + (pressurevalues[i]);
        sumy = sumy + (i+1);
        sumxy = sumxy + (pressurevalues[i] * (i));
        sumxx = sumxx + (pressurevalues[i] * pressurevalues[i]);
    }
    */

    // Setting Up XX and YY.
    float xx[dataset], yy[dataset];

    for (int i = 0; i < dataset; i++){
        xx[i] = (pressurevalues[i] * pressurevalues[i]);
        yy[i] = i * i;
    }

    for (int i = 0; i< dataset; i++){
        sumx += pressurevalues[i];
        sumy += i;
        sumxx += xx[i];
        sumxy += pressurevalues[i] * i;
    }

    //calculating a regression gradient

    float nr, dr;

    nr = (dataset * sumxy) - (sumx * sumy);
    dr = (dataset * sumxx) - (sumx * sumx);

    double gradient = nr / dr;

    /*
    double gradient  = (((dataset) * (sumxy)) - ((sumx)*(sumy))) / (((dataset) * (sumxx)) - ((sumx) * (sumx)));
    */

    //if the gradient is negative pressure is falling, outputs this to the lcd

    if (gradient < 0){
        disp.printf("Rapid Pressure Fall Detected \n");
    }
    printf("%f \n", gradient);

    return average_pressure;
}


void pressurefunction(){
    float pressure;
    disp.cls();     //clears screen
    pressure = pressurefallcheck();
    disp.locate(0,0);
    disp.printf("%.1fmBar\n", pressure); //displays presure value on lcd
    wait_us(500000);
} 