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

Linker::Linker() {
  _activeStreams[0] = nullptr;
  _activeStreams[1] = nullptr;
  _activeStreams[2] = nullptr;
}



void Linker::update() {
  for (int streamCh = 0; streamCh < 3; streamCh++) {
    if (_activeStreams[streamCh] && _activeStreams[streamCh]->available() > 0) {
      
      char data[32] = {0}; // 여유 있게 버퍼 크기 지정 (기존 18층보다 조금 더 넉넉히)
      
      // \n(줄바꿈)을 만날 때까지 읽어 데이터 배열에 채움 (String 제거)
      // 읽어온 바이트 수를 리턴하며, 마지막에 \n은 자동으로 제외됨
      size_t length = _activeStreams[streamCh]->readBytesUntil('\n', data, sizeof(data) - 1);
      
      if (length > 0) {
        data[length] = '\0'; // 문자열의 끝을 보장하는 Null 문자 삽입
        
        // 데이터 양끝 공백 제거 (String.trim() 대체 수동 구현)
        char* trimmedData = data;
        // 앞쪽 공백 제거
        while (*trimmedData == ' ' || *trimmedData == '\r' || *trimmedData == '\t') {
          trimmedData++;
        }
        // 뒤쪽 공백 및 캐리지 리턴(\r) 제거
        size_t len = strlen(trimmedData);
        while (len > 0 && (trimmedData[len - 1] == ' ' || trimmedData[len - 1] == '\r' || trimmedData[len - 1] == '\t')) {
          trimmedData[len - 1] = '\0';
          len--;
        }

        // 공백이 제거된 문자열이 존재한다면 파싱 진행
        if (strlen(trimmedData) > 0) {
          parse(trimmedData, streamCh);
        }
      }
    }
  }
}

void Linker::parse(char* data, int streamCh) {
  // data 형식 예시: "SW,1:1" 또는 "B,12:Volume"
  
  // 1. 포인터 함수들을 이용해 ','와 ':'의 위치(주소)를 찾습니다.
  char* commaPtr = strchr(data, ',');
  char* colonPtr = strchr(data, ':');

  // ','와 ':'가 모두 존재하고, 콜론이 콤마보다 뒤에 있는 경우에만 처리
  if (commaPtr != nullptr && colonPtr != nullptr && colonPtr > commaPtr) {
    
    // 2. [TYPE] 분리 (처음부터 ',' 전까지)
    size_t typeLen = commaPtr - data;
    if (typeLen >= sizeof(type)) typeLen = sizeof(type) - 1; // 오버플로우 방지
    strncpy(type, data, typeLen);
    type[typeLen] = '\0'; // 종단 문자

    // 3. [ID] 분리 (',' 다음부터 ':' 전까지) 및 정수 변환
    // atoi 함수는 문자열 숫자를 정수로 바꾸어 줍니다.
    id = (int8_t)atoi(commaPtr + 1);

    // 4. [VALUE] 분리 (':' 다음부터 끝까지)
    strncpy(value, colonPtr + 1, sizeof(value) - 1);
    value[sizeof(value) - 1] = '\0'; // 오버플로우 방지 및 종단 문자

    // 5. 콜백 함수 호출
    if (_onDataCallback != nullptr) {
      _onDataCallback(streamCh);
    }
  }
}
//=====================

void Linker::write(const char* value, int ch) {
    if(_activeStreams[ch]) _activeStreams[ch]->print(value);
}