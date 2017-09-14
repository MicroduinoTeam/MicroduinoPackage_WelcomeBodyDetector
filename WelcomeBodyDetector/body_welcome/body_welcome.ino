#include "userDef.h"
#include <Microduino_Audio.h>
#include <Microduino_AudioPro.h>
#include "file.h"
AudioPro midiPlayer;


#include <Microduino_ColorLED.h>
ColorLED strip = ColorLED(1, PIN);//设置彩灯个数，引脚

void setup() {
  pinMode(BODY_PIN1, INPUT);
  pinMode(BODY_PIN2, INPUT);
  // initialize serial:
  Serial.begin(115200);
  if (! midiPlayer.begin()) { // initialise the music player
    Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
    while (1);
  }
  Serial.println(F("VS1053 found"));
  midiPlayer.setVolume(127);  //left & right 0-127
  midiPlayer.useInterrupt(VS1053_PIN_DREQ);  // DREQ int

  strip.begin();	//初始化LED
  strip.setPixelColor(0, strip.Color(255, 255, 255));
  strip.show();
}

void loop() {
  if (getPIR(BODY_PIN1, BODY_PIN2)) { //COMING IN
    strip.setPixelColor(0, strip.Color(0, 255, 0));
    strip.show();
    midiPlayer.playROM(welcome, sizeof(welcome));
    Serial.println("COMING IN");
    delay(2500);
    strip.setPixelColor(0, strip.Color(255, 255, 255));
    strip.show();
  }

  if (getPIR(BODY_PIN2, BODY_PIN1)) { //GOING OUT
    strip.setPixelColor(0, strip.Color(0, 0, 255));
    strip.show();
    midiPlayer.playROM(bye, sizeof(bye));
    Serial.println("GOING OUT");
    delay(2500);
    strip.setPixelColor(0, strip.Color(255, 255, 255));
    strip.show();
  }
}

boolean getPIR(int _pinA, int _pinB) {
  if (digitalRead(_pinA) && !digitalRead(_pinB)) {  //激活一边按键时
    delay(5);
    if (digitalRead(_pinA) && !digitalRead(_pinB)) {
      unsigned long _time = millis();
      while (millis() - _time < 1000) { //如果1s内激活另一边按键，即为一次滑动操作
        if (digitalRead(_pinA) && digitalRead(_pinB)) {
          return true;
        }
      }
    }
  }
  return false;
}
