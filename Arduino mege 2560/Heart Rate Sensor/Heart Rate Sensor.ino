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

void setup() {
 display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();
}

void loop()
{

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
  {    // and it's the first time it was high since the last time it was low...
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