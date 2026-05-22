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


//======SWITCH

bool Linker::isButtonUpdated() {
  return type == "B";
}

int8_t Linker::getButtonId() {  //수신된 데이터가 버튼인지 판별 후 아니라면 -1, 맞다면 id 값 반환
  if (!isButtonUpdated()) return -1;
  return id;
}

int Linker::buttonRead(int8_t button_id) {
  if (!isButtonUpdated() || id != button_id) return -1;
  return value.toInt();
}

//======SWITCH

bool Linker::isSwitchUpdated() {
  return type == "SW";
}

int8_t Linker::getSwitchId() {  //수신된 데이터가 스위치인지 판별 후 아니라면 -1, 맞다면 id 값 반환
  if (!isSwitchUpdated()) return -1;
  return id;
}

int Linker::switchRead(int8_t switch_id) {
  if (!isSwitchUpdated() || id != switch_id) return -1;
  return value.toInt();
}

//=====SLIDER

bool Linker::isSliderUpdated() {
  return type == "SL";
}
int8_t Linker::getSliderId() {
  if (!isSliderUpdated()) return -1;
  return id;
}
int Linker::sliderRead(int8_t slider_id) {
  if (!isSliderUpdated() || id != slider_id) return -1;
  return value.toInt();
}
