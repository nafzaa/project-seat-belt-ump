#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <CytronEZMP3.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

CytronEZMP3 mp3;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10
 
 #define outputA 6
 #define outputB 7
 
 int counter = 0; 
 int aState;
 int aLastState;  

 int reedsw = 5;
 int valuereedsw = 0;

 int relay = 8;
 int relay2 = 9;

 int state = 0;
 int state1 = 0;

 int slowengine = 2;
 int stopengine = 4;
 int readtime = 0;
 int readtimeslow = 0;
 int readtimestop = 0; 
 
 void setup() { 
   pinMode (outputA,INPUT);
   pinMode (outputB,INPUT);
   pinMode (reedsw,INPUT);
   pinMode (relay, OUTPUT);
   pinMode (relay2, OUTPUT);

   mp3.begin(10, 11);
   Serial.begin (9600);
   aLastState = digitalRead(outputA);  

   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
   }
 } 
 void loop() { 
   tmElements_t tm;
   RTC.read(tm);

   valuereedsw = digitalRead (reedsw);
   aState = digitalRead(outputA); 
   if (aState != aLastState){     
     if (digitalRead(outputB) != aState) { 
       counter ++;
     } else {
       counter --;
     } 
     Serial.print("Position: ");
     Serial.println(counter);
   } 
   aLastState = aState; 

   if (valuereedsw == LOW){
    rotaryfunction();
    state1 = 0;
    mp3.playTrack(2);
    //Serial.println ("on");
    }

   else {
     if (state1 == 0){
     display.clearDisplay();
     testdrawstyles1();
     state1 = 1;
     mp3.playTrack(1);
     readtimeslow = tm.Minute + slowengine;
     readtimestop = tm.Minute + stopengine;
     Serial.println ("seat belt off");
     }
     readtime = tm.Minute;     
     Serial.print (readtime);
     Serial.print ("  ");
     Serial.print (readtimeslow);
     Serial.print ("  ");
     Serial.println (readtimestop);    

     if (readtime == readtimeslow){
      digitalWrite (relay2, HIGH);
      Serial.println ("engine slow");
      }

     if (readtime == readtimestop){
      digitalWrite (relay, LOW);
      digitalWrite (relay2, LOW);
      Serial.println ("engine stop");
      }
    }
   
 }


void rotaryfunction (){
  if (counter >= 100){
    digitalWrite (relay, HIGH);

    if (state == 0){
      test();
      state = 1;
      }
    testdrawstyles();
    }
    

  else {
    digitalWrite (relay, LOW);
    display.clearDisplay();
    testdrawstyles1();
    }
  }

void testdrawstyles() {
  display.clearDisplay();
  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3,10);
  display.print("START"); 
  display.setCursor(3,30);
  display.print("ENGINE");

  display.display();
  delay(2000);
}

void testdrawstyles1() {
  display.clearDisplay();
  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10,10);
  display.print("SEAT BELT"); 
  display.setCursor(50,40);
  display.print("OFF");

  display.display();
  delay(2000);
}

void test() {
  display.clearDisplay();
  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10,10);
  display.print("SEAT BELT"); 
  display.setCursor(55,40);
  display.print("ON");

  display.display();
  delay(2000);
}
