/*
 * Copyright 2026 [han-sunghyun]
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef Linker_h
#define Linker_h
#include <Arduino.h>

#include "ButtonBuilder.h"
#include "SwitchBuilder.h"
#include "SliderBulider.h"

typedef char String64[64];

// 1. SoftwareSerial 사용 여부 판별
#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_RENESAS)
#define LINKER_ENABLE_SOFTWARE_SERIAL
#endif

#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_ARCH_RENESAS)
#define LINKER_ENABLE_HW_SERIAL
#endif

// 2. 추가 하드웨어 시리얼(Serial1, 2...) 존재 여부
#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP32S2) || defined(ESP32) || defined(__ESP32__) || defined(CONFIG_IDF_TARGET_ESP32S2)
  #define LINKER_ENABLE_MULTI_HW_SERIAL
#endif

// 3. 클래식 블루투스 판별 안정화 (PlatformIO 완벽 대응)
#if defined(ARDUINO_ARCH_ESP32) || defined(ESP32) || defined(__ESP32__)
  // 클래식 BT가 없는 파생 칩셋 매크로들을 철저히 필터링
  #if !defined(ARDUINO_ESP32_S2) && !defined(ARDUINO_ESP32_S3) && !defined(ARDUINO_ESP32_C3) && !defined(ARDUINO_ESP32_C6) && !defined(CONFIG_IDF_TARGET_ESP32S2) && !defined(CONFIG_IDF_TARGET_ESP32S3) && !defined(CONFIG_IDF_TARGET_ESP32C3) && !defined(CONFIG_IDF_TARGET_ESP32C6)
    #define LINKER_SUPPORT_CLASSIC_BT
  #endif
#endif

// 4. BLE 지원 여부 유연화
#if defined(ARDUINO_ARCH_ESP32) || defined(__ESP32__) || defined(ESP32)
  #if defined(CONFIG_BT_NIMBLE_ENABLED) || defined(ARDUINO_ESP32_S3) || defined(ARDUINO_ESP32_C3)
    #define LINKER_SUPPORT_BLE
  #endif
#endif

#if defined(LINKER_SUPPORT_CLASSIC_BT)
  #include <BluetoothSerial.h>
#endif

#if defined(LINKER_ENABLE_MULTI_HW_SERIAL)
  #include <freertos/FreeRTOS.h>
  #include <freertos/task.h>
#endif

class Linker {
private:
  Stream* _activeStreams[3] = { nullptr, nullptr, nullptr };
  bool _isSoftwareSerial = false;

#if defined(LINKER_ENABLE_MULTI_HW_SERIAL)
  TaskHandle_t _taskHandle = NULL;
#endif

  char type[5] = "";
  int8_t id = 0;
  char value[11] = "";

  void (*_onDataCallback)(int) = nullptr;

  void createTask();
  void parse(char* data, int streamCh);

public:
  Linker();

  //============AVR SETUP
#if defined(LINKER_ENABLE_SOFTWARE_SERIAL)
  void begin(int rx, int tx, int ch = 0, long baud = 9600); 
#endif

#if defined(LINKER_ENABLE_HW_SERIAL)
  void begin(HardwareSerial& hwSerial, int ch = 0, long baud = 9600); 
#endif

  //============ESP SETUP
public:
#if defined(LINKER_ENABLE_MULTI_HW_SERIAL)
  void begin(int uart_nr, int rx, int tx, int ch = 0, long baud = 9600);
#endif

// 선언부도 알맹이 매크로인 LINKER_SUPPORT_CLASSIC_BT와 일치시켜 링커오류 원천 차단
#if defined(LINKER_SUPPORT_CLASSIC_BT)
  void beginInternal(String deviceName = "ESP32_Linker", int ch = 0);
#endif

public:
  void update();

  void onDataReceived(void (*func)(int)) {
    _onDataCallback = func;
  }

public:
  bool isButtonUpdated();
  int8_t getButtonId();
  int buttonRead(int8_t button_id);

  bool isSwitchUpdated();
  int8_t getSwitchId();
  int switchRead(int8_t switch_id);

  bool isSliderUpdated();
  int8_t getSliderId();
  int sliderRead(int8_t slider_id);

public:
  void write(const char* value, int ch = 0);
};

#endif