#include "../lib/uopmsb/uop_msb_2_0_0.h"
#include <chrono>
#include <cstdint>
using namespace uop_msb_200;
using namespace chrono;

//Fun output stuff
extern LCD_16X2_DISPLAY disp;
extern Buzzer buzz;
LatchedLED ledDisp(LatchedLED::STRIP);

//Analogue Inputs
AnalogIn ldr(AN_LDR_PIN);

 int second()
{

    //Test LED Bar Display
    ledDisp.enable(true);

    ledDisp.setGroup(LatchedLED::LEDGROUP::RED);
    for (unsigned int n=0; n<8; n++) {
        ledDisp = 1 << n;
        wait_us(250000);
    }
    ledDisp = 0;

    ledDisp.setGroup(LatchedLED::LEDGROUP::GREEN);
    for (unsigned int n=0; n<8; n++) {
        ledDisp = 1 << n;
        wait_us(250000);
    }
    ledDisp = 0;
    
    ledDisp.setGroup(LatchedLED::LEDGROUP::BLUE);
    for (unsigned int n=0; n<8; n++) {
        ledDisp = 1 << n;
        wait_us(250000);
    }     
    ledDisp = 0;


    while (true) {
        //Read Analog to Digital Converter values (16 bit)
        unsigned short lightVal = ldr.read_u16();


        //Write to terminal
        printf("--------------------------------\n");
        printf("Light Dependant Resistor: %d\n", lightVal);  

        //Wait 0.25 seconds
        wait_us(500000);

    }
}
