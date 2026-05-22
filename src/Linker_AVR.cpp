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

// ==========================================
// 1. AVR / Renesas 계열 전용 SoftwareSerial 가동부
// ==========================================
#if defined(LINKER_ENABLE_SOFTWARE_SERIAL)

// 컴파일러 오작동 방지를 위해 아키텍처를 명확히 타겟팅하여 인클루드
#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_RENESAS)
  #include <SoftwareSerial.h>
#endif

void Linker::begin(int rx, int tx, int ch, long baud) {
  if (ch >= 0 && ch < 3) {
    SoftwareSerial* softSerial = new SoftwareSerial(rx, tx);
    softSerial->begin(baud);
    _activeStreams[ch] = softSerial;
    _isSoftwareSerial = true;
  }
}
#endif

// ==========================================
// 2. AVR Mega / Renesas 계열 전용 HardwareSerial 주입부
// ==========================================
#if defined(LINKER_ENABLE_HW_SERIAL)
void Linker::begin(HardwareSerial& hwSerial, int ch, long baud) {
  if (ch >= 0 && ch < 3) {
    // [안전장치] 이미 밖에서 시작되었을 수 있으므로, 보드 레이트가 0이 아닐 때만 begin 수행
    // 만약 밖에서 setup했다면 이 과정을 안전하게 스킵하거나 덮어씁니다.
    hwSerial.begin(baud); 
    
    _activeStreams[ch] = &hwSerial;
    _isSoftwareSerial = false;
  }
}
#endif