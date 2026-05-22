/*
 * Copyright 2026 [han-sunghyun]
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 */
 
class SwitchStyleBuilder {
private:
  int _id = 0;
  int _value = -1;
  String _text = "";
  String _textColor = "";
  String _switchColor = "";

public:
  SwitchStyleBuilder& setId(int id) {
    _id = id;
    return *this;
  }

  SwitchStyleBuilder& setValue(bool val) {
    _value = val;
    return *this;
  }

  SwitchStyleBuilder& setText(int val) {
    _text = String(val);
    return *this;
  }

  SwitchStyleBuilder& setText(float val) {
    _text = String(val);
    return *this;
  }

  SwitchStyleBuilder& setText(char val) {
    _text = String(val);
    return *this;
  }

  SwitchStyleBuilder& setText(const String& val) {
    _text = val;
    return *this;
  }

  SwitchStyleBuilder& setTextColor(String color) {
    _textColor = color;
    return *this;
  }

  SwitchStyleBuilder& setSwitchColor(String color) {
    _switchColor = color;
    return *this;
  }

  String build() {
    String result = "SW:";
    result += String(_id) + ",";
    if(_value == -1) {
      result += ",";
    } else {
      result += String(_value) + ",";
    }
    result += _text + ",";
    result += _textColor + ",";
    result += _switchColor + "\n";
    return result;
  }
};