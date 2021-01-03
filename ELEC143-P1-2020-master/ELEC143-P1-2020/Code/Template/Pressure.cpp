#include "../lib/uopmsb/uop_msb_2_0_0.h"
#include "BMP280_SPI.h"

using namespace uop_msb_200;

LCD_16X2_DISPLAY lcd;

extern EnvironmentalSensor sensor;

float pressure;
int front = 0;

void pressurefallcheck(float pressure){
    int dataset = 50;
    float pressurevalues [dataset];
    pressurevalues[front] = pressure;
    front = (front + 1) % sizeof(pressurevalues);

    int i = 0;
    int sumx = 0;
    int sumy = 0 ;
    int sumxy = 0;
    int sumxx = 0;
    for(i = 0; i < dataset; i++){
        int index = (front + i) % sizeof(pressurevalues);
        sumx = sumx + pressurevalues[index];
        sumy = sumy + index;
        sumxy = sumxy + pressurevalues[index] * index;
        sumxx = sumxx + pow(pressurevalues[index], 2);
    }

    float average  = (dataset * sumxy - (sumx)*(sumy)) / (dataset * sumxx - (sumx) * (sumx));
    if (average < -1){
        lcd.locate(1, 0);
        lcd.printf("Rapid Pressure Fall Detected");
    }

    //printf("average = %.1fmBar\n", average);
}


void pressurefunction(){
    float pressure;
    int front = 0;
    
    while (true){
        lcd.cls();
        pressure  = sensor.getPressure();
        lcd.locate(0,0);
        lcd.printf("pressure = %.1fmBar\n", pressure);
        pressurefallcheck(pressure);

    }

} 