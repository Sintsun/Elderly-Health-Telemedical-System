// This program is used to control the Pressure Sensor Module (PSM) for measuring the blood pressure manually.
// You need to connect proper wiring between the Mega 2560, Pressure Sensor Module and the OLED as described below.
// Developed by Andy Chan 20 Oct 2022

// Circuit Wiring betweem Mega 2560 and PSM
//  Conntect a Button (A) to the Digital Pin D13 of Mega Board, other end to GND
//  Connect a Button (B) to the Digital Pin D12 of the Mega Board, other end to GND
//  Connect HB-o/p pin of the pump circuit to A0 of the Mega Board
//  Connect BP-o/p pin of the pump circuit of A1 of the Mega Board

// Power of Pump Control Circuit
//  Connect 5V, 3.3V, GND of the PSM to the 5V,3V, and GND of the development board 

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
//  OLED -> Arduino Mega
//   GND -> GND
//   VCC -> 5V
//   SCL -> SCL(21)
//   SDA -> SDA(20)

// Operation of the Blood Pressure Monitor in menu mode
// 1) Open the serial monitor of the Arduino IDE which is used to display the blood pressure waveform
// 2) The OLED shows the Realtime BP when the air is pumping into the cuff
// 3) Press the "Air Pump" of the PSM while observing the blood pressure waveform
// 4) Stop pressing the "Air Pump" Button when the Realtime pressure reach 130mmHg
// 5) Wait the blood pressure to reduce gradually and observe the blood pressure waveform to reach max and steady amplitude
// 6) Press button A to record the Systolic Blood Pressure on the OLED
// 7) Continue to observing the amplitude until it reduce to around half the the peak value
// 8) Press button B to record the Diastolic Blood Pressure on the OLED


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_I2C_ADDR 0x3C

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // SSD1306 doesn't have a reset pin

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setupOLED(void) {
// SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR)) { // Address 0x3C for SSD1306 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;) yield(); // Don't proceed, loop forever
  }

}

int test=0;
int val_hb = 0;// blood pressure signal
int val_bp = 0;// average blood pressure 
int cnt = 0; 

void setup() {
  Serial.begin(115200);

  pinMode(12, INPUT_PULLUP); //Button B record Diastolic pressure
  pinMode(13, INPUT_PULLUP); // Button A -record Systolic pressure
  
  
  
  //display.setCursor(0, 16); // Start at top-left corner
 // display.print("RT_BP  :         mmHg");
  
 // display.setCursor(0, 32);
 // display.print("SYS_BP :         mmHg");

 // display.setCursor(0,48);
  //display.print("DIA_BP :         mmHg");

  delay(2000);
}


void loop() {
  // measuare the voltage HB-o/p of Pump Control Circuit
setupOLED();  // setup data screeen to display data
 int count;
  val_hb = analogRead(A0); 
 
  //Serial.println(val_hb);
  cnt = (cnt + 1)%20;
  
  // measure the voltage BP-o/p of the PSM
  // and convert it into corresponding pressure in mmHg
  // BP(mmHg) = 80.0064*DVBP*Vcc/1023 - 24.0019; 
  // DVBP: digital value of the pressure sensor output voltage sampled by analogRead
  // Vcc: 5V 
  // The above formula is based on pressure sensor sensitivity chart
  val_bp = int((80.0064*5*analogRead(A1)/1023)- 24.0019); 

  // avoid negative readings
  if (val_bp <= 0) {
    val_bp = 0;
  } 
  if(val_bp <130&& count==0){
   display.clearDisplay();
  display.setTextSize(3); // Normal 1:1 pixel scale
  display.setTextColor(WHITE,BLACK); // Draw white text
  
  display.setCursor(0,0);
  display.cp437(true); // Use full 256 char 'Code Page 437' font
  display.print("Pumping");
  
  
  }
   analogWrite(12,HIGH);
  if(val_bp >=130){
     count++;
    analogWrite(12,LOW);
      display.clearDisplay();
  display.setTextSize(3); // Normal 1:1 pixel scale
  display.setTextColor(WHITE,BLACK); // Draw white text
  
  display.setCursor(0,0);
  display.cp437(true); // Use full 256 char 'Code Page 437' font
  display.print("Wait   for 10s");
  
  display.display();
    delay(10000);
    display.clearDisplay();
    test=int((80.0064*5*analogRead(A1)/1023)- 24.0019);  //value output 
    Serial.println(test);
  }
     if(digitalRead(12)==LOW){
         analogWrite(13,0);
            while(1){
  display.setTextSize(3); // Normal 1:1 pixel scale
  display.setTextColor(WHITE,BLACK); // Draw white text
  
  display.setCursor(0,0);
  
  display.print("BP:");
  display.setCursor(60,0);
  display.print(test);
       display.display();     
       
            }

    }
  // update realtime blood pressure every 10 samples interval
  if ((cnt % 10) == 0) {
    //display.fillRect(50,16,45,8, BLACK);
 //   display.setTextColor(WHITE,BLACK); // Draw white text
   // display.setCursor(50, 16);
  //  display.print(String(val_bp,DEC));
   // display.display();
  }

  // record the current blood pressure as Sys Blood Pressure
  // when Button A is pressed. Reading shows on OLED
  int SYS_RB = digitalRead(13);

 
  //  display.fillRect(50,32,45,8, BLACK);
//    display.setTextColor(WHITE,BLACK); // Draw white text
   // display.setCursor(50, 32);
  //  display.print(String(val_bp,DEC));
  //  display.display();
  

  // record the current blood pressure as Dia Blood Pressure
  // when Button B is pressed. Reading shows on OLED
  int DIA_RB = digitalRead(12);
 
   // display.fillRect(50,48,45,8, BLACK);
  //  display.setTextColor(WHITE,BLACK); // Draw white text
  //  display.setCursor(50, 48);
 //   display.print(String(val_bp,DEC));
  //  display.display();
    
  delay(20);
  display.display();
}