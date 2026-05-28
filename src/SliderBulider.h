/*
 * Copyright 2026 [han-sunghyun]
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * https://www.apache.org/licenses/LICENSE-2.0
 */

class SliderStyleBuilder {
private:
    int _id = 0;
    int _value = 0;
    char _text[21] = {0};
    char _textColor[7] = {0};
    char _sliderHandColor[7] = {0}; // 오타 수정 (Hane -> Hand)
    char _sliderLineColor[7] = {0};

public:
    SliderStyleBuilder& setId(int id) {
        _id = id;
        return *this;
    }

    SliderStyleBuilder& setValue(int val) {
        _value = val;
        return *this;
    }

    // 정수형 처리
    SliderStyleBuilder& setText(int val) {
        snprintf(_text, sizeof(_text), "%d", val);
        return *this;
    }

    // 실수형 처리 (dtostrf 사용)
    SliderStyleBuilder& setText(float val, int precision = 2) {
        dtostrf(val, 1, precision, _text);
        return *this;
    }

    // 단일 문자 처리
    SliderStyleBuilder& setText(char val) {
        _text[0] = val;
        _text[1] = '\0';
        return *this;
    }

    // 문자열(char 배열) 처리
    SliderStyleBuilder& setText(const char* val) {
        strncpy(_text, val, sizeof(_text) - 1);
        _text[sizeof(_text) - 1] = '\0';
        return *this;
    }

    SliderStyleBuilder& setTextColor(const char* color) {
        strncpy(_textColor, color, sizeof(_textColor) - 1);
        _textColor[sizeof(_textColor) - 1] = '\0';
        return *this;
    }

    SliderStyleBuilder& setSliderHandColor(const char* color) {
        strncpy(_sliderHandColor, color, sizeof(_sliderHandColor) - 1);
        _sliderHandColor[sizeof(_sliderHandColor) - 1] = '\0';
        return *this;
    }

    SliderStyleBuilder& setSliderLineColor(const char* color) {
        strncpy(_sliderLineColor, color, sizeof(_sliderLineColor) - 1);
        _sliderLineColor[sizeof(_sliderLineColor) - 1] = '\0';
        return *this;
    }

    // 결과물을 외부 버퍼에 담아주는 안전한 build 함수
    void build(char* outBuffer, size_t bufferSize) {
        snprintf(outBuffer, bufferSize, "SL:%d,%d,%s,%s,%s,%s\n", 
                 _id, _value, _text, _textColor, _sliderHandColor, _sliderLineColor);
    }
};