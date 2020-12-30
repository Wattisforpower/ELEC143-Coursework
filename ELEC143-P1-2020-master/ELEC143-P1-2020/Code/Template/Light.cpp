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
    dispBackLight.period(0.100f);   // 1ms
    dispBackLight.write(1-meanLDR);       // 100% duty  
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
    else if(meanLDR>0.65){
        printf("LOW %f\n",meanLDR);
    }
    else if(meanLDR>0.50){
        printf("DAY %f\n",meanLDR);
    } 
    else if(meanLDR>0.25){
        printf("INTENSE %f\n",meanLDR);
    }
    printf("*****************************************\n");
    LCDbrightctrll();
    
    
}




// static float Sum_of_LDR(float *array){
//     float Sum_of_LDR_Array = 0;
//     int Sum_Index;

//     for (Sum_Index = 0; Sum_Index < 10; Sum_Index++){
//         Sum_of_LDR_Array += array[Sum_Index];
//     }

//     return Sum_of_LDR_Array;
// }



// //Analogue Inputs

// static float Average_LDR2(){
    
//  float Average_LDR; // Average LDR
//  float LDR[10]; // Takes in the rolling average
//  int LDR_Index;
//  float Sum_LDR; // Takes the result of the Sum_of_Temperature Function
//  float LDR2;
    

//  memset(LDR, 0, size_t(10)); // Sets all elements of the temperature_values array to 0

//     for (LDR_Index = 0; LDR_Index < 10; LDR_Index++){
//         LDR2 = ldr.read();
//         LDR[LDR_Index] = LDR2;
//     }

//     Sum_LDR = Sum_of_LDR(LDR);

//     Average_LDR = Sum_LDR / 10;

//     return Average_LDR;

// }
// void second(float lightVal){
//      /*
//         Ranges:         | With Hysteresis:
//         x < 0.25        |  x < 0.24
//         0.25 < x < 0.50 |  0.26 < x < 0.49
//         0.50 < x < 0.75 |  0.51 < x < 0.74
//         0.75 < x < 1    |  0.76 < x < 1
//     */
    




//     //Test LED Bar Display

//     //Write to terminal
//     printf("--------------------------------\n");
//     printf("Light Dependant Resistor: %f\n", lightVal);  
//     //Wait 0.25 seconds

// }
//  void LDRfunction1(){
//     static float Old_LDR;
//     float Average_LDR;

//     Average_LDR = Average_LDR2();

//     if (Average_LDR != Old_LDR){
//         second(Average_LDR);
//     }
//     else{
//         wait_us(1000000); // puts the cpu on hold for 1 second
//     }

//     Old_LDR = Average_LDR;


//  }