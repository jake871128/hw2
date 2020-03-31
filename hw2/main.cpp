#include "mbed.h"

//analog input
Serial pc( USBTX, USBRX );
AnalogIn Ain(A0);
AnalogOut Aout(DAC0_OUT);

//LED
DigitalOut redLED(LED1);
DigitalOut greenLED(LED2);
DigitalIn  Switch(SW3);


//7-segment display
BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
char table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
char ptable[10] = {0xBF, 0x86, 0xDB, 0xAF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF};


int sample = 1024;
int i;
float dot1=0;
float dot2=0;

float ADCdata[1024];
float f=0;
int main(){
    while(1){
        for (i = 0; i < sample; i++){
            Aout = Ain;
            ADCdata[i] = Ain;
            wait(1./sample);
        }

        for (i = 0; i < sample; i++){
            pc.printf("%1.3f\r\n", ADCdata[i]);
            wait(0.001);
        }

        for (i =1 ; i < sample; i++){
            if( (ADCdata[i] > ADCdata[i-1]) && (ADCdata[i] > ADCdata[i+1])){
                if(dot1==0) dot1 = i;
                else if(dot2==0){ 
                    dot2 = i;
                    f =1/((1./sample)*(dot2-dot1));
                    pc.printf("%f\n", f);
                    break;
                }
            }
        }
        while(1){
            //press button redLED
            if( Switch == 0 ){
                redLED = 0;
                greenLED = 1;

                int seg = 0;
                seg = (int)(f+0.5);
                pc.printf("%d\n", seg);
                int num = 0;
                num = seg/100;
                display = table[num];
                wait(0.5);
                num = seg/10%10;
                display = table[num];
                wait(0.5);
                num = seg%10;
                display = ptable[num];
                wait(0.5);
            }
            else {
                redLED = 1;
                greenLED = 0;
                display = 0x00;
            }   

            
            for(float j=0; j<2; j+=0.05 ){
                Aout = 0.5 + 0.5*sin(j*3.14159);
                wait(0.05/(2*f));
            }
            
            
            

        }
        
    }

}