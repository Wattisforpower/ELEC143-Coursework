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

    for (int n = 0; n < dataset; n++){
        pressure  = sensor.getPressure();
        pressurevalues[n] = pressure;
        //printf("Pressurevalue = %f", pressurevalues[n]);
    }

    int i = 0;
    long sumx = 0;
    long sumy = 0 ;
    long sumxy = 0;
    long sumxx = 0;
    /*
    for(i = 0; i < dataset; i++){
        int index = (front + i) % sizeof(pressurevalues);
        sumx = sumx + pressurevalues[index];
        sumy = sumy + index;
        sumxy = sumxy + pressurevalues[index] * index;
        sumxx = sumxx + (pressurevalues[index] * pressurevalues[index]);
    }*/

    for(i = 0; i < dataset; i++){
        //int index = (front + i) % sizeof(pressurevalues);
        sumx = sumx + int(pressurevalues[i]);
        //printf("sumx = %ld\n", sumx);
        //printf("pressurevalues = %f\n", pressurevalues[i]);
        sumy = sumy + (i+1);
        sumxy = sumxy + pressurevalues[i] * i;
        sumxx = sumxx + (pressurevalues[i] * pressurevalues[i]);
    }

    
    double gradient  = (dataset * sumxy - (sumx)*(sumy)) / (dataset * sumxx - (sumx) * (sumx));


    if (gradient < 0){
        disp.locate(1, 0);
        disp.printf("Rapid Pressure Fall Detected \n");
    }
    
    printf("sumx = %ld\n", sumx);
    printf("sumy = %ld\n", sumy);
    printf("gradient = %f\n", gradient);
}


void pressurefunction(){
    //float pressure;
    //int front = 0;
    disp.cls();
    disp.locate(0,0);
    disp.printf("%.1fmBar\n", pressure);
    wait_us(500000);
    pressurefallcheck(pressure);

    

} 