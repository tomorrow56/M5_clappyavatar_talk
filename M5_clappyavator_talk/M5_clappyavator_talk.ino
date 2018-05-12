// Talking test for M5Stack
// 2018/5/12 @tomorrow56
// 
// see:
// https://twitter.com/meganetaaan/status/982258576361078784
// http://blog-yama.a-quest.com/?eid=970191
// prerequisite: AquesTalk pico for ESP32
// https://www.a-quest.com/download.html#a-etc
#include <M5Stack.h>
#include "clappyavator.h"
#include "M5StackUpdater.h"
#include "AquesTalkTTS.h"

const char* licencekey = "xxxx-xxxx-xxxx-xxxx";  // AquesTalk-ESP licencekey

Avator *avator;
int count = 0;
bool displayOn = true;

void setup()
{
  M5.begin();
  
  if(digitalRead(BUTTON_A_PIN) == 0) {
    Serial.println("Will Load menu binary");
    updateFromFS(SD);
    ESP.restart();
  }
  
  avator = new Avator();

  M5.Lcd.fillRect(0, 0, 320, 240, SECONDARY_COLOR);
  M5.Lcd.setCursor(60, 10);
  M5.Lcd.setTextColor(PRIMARY_COLOR);
  M5.Lcd.setTextSize(2);
  M5.Lcd.print("#ClappyChallenge");

  setDisplayOn(true);
  avator->init();

  int iret;
  
   // Init Text-to-Speech (AquesTalk-ESP + I2S + Internal-DAC)
  iret = TTS.create(licencekey);
  if(iret){
       Serial.print("ERR: TTS_create():");
       Serial.println(iret);
  }
}

void loop()
{
  int iret;
  struct tm timeinfo;
  char koe[100];

  if(M5.BtnA.wasPressed()){
    setDisplayOn(true);

    Serial.println("BtnA.wasPressed");
    sprintf(koe,"kura'ppi-,/cha'renji.");
    iret = TTS.play(koe, 100);
    if(iret){
      Serial.println("ERR:TTS.play()");
    }
  }
  else if(M5.BtnB.wasPressed()){
    setDisplayOn(true);

    Serial.println("BtnB.wasPressed");
    sprintf(koe,"iwaokasann,wa,/cho'tto.");
    iret = TTS.play(koe, 100);
    if(iret){
      Serial.println("ERR:TTS.play()");
    }
  }
  else if(M5.BtnC.wasPressed()){
    Serial.println("BtnC.wasPressed");
    TTS.stop();
    setDisplayOn(false);
  }
  if(displayOn){
    avatorUpdate();
  }
  M5.update();
}

void setDisplayOn(bool onState){
  if(onState){
    M5.Lcd.setBrightness(60);
    displayOn = true;
  }else{
    M5.Lcd.setBrightness(0);
    displayOn = false;
  }
}

int percent_prev;

void avatorUpdate(){
  count++;
  int percent = abs(TTS.getLevel()) * 100 / 256;

  if(percent != percent_prev){
    avator->openMouth(percent);
  }
  percent_prev = percent;

  if (count % 100 == 97){
    avator->openEye(false);
  }
  if (count % 100 == 0){
    avator->openEye(true);
    count = random(50);
  }
  delay(50);
}

