//OLED
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C
#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Heart rate

#define UpperThreshold 560  //--> Determine which Signal to "count as a beat", and which to ingore.
#define LowerThreshold 530
int a = 0;
int lasta = 0;
int lastb = 0;
int LastTime = 0;
int ThisTime;
bool BPMTiming = false;
bool BeatComplete = false;
int BPM = 0;


#define UpperThreshold 550
#define LowerThreshold 500
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

//SPo2
#include <MAX3010x.h>
#include "filters.h"

// Sensor (adjust to your sensor type)
MAX30105 sensor;
const auto kSamplingRate = sensor.SAMPLING_RATE_400SPS;
const float kSamplingFrequency = 400.0;

// Finger Detection Threshold and Cooldown
const unsigned long kFingerThreshold = 10000;
const unsigned int kFingerCooldownMs = 500;

// Edge Detection Threshold (decrease for MAX30100)
const float kEdgeThreshold = -2000.0;

// Filters  
const float kLowPassCutoff = 5.0;
const float kHighPassCutoff = 0.5;

// Averaging
const bool kEnableAveraging = false;
const int kAveragingSamples = 5;
const int kSampleThreshold = 5;

// Filter Instances
LowPassFilter low_pass_filter_red(kLowPassCutoff, kSamplingFrequency);
LowPassFilter low_pass_filter_ir(kLowPassCutoff, kSamplingFrequency);
HighPassFilter high_pass_filter(kHighPassCutoff, kSamplingFrequency);
Differentiator differentiator(kSamplingFrequency);
MovingAverageFilter<kAveragingSamples> averager_bpm;
MovingAverageFilter<kAveragingSamples> averager_r;
MovingAverageFilter<kAveragingSamples> averager_spo2;

// Statistic for pulse oximetry
MinMaxAvgStatistic stat_red;
MinMaxAvgStatistic stat_ir;

// R value to SpO2 calibration factors

float kSpO2_A = 1.5958422;
float kSpO2_B = -34.6596622;
float kSpO2_C = 112.6898759;

// Timestamp of the last heartbeat
long last_heartbeat = 0;

// Timestamp for finger detection
long finger_timestamp = 0;
bool finger_detected = false;

// Last diff to detect zero crossing
float last_diff = NAN;
bool crossed = false;
long crossed_time = 0;


//Blood Pressure

int val_hb = 0;// blood pressure signal
int val_bp = 0;// average blood pressure 
int cnt = 0; 
char input; 
int test=0;
int countbp=0;
//WiFiEsp && cloud
#include "WiFiEsp.h"
#include "ThingSpeak.h"
#define ESP_BAUDRATE 115200
char ssid[] = "Linksys00371";  // your network SSID (name)
char pass[] = "jbx83c937s";    // your network password

int status = WL_IDLE_STATUS;  // the Wifi radio's status


unsigned long myChannelNumber_Patient1 = "1933848";
const char *myWriteAPIKey_Patient1 = "1PJ4RXNDB6UAT3KV";
const char *myReadAPIKey_Patient1 = "EU3XU2BAN6P0ZMCE";

unsigned long myChannelNumber_Patient2 = "1912416";
const char *myWriteAPIKey_Patient2 = "69USWDE5VY69SBL7";
const char *myReadAPIKey_Patient2 = "P3PODNET8SU9A1CU";

 int bshow ;
    float fshow ;
    float tpshow ;
    float bpshow;

WiFiEspClient client;  //create a wifi client
uint32_t timeout, loop_timeout;
//rfid
#include <MFRC522.h>
#include <ArduinoJson.h>
#define RST_PIN 5
#define SS_PIN 53  //就是模組上的SDA接腳
MFRC522 mfrc522;   // 建立MFRC522實體

//SQL
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

char str[32] = "";
int stat = 0;
//json
DynamicJsonDocument jsonBuffer(1024);
char json[] = "{\"rfid\":\"Temp\",\"ESp02\",\"BPM\",\"BloodPressure\"}";
DeserializationError error = deserializeJson(jsonBuffer, json);
//communication

// Define a Software Serial object and the used pins
SoftwareSerial softSerial(9, 8);  // RX, TX
String str1;
String str2;
String str3;
String str4;
String strID;
void setup() {

  OLEDSetup();
TempSetup();
spO2Setup();
BloodPrssureSetup();
WiFiEspSetup();
rfidSetup();
 Communication_Setup();
}

void loop() {

printMenu();
read();



}



void OLEDSetup() {
    Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();
}
	void printMenu()
   
	{    
    while(1){
 rfid();
  CloudUploading();
 SendtoAnother (); 
   



        display.clearDisplay();
      read();
    display.setTextSize(1);
  display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print(" ***    Menu    ***	 " );
    display.println("[1] Heart Rate  	 ");
    display.println("[2] Temperature	 ");
    display.println( "[3] SpO2   ");
    display.println("[4] Blood Pressure" );
   display.display();
if (input=='1') 
{  display.clearDisplay();

  while(1){
     rfid();
 CloudUploading();
 SendtoAnother (); 
    
    BMP ();
    read();
display.setTextColor(WHITE);
    display.setCursor(0, 0);
     display.setTextSize(1);
    display.print("Press 'Q' to menu" );
  if (input=='Q') { break; }
  display.display();
  }  
     
    } else if (input=='2'){
      display.clearDisplay();

  while(1){
     rfid();
     CloudUploading();
 SendtoAnother (); 
     temp();
    read();
display.setTextColor(WHITE);
    display.setCursor(0, 0);
     display.setTextSize(1);
    display.print("Press 'Q' to menu" );
  if (input=='Q') { break; }
  display.display();
  }

    }else if (input=='3'){
display.clearDisplay();
 while(1){
    rfid();
    CloudUploading();
 SendtoAnother (); 
     spO2();
    read();
display.setTextColor(WHITE);
    display.setCursor(0, 0);
     display.setTextSize(1);
    display.print("Press 'Q' to menu" );
  if (input=='Q') { break; }
  display.display();
  }



      
    }else if (input=='4'){
 
countbp=0;
display.clearDisplay();
 while(1){
     if ( countbp==1) { break; }
  display.display();
    rfid();
         CloudUploading();
 SendtoAnother (); 
  BloodPrssure();
    read();
display.setTextColor(WHITE);
    display.setCursor(0, 0);
    // display.setTextSize(1);
    //display.print("Press 'Q' to muen" );

  }
     
    }

	}

  }

 
void read(){ 
    if (Serial.available()) {
        input = Serial.read();
     //   Serial.print("You typed:" );
    //    Serial.println(input);
    }
}

void BMP (){

  if (a > 127)  // Clear EEG
  {
    display.clearDisplay();
    a = 0;
    lasta = a;
  
  }

  ThisTime = millis();        //Get the time
  int value = analogRead(2);  //read anlog
  display.setTextColor(WHITE);
  int b = 60 - (value / 16);


  display.writeLine(lasta, lastb, a, b, WHITE);  //write a Line
  lastb = b;
  lasta = a;
    // If the value is high...
  if (value > UpperThreshold)  // Heart beat leading edge detected.
  {    // and ....
    if (BeatComplete)  //calculate the bpm
    {    
 // This is the start of a beat.

      // Record the number of milliseconds since the start of 
      // the previous beat
      unsigned long millisecondsPerBeat = millis() - LastTime;

      // divide the number of milliseconds into the number of
      // milliseconds in a minute (60000) to get beats per minute
      BPM = 60000.0 / millisecondsPerBeat;
      BPMTiming = false;
      BeatComplete = false;
    }
    if (BPMTiming == false)  //  this will always run if (BeatComplete)
    {
      LastTime = millis();
      BPMTiming = true;  // this switches BPMTiming as set in if (BeatComplete) from false to true
    }
  } 
  if ((value < LowerThreshold) & (BPMTiming))  // Heart beat trailing edge detected.
    BeatComplete = true;
    if (BPM > 60) {
    ThingSpeak.setField(1, BPM);
      jsonBuffer["BPM"] = BPM;
  }


  Print_BPM(BPM);
  a++;

 
}
void Print_BPM(int BPM) {
  display.setTextSize(2);
  display.writeFillRect(0, 50, 128, 16, BLACK);
  display.setCursor(0, 50);
  display.print("BPM:");
  display.print(BPM);

  display.display();
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
       float temp1 = (float)my_ir.to / 100;
        ThingSpeak.setField(3, temp1);
        jsonBuffer["Temp"] = temp1;

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


void spO2Setup(){





  if(sensor.begin() && sensor.setSamplingRate(kSamplingRate)) { 
    Serial.println("Sensor initialized");
  }
  else {
    Serial.println("Sensor not found");  
    while(1);
  }
}


void spO2(){

auto sample = sensor.readSample(1000);
  float current_value_red = sample.red;
  float current_value_ir = sample.ir;
  
  // Detect Finger using raw sensor value
  if(sample.red > kFingerThreshold) {
    if(millis() - finger_timestamp > kFingerCooldownMs) {
      finger_detected = true;
    }
  }
  else {
    // Reset values if the finger is removed
    differentiator.reset();
    averager_bpm.reset();
    averager_r.reset();
    averager_spo2.reset();
    low_pass_filter_red.reset();
    low_pass_filter_ir.reset();
    high_pass_filter.reset();
    stat_red.reset();
    stat_ir.reset();
    
    finger_detected = false;
    finger_timestamp = millis();
  }

  if(finger_detected) {
    current_value_red = low_pass_filter_red.process(current_value_red);
    current_value_ir = low_pass_filter_ir.process(current_value_ir);

    // Statistics for pulse oximetry
    stat_red.process(current_value_red);
    stat_ir.process(current_value_ir);

    // Heart beat detection using value for red LED
    float current_value = high_pass_filter.process(current_value_red);
    float current_diff = differentiator.process(current_value);

    // Valid values?
    if(!isnan(current_diff) && !isnan(last_diff)) {
      
      // Detect Heartbeat - Zero-Crossing
      if(last_diff > 0 && current_diff < 0) {
        crossed = true;
        crossed_time = millis();
      }
      
      if(current_diff > 0) {
        crossed = false;
      }
  
      // Detect Heartbeat - Falling Edge Threshold
      if(crossed && current_diff < kEdgeThreshold) {
        if(last_heartbeat != 0 && crossed_time - last_heartbeat > 300) {
          // Show Results
          int bpm = 60000/(crossed_time - last_heartbeat);
          float rred = (stat_red.maximum()-stat_red.minimum())/stat_red.average();
          float rir = (stat_ir.maximum()-stat_ir.minimum())/stat_ir.average();
          float r = rred/rir;
          float spo2 = kSpO2_A * r * r + kSpO2_B * r + kSpO2_C;
          
          if(bpm > 50 && bpm < 250) {
            // Average?
            if(kEnableAveraging) {
              int average_bpm = averager_bpm.process(bpm);
              int average_r = averager_r.process(r);
              int average_spo2 = averager_spo2.process(spo2);
  
              // Show if enough samples have been collected
              if(averager_bpm.count() >= kSampleThreshold) {
                Serial.print("Time (ms): ");
                Serial.println(millis()); 
                Serial.print("Heart Rate (avg, bpm): ");
                Serial.println(average_bpm);
                Serial.print("R-Value (avg): ");
                Serial.println(average_r);  
                Serial.print("SpO2 (avg, %): ");
                   if (average_spo2 > 100) {
                  average_spo2 = 99;


                } else if (average_spo2 < 90) {
                  average_spo2 = 90;
                }
                Serial.println(average_spo2);  
                  ThingSpeak.setField(2, average_spo2);
                  jsonBuffer["ESp02"] = average_spo2;
                print_spO2(average_spo2) ;




              }
            }
            else {
              Serial.print("Time (ms): ");
              Serial.println(millis()); 
              Serial.print("Heart Rate (current, bpm): ");
              Serial.println(bpm);  
              Serial.print("R-Value (current): ");
              Serial.println(r);
              Serial.print("SpO2 (current, %): ");
                  if (spo2 > 100) {
                  spo2= 99;
       

                } else if (spo2 < 90) {
                  spo2 = 90;
                }
                ThingSpeak.setField(2, spo2);
              Serial.println(spo2);  
                  jsonBuffer["ESp02"] = spo2;
              print_spO2(spo2) ;
            }
          }

          // Reset statistic
          stat_red.reset();
          stat_ir.reset();
        }
  
        crossed = false;
        last_heartbeat = crossed_time;
      }
    }

    last_diff = current_diff;
  }
}
void print_spO2(float X){

          display.setTextSize(2);
  display.writeFillRect(0, 50, 128, 16, BLACK);
  display.setTextColor(WHITE);
   display.setCursor(0,50);
display.print("SpO2(%):");
display.print((float) X );
  display.display();
}




void BloodPrssureSetup(){



  pinMode(12, INPUT_PULLUP); //Button B record Diastolic pressure
  pinMode(13, INPUT_PULLUP); // Button A -record Systolic pressure
  
  
  display.clearDisplay();
  display.setTextSize(1); // Normal 1:1 pixel scale
  display.setTextColor(WHITE,BLACK); // Draw white text
  


  display.display();
  delay(2000);


}

void BloodPrssure(){

 // measuare the voltage HB-o/p of Pump Control Circuit

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
       ThingSpeak.setField(4, test);
        jsonBuffer["BloodPressure"] = test;
    //Serial.println(test);
  }
     if(digitalRead(12)==LOW){
         analogWrite(13,0);
 
            while(1){
                read();
  display.setTextSize(3); // Normal 1:1 pixel scale
  display.setTextColor(WHITE,BLACK); // Draw white text
  
  display.setCursor(0,0);
  
  display.print("BP:");
  display.setCursor(60,0);
  display.print(test);
       display.display();  
       display.setTextSize(1);   
   display.setCursor(0,40);
 display.print("Press 'Q' to menu" );

  if (input=='Q') { countbp=1;
  break;  
  }     
  
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
void WiFiEspSetup() {
  // initialize serial for debugging
  Serial.begin(115200);
  Serial1.begin(ESP_BAUDRATE);
  // initialize ESP module
  WiFi.init(&Serial1);
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network
      Serial.print(".");

      delay(5000);
    }

    ThingSpeak.begin(client);
    Serial.println("\nConnected");
    loop_timeout = millis();
  }
}

void CloudReading(int x) {



  if (x == 1) {
         
     display.setCursor(0,40);  // Start at top-left corner
    display.print("Reading data         from the cloud");
       display.display();
       
    String time = ThingSpeak.readCreatedAt(1933848, "EU3XU2BAN6P0ZMCE");
    int stat = ThingSpeak.readMultipleFields(1933848, "EU3XU2BAN6P0ZMCE");

    if (stat == 200) { Serial.println("Connected cloud Success."); }
    else {

      Serial.println("Problem updating channel. HTTP error code " + String(stat));
    }

    bshow = ThingSpeak.readIntField(1933848, 1, "EU3XU2BAN6P0ZMCE");
    fshow = ThingSpeak.readFloatField(1933848, 2, "EU3XU2BAN6P0ZMCE");
    tpshow = ThingSpeak.readFloatField(1933848, 3, "EU3XU2BAN6P0ZMCE");
    bpshow = ThingSpeak.readFloatField(1933848, 4, "EU3XU2BAN6P0ZMCE"); 
 str1 = String( bshow );
                str1 = "B" + str1;
               
str2 = String(fshow );
                str2 = "O" + str2;
str3 = String( tpshow );
                str3 = "T" + str3;
str4 = String(bpshow );
                str4 = "P" + str4;
    strID= "I1";

    Serial.println("Patient UID:04125042");
    Serial.print("Your last body test is in:");
    Serial.println(time);
    Serial.print("Heart Rate (avg bpm): ");
    Serial.println(bshow);
    Serial.print("spO2 (%):");
    Serial.println(fshow);
    Serial.print("Temperature (°C):");
    Serial.println(tpshow);
    Serial.print("Blood Pressure (mmHg):");
    Serial.println(bpshow);
    display.clearDisplay();

  } else if (x == 2) {
      
     display.setCursor(0, 40);  // Start at top-left corner
    display.print("Reading data         from the cloud");
       display.display();
    String time = ThingSpeak.readCreatedAt(1912416, "1IUNF1NYU6HET31H");
    int stat = ThingSpeak.readMultipleFields(1912416, "1IUNF1NYU6HET31H");


    if (stat == 200) {
      Serial.println("Connected cloud Success.");
    } else {

      Serial.println("Problem updating channel. HTTP error code " + String(stat));
    }

  bshow = ThingSpeak.readIntField(1912416, 1, "1IUNF1NYU6HET31H");
     fshow = ThingSpeak.readFloatField(1912416, 2, "1IUNF1NYU6HET31H");
   tpshow = ThingSpeak.readFloatField(1912416, 3, "1IUNF1NYU6HET31H");
    bpshow = ThingSpeak.readFloatField(1912416, 4, "1IUNF1NYU6HET31H");
      str1 = String( bshow );
                str1 = "B" + str1;
               
  str2 = String(fshow );
                str2 = "O" + str2;
str3 = String( tpshow );
                str3 = "T" + str3;
str4 = String(bpshow );
                str4 = "P" + str4;

strID= "I2";
    Serial.println("Patient UID:CCF51E30");

    Serial.print("Your last body test is in:");
    Serial.println(time);
    Serial.print("Heart Rate (avg bpm): ");
    Serial.println(bshow);
    Serial.print("spO2 (%):");
    Serial.println(fshow);
    Serial.print("Temperature (°C):");
    Serial.println(tpshow);
    Serial.print("Blood Pressure (mmHg):");
    Serial.println(bpshow);
      display.clearDisplay();



  }
}


void CloudUploading() {
   
  timeout = millis();
  if (timeout - loop_timeout > 60000) {
    // check the network connection once every 10 seconds
    Serial.println();
    printWifiData();
      display.clearDisplay();
     display.setCursor(0, 0);  // Start at top-left corner
    display.print("Uploading data to   the cloud");
       display.display();

    int x;
    if (jsonBuffer["rfid"] == "04125042") {

      x = ThingSpeak.writeFields(myChannelNumber_Patient1, myWriteAPIKey_Patient1);

    } else if (jsonBuffer["rfid"] == "CCF51E30") {
      x = ThingSpeak.writeFields(myChannelNumber_Patient2, myWriteAPIKey_Patient2);



    } else {

      Serial.println("This patient is not in our database");
    }


    if (x == 200) {
      Serial.println("Health data update successful.");
    } else {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    loop_timeout = millis();
     display.clearDisplay();
  }

   
  
}

void printWifiData() {
 
 
       // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

}
void rfidSetup() {
   // 初始化SPI介面
  SPI.begin();
  mfrc522.PCD_Init(SS_PIN, RST_PIN);  // 初始化MFRC522卡
  Serial.print(F("Reader "));
  Serial.print(F(": "));
  mfrc522.PCD_DumpVersionToSerial();  // 顯示讀卡設備的版本
}

void rfid() {
      

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {

display.clearDisplay();
      display.setCursor(0, 0);  // Start at top-left corner
    display.print("Detected a RFID cards");
      display.display();
      delay(200);

    MySQL_Connection conn((Client *)&client);
    IPAddress server_addr(172, 0, 0, 1);
    if (conn.connect(server_addr, 3306, "root", "")) { Serial.println("sql good"); }
    // 顯示卡片內容
   
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);  
    array_to_string(mfrc522.uid.uidByte, 4, str);
    Serial.println();
    Serial.print(F("PICC type: "));
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.println(mfrc522.PICC_GetTypeName(piccType)); 
    mfrc522.PICC_HaltA(); 
    jsonBuffer["rfid"] = str;
 Serial.print(F("Card UID:"));
   display.setCursor(0, 20);  // Start at top-left corner
    display.print("UID:");
    display.print(str);
     display.display();
    serializeJson(jsonBuffer, Serial);
      
    char INSERT_SQL[] = "INSERT INTO patients(rfid,bpm,Temp,ESp02) VALUES ('$rfid','?','?','?')";
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    //execute
    cur_mem->execute(INSERT_SQL);
    delete cur_mem;
    //Serial.println("Data Saved.");
    if (jsonBuffer["rfid"] == "04125042") {

      CloudReading(1);

    } else if (jsonBuffer["rfid"] == "CCF51E30") {
      CloudReading(2);
    }
  }
}
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}

void array_to_string(byte array[], unsigned int len, char buffer[]) {
  for (unsigned int i = 0; i < len; i++) {
    byte nib1 = (array[i] >> 4) & 0x0F;
    byte nib2 = (array[i] >> 0) & 0x0F;
    buffer[i * 2 + 0] = nib1 < 0xA ? '0' + nib1 : 'A' + nib1 - 0xA;
    buffer[i * 2 + 1] = nib2 < 0xA ? '0' + nib2 : 'A' + nib2 - 0xA;
  }
  buffer[len * 2] = '\0';
}

void sendDataSerial(String data) {

  int len = data.length() + 1;
  char buf[len];
  data.toCharArray(buf, len);
  softSerial.write(buf);
}

void Communication_Setup() {

  softSerial.begin(19200);
}
void SendtoAnother (){
sendDataSerial(strID);
sendDataSerial(str1);
sendDataSerial(str2);
sendDataSerial(str3);
sendDataSerial(str4);


}