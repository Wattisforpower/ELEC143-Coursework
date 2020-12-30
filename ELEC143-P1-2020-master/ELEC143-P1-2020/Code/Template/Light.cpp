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


void ReadAvgLDR()
{
    float sum=0;
    if(i<=10){
        LDRArray[i]=ldr.read();
        i++;
    }
    else {
        i=0;
    }

    for (int j=0; j<10; j++ )
    {
        sum = sum + LDRArray [j];
    }
     meanLDR =sum/10;
}

void LCDbrightctrll()
{   
    //Update display
    dis.locate(1, 0);
    dis.printf("Turn the POT");
    dispBackLight.write(1.0 - meanLDR);
    dispBackLight.period(0.020f);
}




void LDRfunction()
{
    ReadAvgLDR();
    // printf("LDR Average  %f\n",meanLDR);
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
