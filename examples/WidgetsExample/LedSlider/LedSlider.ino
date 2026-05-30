/*
 * Copyright 2026 [han-sunghyun]
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 */

#include <Linker.h>

#define TX 2
#define RX 3

#define LED_R_SLIDE_ID 0
#define LED_G_SLIDE_ID 1
#define LED_B_SLIDE_ID 2

#define LED_R_PIN 11
#define LED_G_PIN 10
#define LED_B_PIN 9

Linker linker;

SliderStyleBuilder sliderStyle;

String64 sliderStyleStr;

void callback(int ch) {
  if (linker.isSliderUpdated()) {
    int slideId = linker.getSliderId();
    int targetPin = -1;

    // 핀 번호만 매칭해줌
    switch (slideId) {
      case LED_R_SLIDE_ID: targetPin = LED_R_PIN; break;
      case LED_G_SLIDE_ID: targetPin = LED_G_PIN; break;
      case LED_B_SLIDE_ID: targetPin = LED_B_PIN; break;
    }

    if (targetPin != -1) {
      int value = linker.sliderRead(slideId);
      
      sliderStyle.setId(slideId).setText(value).build(sliderStyleStr, sizeof(sliderStyleStr));
      analogWrite(targetPin, value);
      linker.write(sliderStyleStr);
    }
  }
}

void setup() {
  // linker.begin(RX, TX, 0, 9600);// RX, TX, channel, Baud Rate
  /*
  Arduino Uno R3
  Arduino Nano
  */

  //=====================================

  // linker.begin(Serial1, 0, 9600);  // UART, channel, Baud Rate
  /*
  Arduino Mega2560(Serial1, Serial2, Serial3)
  Arduino Due(Serial1, Serial2, Serial3)
  Arduino Uno R4(Serial1)
  */

  //=====================================

  // linker.begin(18, 17, 0, 9600);// RX, TX, channel, Baud Rate (라이브러리 선언 구조에 맞춤)
  /*
  ESP32
  ESP32_S2(UART1 = 1)
  ESP32_S3(UART1 = 1, UART2 = 2)
  ESP32_C2(UART1 = 1)
  ESP32_C3(UART1 = 1)
  */

  //=====================================

  // linker.beginInternal("MyESP32");// Name, channel
  /*
  ESP32
  */

  linker.onDataReceived(callback);

  pinMode(LED_R_PIN, OUTPUT);
  pinMode(LED_G_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
}

void loop() {
  linker.update();
  /*
  [en]
  The ESP32 series automatically checks for incoming data via its internal RTOS, so the update() function is not necessary.
  However, boards such as Arduino (UNO R3, UNO R4, NANO, MEGA, DUE) must call the update() function in every loop() iteration to check for received data.
  [ko]
  ESP32 시리즈는 내부 RTOS를 통해 수신 여부를 자동으로 확인하지만, Arduino(UNO R3, UNO R4, NANO, MEGA, DUE)와 같은 보드는 매 loop() 반복마다
  update() 함수를 호출하여 수신된 데이터가 있는지 확인해야 합니다.
  */
}