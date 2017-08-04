#include "userDef.h"
#include <JQ6500.h>

#ifdef Audio_SoftSerial
#include<SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX
JQ6500 Audio(&mySerial);     //音频控制类实例,使用软串口（2，3）
#else
JQ6500 Audio(&Serial1);
#endif

#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);//设置彩灯个数，引脚

void setup() {
  pinMode(BODY_PIN1, INPUT);
  pinMode(BODY_PIN2, INPUT);
  // initialize serial:
  Serial.begin(9600);
  Audio.init(DEVICE_FLASH, MODE_ONE_STOP, MUSICVOL);
  delay(200);
  Audio.volumn(MUSICVOL);

  strip.begin();	//初始化LED
  strip.setPixelColor(0, strip.Color(255, 255, 255));
  strip.show();
}

void loop() {
  if (getPIR(BODY_PIN1, BODY_PIN2)) { //COMING IN
    strip.setPixelColor(0, strip.Color(0, 255, 0));
    strip.show();
    Audio.choose(1);
    Serial.println("COMING IN");
    delay(2500);
    strip.setPixelColor(0, strip.Color(255, 255, 255));
    strip.show();
  }

  if (getPIR(BODY_PIN2, BODY_PIN1)) { //GOING OUT
    strip.setPixelColor(0, strip.Color(0, 0, 255));
    strip.show();
    Audio.choose(2);
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
