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
    //_activeStreams
    if (_activeStreams[streamCh] && _activeStreams[streamCh]->available() > 0) {
      String data = _activeStreams[streamCh]->readStringUntil('\n');
      data.trim();  // 공백 제거
      if (data.length() > 0) {
        parse(data, streamCh);
      }
    }
  }
}

void Linker::parse(String data, int streamCh) {
  // 여기에 이전에 설계한 [TYPE],[ID]:[VALUE] 파싱 로직 구현

  int commaIndex = data.indexOf(',');
  int colonIndex = data.indexOf(':');

  if (commaIndex != -1 && colonIndex != -1) {
    // 3. 데이터 분리 (substring 이용)
    type = data.substring(0, commaIndex);                     // 처음부터 ',' 전까지
    id = data.substring(commaIndex + 1, colonIndex).toInt();  // ',' 다음부터 ':' 전까지
    value = data.substring(colonIndex + 1);

    // 2. 콜백 함수 호출
    if (_onDataCallback != nullptr) {
      _onDataCallback(streamCh);
    }
  }
}

//=====================

void Linker::write(String value, int ch) {

  if(_activeStreams[ch]) _activeStreams[ch]->print(value);
}