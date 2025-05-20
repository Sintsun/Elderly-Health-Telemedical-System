//OLED
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C
#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


//Temp
typedef struct
{
    float e;
    float to;
    float ta;
    float bo;
} gyir;

byte add=0xa4;
byte len=0,start_reg=0;
unsigned char Re_buf[30],counter=0;
unsigned char sign=0;
gyir my_ir;


void setup() {
  // put your setup code here, to run once:
  

 OLEDSetup();
 TempSetup();
}

void loop() {
  
  temp();
 
  
  
  }

void TempSetup(){

    Serial.begin(115200);
   Serial2.begin(9600);
   
}
void temp(){
unsigned char i=0,sum=0;
  while (Serial2.available()) {   
    Re_buf[counter]=(unsigned char)Serial2.read();
    switch(counter)
    {
      case 0:
      if(Re_buf[0]!=add)
      return;
      break;
      case 1:
    
      if(Re_buf[1]!=0x03)
        { 
          counter=0;
          return;
      }
      break;
      case 2:
        if(Re_buf[2]<16)
         start_reg=Re_buf[2];
         else
            { 
          counter=0;
          return;
         }
      break;
      case 3:
         if((start_reg+Re_buf[3])<16)
            len=Re_buf[3];
            else
            { 
              counter=0;
             return;
         }
            
      break;
      default:
         if(len+5==counter)
         {  
            sign=1;
         }
      break;
    }
    
  if(sign)
  {   
     sign=0;
     for(i=0;i<counter-1;i++)
      sum+=Re_buf[i]; 
       counter=0;
     if(sum==Re_buf[i] )        //检查帧头，帧尾
     {           
        if(start_reg==0x07)
        {
         my_ir.to=(Re_buf[5]<<8)|Re_buf[6];
   my_ir.ta=(Re_buf[7]<<8)|Re_buf[8];
   my_ir.bo=(Re_buf[9]<<8)|Re_buf[10];
   my_ir.e=Re_buf[4];
        }
     
      
     Serial.print("E:");
      Serial.print((float)my_ir.e/100);
  Serial.print(",to:");
    Serial.print((float)my_ir.to/100);
 Serial.print(",ta:");
       Serial.print((float)my_ir.ta/100);
   Serial.print(" ,bo:");  
        Serial.println((float)my_ir.bo/100);
   Display_Temp();
  
     }
//     else
//        {
//          Serial.print(" sum ");
//          Serial.println(sum);
//      } 
   }
   else
   counter++; 
//   Serial.print("cont:");
//   Serial.println(counter);
  }

 

}
void Display_Temp(){
          display.setTextSize(2);
  display.writeFillRect(0, 50, 128, 16, BLACK);
  display.setTextColor(WHITE);
   display.setCursor(0,50);
display.print("TEMP:");
display.print((float)my_ir.to/100 );
  display.display();

}

void OLEDSetup(){

display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

display.clearDisplay();

}