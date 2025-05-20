//communication
#include <SoftwareSerial.h>
// Define a Software Serial object and the used pins 
SoftwareSerial softSerial(10, 11);
char Type;
int  Data;
float Temp;
int BPM;
int SPO2;
int BloodPressure;
int ID;
String UID;
int count=0;
//e-Paper
// this part of code has been explained in Sec.D.2

//if you have another microcontroller or another e-ink display module you have to change the following line
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h> // including both doesn't use more code or ram
#include <U8g2_for_Adafruit_GFX.h>
#define ENABLE_GxEPD2_GFX 0
#define MAX_DISPLAY_BUFFER_SIZE 800
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? EPD::HEIGHT : MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8))
GxEPD2_BW<GxEPD2_154_D67, MAX_HEIGHT(GxEPD2_154_D67)> display(GxEPD2_154_D67(/*CS=10*/ A0, /*DC=*/ 7, /*RST=*/ 8, /*BUSY=*/ 9)); // GDEH0154D67 200x200
U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;


void setup() {
  // put your setup code here, to run once:

 Communication_Setup  ();
ePaperSetup();
Serial.begin(9600); 
}

void loop() {
  // put your main code here, to run repeatedly:
  Communication ();
 
 epaper();
}


void Communication_Setup  (){

softSerial.begin(19200); 



}
void Communication ()  {

  if (softSerial.available())  { 
     char Type = softSerial.read();
   Serial.println(Type);
     if (Type=='T'){

Temp=softSerial.parseFloat();
Serial.println("Temp:");
Serial.println(Temp);

  }else if (Type=='B'){

BPM=softSerial.parseInt();
  Serial.println("BMP:");
  Serial.println(BPM);
  }else if (Type=='O'){

SPO2=softSerial.parseFloat();
 Serial.println("SPO2");
 Serial.println(SPO2);
  }else if (Type=='P'){

BloodPressure=softSerial.parseInt();
 Serial.println("Blood Pressure");
 Serial.println(BloodPressure);
  }else if (Type=='I'){

ID=softSerial.parseInt();
 Serial.println("ID");
 Serial.println(ID);
if(ID==1){
UID="04125042";


}else if (ID==2){
UID="CCF51E30";
  }




}}}
void ePaperSetup(){

display.init();
  display.setTextColor(GxEPD_BLACK);
  display.firstPage();
    display.setRotation(3);

  u8g2Fonts.begin(display); // connect u8g2 procedures to Adafruit GFX
  delay(1000);

  uint16_t bg = GxEPD_WHITE;
  uint16_t fg = GxEPD_BLACK;
  u8g2Fonts.setForegroundColor(fg);         // apply Adafruit GFX color
  u8g2Fonts.setBackgroundColor(bg);

  do{
    display.fillScreen(GxEPD_WHITE);

    u8g2Fonts.setFont(u8g2_font_fub20_tr);   //font is set
   u8g2Fonts.setCursor(20,30 );             //cursor(x,y)
    u8g2Fonts.print("Last Record ");

    u8g2Fonts.setCursor(10,60 );             //cursor(x,y)

    u8g2Fonts.print("UID: ");   //print the text
    u8g2Fonts.setCursor(10, 90);       //cursor(x,y)
       
    u8g2Fonts.print("BMP: "); 
 u8g2Fonts.setCursor(10, 120);             //cursor(x,y)
    u8g2Fonts.print("Temp: "); 

 u8g2Fonts.setCursor(10, 150);             //cursor(x,y)
    u8g2Fonts.print("spO2: "); 
    u8g2Fonts.setCursor(10, 180);             //cursor(x,y)
    u8g2Fonts.print("BP: ");
 
    
  }while (display.nextPage());

  
}
void epaper(){

display.setPartialWindow(80,40, 200, 200);
  display.firstPage();

  do{
   
    display.fillScreen(GxEPD_WHITE);
    u8g2Fonts.setCursor(80, 60);
      
    u8g2Fonts.print(UID); 
  
     u8g2Fonts.setCursor(100, 90);
    u8g2Fonts.print(BPM);
 
    u8g2Fonts.setCursor(100, 120);
    u8g2Fonts.print(Temp);
        u8g2Fonts.setCursor(170, 120);
    u8g2Fonts.print("C");
    u8g2Fonts.setCursor(100, 150);
    u8g2Fonts.print(SPO2);
       u8g2Fonts.setCursor(150, 150);
    u8g2Fonts.print("%");
  u8g2Fonts.setCursor(85, 180);
    u8g2Fonts.print(BloodPressure);
       u8g2Fonts.setCursor(130, 180);
    u8g2Fonts.print("mmHg");
  

  }while(display.nextPage());
}

