/*
 * Copyright 2026 [han-sunghyun]
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 */

#include "Linker.h"

#if defined(ARDUINO_ARCH_ESP32) || defined(__ESP32__) || defined(ESP32)

void Linker::createTask() {
  // 1. 이미 테스크가 생성되어 정상 가동 중인지 철저히 체크
  if (_taskHandle != NULL) {
    eTaskState state = eTaskGetState(_taskHandle);
    if (state != eDeleted) {
      return; // 살아있는 테스크가 있다면 중복 생성 방지를 위해 탈출
    }
    _taskHandle = NULL;
  }

  // 2. 멀티코어 핀 고정형 테스크 생성 (xTaskCreatePinnedToCore)
  // 무선 통신 프로토콜 스택이 상주하는 Core 0번에 강제 할당하여 효율 극대화
  BaseType_t result = xTaskCreatePinnedToCore(
    [](void* obj) {
      while (1) {
        if (obj != nullptr) {
          static_cast<Linker*>(obj)->update();
        }
        vTaskDelay(pdMS_TO_TICKS(10)); // 워치독 방지 및 컨텍스트 스위칭 마진 확보
      }
      // [필수 보완] 만약의 사태로 루프를 탈출하더라도 커널 패닉을 막기 위해 메모리 해제 보장
      vTaskDelete(NULL); 
    },
    "LinkerTask", 
    4096,         // 문자열 파싱(String) 연산의 메모리 오버헤드를 고려해 4KB 할당 (안전)
    this, 
    1,            // 우선순위
    &_taskHandle, 
    0             // ➔ [핵심] Core 0번에 고정 (메인 시스템 루프가 도는 Core 1번 보호)
  );

  if (result != pdPASS) {
    _taskHandle = NULL;
  }
}
#endif


#if defined(LINKER_ENABLE_MULTI_HW_SERIAL)

void Linker::begin(int uart_nr, int rx, int tx, int ch, long baud) {
  if (ch >= 0 && ch < 3) {
    HardwareSerial* espSerial = new HardwareSerial(uart_nr);
    espSerial->begin(baud, SERIAL_8N1, rx, tx);
    _activeStreams[ch] = espSerial;
    _isSoftwareSerial = false;

    createTask();
  }
}

#endif


// [핵심 보정] 헤더 파일의 매크로 분기와 완벽하게 일치시킴으로써 PlatformIO 링커 에러 원천 차단!
#if defined(LINKER_SUPPORT_CLASSIC_BT)
void Linker::beginInternal(String deviceName, int ch) {
  if (ch >= 0 && ch < 3) {
    BluetoothSerial* btInternal = new BluetoothSerial();
    btInternal->begin(deviceName);
    _activeStreams[ch] = btInternal;
    _isSoftwareSerial = false; // 클래식 블루투스는 칩셋 내장 RF 코어를 사용하므로 false 처리

    createTask();
  }
}
#endif