#include "../lib/uopmsb/uop_msb_2_0_0.h"
#include <chrono>
#include <cstdint>
using namespace uop_msb_200;
using namespace chrono;

AnalogIn ldr(AN_LDR_PIN);
float LDRArray [10];
int i=0;
float meanLDR= 0;

LCD_16X2_DISPLAY dis;
PwmOut dispBackLight(LCD_BKL_PIN);

// Finding the average of the inputs
void ReadAvgLDR()
{
    float sum=0;
    if(i<=10){
        //reads theLDR Values
        LDRArray[i]=ldr.read();
        i++;
    }
    else {
        i=0;
    }
    
    for (int j=0; j<10; j++ )
    {
        //averages the 10 LDR values
        sum = sum + LDRArray [j];
    }
     meanLDR =sum/10;
}

void LCDbrightctrll()
{   
    //display's Backlight changes
    dispBackLight.write(1.0 - meanLDR);
    dispBackLight.period(0.020f);
}

/*
        Ranges:         | With Hysteresis:
        x > 0           |  x > 0.0
        0 < x > 0.25    |  x > 0.25
        0.26 < x > 0.50 |  x > 0.50
        0.75 < x        |  x > 0.75
    */



void LDRfunction()
{
    //displays the LDR Value to the serial port
    ReadAvgLDR();
    
    printf("\n*****************************************");
    printf("\nLight Feedback: ");
    
    if (meanLDR>0.75){
        printf("DARK %f\n",meanLDR);
    } 
    else if(meanLDR>0.50){
        printf("LOW %f\n",meanLDR);
    }
    else if(meanLDR>0.25){
        printf("DAY %f\n",meanLDR);
    } 
    else if(meanLDR>0.0){
        printf("INTENSE %f\n",meanLDR);
    }
    printf("*****************************************\n");
    LCDbrightctrll();
    
}
