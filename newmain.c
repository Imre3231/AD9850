 
#include"p18f4520.h"
#include <xc.h>
#include "DELAY.h"
#include "CONFIG.h"
#include "lcd.h"
#include <stdlib.h>


const int SINE = 0x2000;                    // Define AD9833's waveform register value.
const int SQUARE = 0x2028;                  // When we update the frequency, we need to
const int TRIANGLE = 0x2002;                // define the waveform when we end writing.    

#define Button1 PORTAbits.RA2
#define Button2 PORTAbits.RA3
#define RESET PORTDbits.RD3
#define FSYNC PORTDbits.RD2
#define SCLK PORTCbits.RC3
#define SDO PORTCbits.RC5


 long long data2;
 long long data;
unsigned int num = 174;
 
unsigned char txt7[16]={" ^       "}; 
unsigned char txt6[16]={"   ^     "};
unsigned char txt5[16]={"    ^    "};
unsigned char txt4[16]={"     ^   "};
unsigned char txt3[16]={"       ^ "}; 
unsigned char txt2[16]={"        ^ "};
unsigned char txt1[16]={"         ^"};

unsigned char buff[10] ;
unsigned char buff2[10] ; 
 signed int value = 0;
 signed int value2 = 0;
unsigned char x;


unsigned char  buttonState1;
unsigned char  previousState1;
unsigned char  buttonState2;
unsigned char  previousState2;


 char new_encoder;
char last_encoder;



void ENCODER(void)
{

    new_encoder = (PORTA & 0x03);      // keep only 2 bits
    if(new_encoder !=last_encoder)      // if encoder has changed
   {
         if((last_encoder == 1)&&(new_encoder==3)){         
 
             
            if(value==0){data++;} 
            if(value==1){data=data+10;} 
            if(value==2){data=data+100;}  
            if(value==3){data=data+1000;} 
            if(value==4){data=data+10000;} 
            if(value==5){data=data+100000;}
            if(value==6){data=data+1000000;}
            
            if(data>30000000){data=30000000;}
            
            
            
 } 
      if((last_encoder == 2)&&(new_encoder==3)){
         
            if(value==0){data--;} 
            if(value==1){data=data-10;} 
            if(value==2){data=data-100;}  
            if(value==3){data=data-1000;} 
            if(value==4){data=data-10000;} 
            if(value==5){data=data-100000;}
            if(value==6){data=data-1000000;}
           
            if(data<1){data=0;}
         
      } 

      last_encoder = new_encoder;   // save for next time
   }

}




void DDS (unsigned long data)

{
    unsigned char rotate;
    unsigned long word;
    unsigned char i;
 
    
 
    rotate=0;
    for(i=0;i<32;i++)
    {
        word=data;
        word=(word>>(rotate));
        
        SDO=(word&0x01);
        SCLK=0;
        SCLK=1;
        rotate++;
    
    }
     
    

  

}




void DDSad9833 (unsigned long data)

{

unsigned long FreqWord =  data/7.45   ;

unsigned  char MSB;
unsigned  char LSB;
unsigned  char MSB2;
unsigned  char LSB2;

MSB2 = ((FreqWord >> 24 ) )  ;    //Only lower 14 bits are used for data
MSB = ((FreqWord >> 16 ) )  ;    //Only lower 14 bits are used for data
LSB2 = ((FreqWord >> 8 ) )  ;    //Only lower 14 bits are used for data
LSB = ((FreqWord >> 0 ) )  ;    //Only lower 14 bits are used for data 
 

    
 
   DDS(FreqWord);               // Phase register
   FSYNC=1;
  
    FSYNC=0; 
     
    FSYNC=1;
      
      
}






int main()
{
  ADCON1 = 0x0F;  // Configure AN pins as digital    
  TRISB = 0; //0 as Output PIN
  TRISC = 0;
  TRISD = 0x00;
  TRISA  = 0xff; 
    
  LCD_init();
 

  
  delay_ms(1000);
  
 
    num++;
    intToStr(num,buff,1,6);        //itoa(buf2, num, 10);
    LCD_cmd((LCD_LINE2+(0)));     //SET cursor position
    LCD_text((buff));
      CLS();
    LCD_cmd((LCD_LINE1+(1))); LCD_text("DDS  VFO");    
        
    
     RESET=1;
  
     RESET=0; 
     
    
    
    
   while(1){
        
       value2=value;
       data2=data;
       ENCODER();
       
       
            buttonState1=Button1; 
            if(buttonState1 != previousState1){ if (buttonState1==1){ 
            value++;  if(value>6){value=6;};
            
            }
            previousState1 = buttonState1;
            
            }
            
            
            buttonState2=Button2; 
            if(buttonState2 != previousState2){ if (buttonState2==1){ 
            value--;  if(value<1){value=0;};
            
            }
            previousState2 = buttonState2;
            
            }
 
 
              if((data!=data2)|(value2!=value))
        
              {
               
   tostring3(data, buff);
   
           
    
         LCD_cmd((LCD_LINE1+(0))); LCD_text(buff);  
         
         
         LCD_cmd((LCD_LINE1+(10))); LCD_text("  Mhz");        
  
    
          
         
         
       if(value==0){ LCD_cmd((LCD_LINE2+(0))); LCD_text(txt1);}    
       if(value==1){ LCD_cmd((LCD_LINE2+(0))); LCD_text(txt2);}  
       if(value==2){ LCD_cmd((LCD_LINE2+(0))); LCD_text(txt3);}  
       if(value==3){ LCD_cmd((LCD_LINE2+(0))); LCD_text(txt4);}  
       if(value==4){ LCD_cmd((LCD_LINE2+(0))); LCD_text(txt5);}  
       if(value==5){ LCD_cmd((LCD_LINE2+(0))); LCD_text(txt6);}   
       if(value==6){ LCD_cmd((LCD_LINE2+(0))); LCD_text(txt7);}  
       
         
    
         
         DDSad9833 (data);
         
              }
       
    
    }
    
    
   
 }
 