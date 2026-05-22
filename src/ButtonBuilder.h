/*
 * Copyright 2026 [han-sunghyun]
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 */
 
class ButtonStyleBuilder {
private:
    int _id = 0;
    String _text = "";
    String _textColor = "";
    String _buttonColor = "";

public:
    ButtonStyleBuilder& setId(int id) {
        _id = id;
        return *this;
    }

    ButtonStyleBuilder& setText(int val) {
        _text = String(val);
        return *this;
    }

    ButtonStyleBuilder& setText(float val) {
        _text = String(val);
        return *this;
    }

    ButtonStyleBuilder& setText(char val) {
        _text = String(val);
        return *this;
    }

    ButtonStyleBuilder& setText(const String& val) {
        _text = val;
        return *this;
    }

    ButtonStyleBuilder& setTextColor(String color) {
        _textColor = color;
        return *this;
    }

    ButtonStyleBuilder& setButtonColor(String color) {
        _buttonColor = color;
        return *this;
    }

    String build() {
        String result = "B:";
        result += String(_id) + ",";
        result += _text + ",";
        result += _textColor + ",";
        result += _buttonColor + "\n";
        return result;
    }
};