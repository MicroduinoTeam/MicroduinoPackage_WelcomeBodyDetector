#include "userDef.h"
#include <Microduino_Audio.h>


//Core UART Port: [SoftSerial] [D2,D3]
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); /* RX:D2, TX:D3 */
#define AudioSerial mySerial
#endif

//Core+ UART Port: [Serial1] [D2,D3]
#if defined(__AVR_ATmega1284P__) || defined (__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define AudioSerial Serial1
#endif

Audio audio(&AudioSerial);

#include <Microduino_ColorLED.h>
ColorLED strip = ColorLED(1, PIN);//设置彩灯个数，引脚

void setup() {
  pinMode(BODY_PIN1, INPUT);
  pinMode(BODY_PIN2, INPUT);
  // initialize serial:
  Serial.begin(9600);
  audio.begin(DEVICE_FLASH, MODE_ONE_STOP, MUSICVOL);
  delay(200);
  audio.setVolume(MUSICVOL);

  strip.begin();	//初始化LED
  strip.setPixelColor(0, strip.Color(255, 255, 255));
  strip.show();
}

void loop() {
  if (getPIR(BODY_PIN1, BODY_PIN2)) { //COMING IN
    strip.setPixelColor(0, strip.Color(0, 255, 0));
    strip.show();
    audio.chooseMusic(1);
    Serial.println("COMING IN");
    delay(2500);
    strip.setPixelColor(0, strip.Color(255, 255, 255));
    strip.show();
  }

  if (getPIR(BODY_PIN2, BODY_PIN1)) { //GOING OUT
    strip.setPixelColor(0, strip.Color(0, 0, 255));
    strip.show();
    audio.chooseMusic(2);
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
