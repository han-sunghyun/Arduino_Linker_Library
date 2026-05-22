/*
 * Copyright 2026 [han-sunghyun]
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 */
 
class SliderStyleBuilder {
private:
  int _id = 0;
  int _value = 0;
  String _text = "";
  String _textColor = "";
  String _sliderHaneColor = "";
  String _sliderLineColor = "";

public:
  SliderStyleBuilder& setId(int id) {
    _id = id;
    return *this;
  }

  SliderStyleBuilder& setValue(int val) {
    _value = val;
    return *this;
  }

  SliderStyleBuilder& setText(int val) {
    _text = String(val);
    return *this;
  }

  SliderStyleBuilder& setText(float val) {
    _text = String(val);
    return *this;
  }

  SliderStyleBuilder& setText(char val) {
    _text = String(val);
    return *this;
  }

  SliderStyleBuilder& setText(const String& val) {
    _text = val;
    return *this;
  }

  SliderStyleBuilder& setTextColor(String color) {
    _textColor = color;
    return *this;
  }

  SliderStyleBuilder& setSliderHandColor(String color) {
    _sliderHaneColor = color;
    return *this;
  }

  SliderStyleBuilder& setSliderLineColor(String color) {
    _sliderLineColor = color;
    return *this;
  }

  String build() {
    String result = "SL:";
    result += String(_id) + ",";
    result += String(_value) + ",";
    result += _text + ",";
    result += _textColor + ",";
    result += _sliderHaneColor + ",";
    result += _sliderLineColor + "\n";
    return result;
  }
};