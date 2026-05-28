/*
 * Copyright 2026 [han-sunghyun]
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * https://www.apache.org/licenses/LICENSE-2.0
 */

class SwitchStyleBuilder {
private:
    int _id = 0;
    int _value = -1;
    char _text[21] = {0};
    char _textColor[7] = {0};
    char _switchColor[7] = {0};

public:
    SwitchStyleBuilder& setId(int id) {
        _id = id;
        return *this;
    }

    SwitchStyleBuilder& setValue(bool val) {
        _value = val ? 1 : 0; // bool 값을 0 또는 1로 명확히 저장
        return *this;
    }

    // 정수형 처리
    SwitchStyleBuilder& setText(int val) {
        snprintf(_text, sizeof(_text), "%d", val);
        return *this;
    }

    // 실수형 처리 (dtostrf 사용)
    SwitchStyleBuilder& setText(float val, int precision = 2) {
        dtostrf(val, 1, precision, _text);
        return *this;
    }

    // 단일 문자 처리
    SwitchStyleBuilder& setText(char val) {
        _text[0] = val;
        _text[1] = '\0';
        return *this;
    }

    // 문자열(char 배열) 처리
    SwitchStyleBuilder& setText(const char* val) {
        strncpy(_text, val, sizeof(_text) - 1);
        _text[sizeof(_text) - 1] = '\0';
        return *this;
    }

    SwitchStyleBuilder& setTextColor(const char* color) {
        strncpy(_textColor, color, sizeof(_textColor) - 1);
        _textColor[sizeof(_textColor) - 1] = '\0';
        return *this;
    }

    SwitchStyleBuilder& setSwitchColor(const char* color) {
        strncpy(_switchColor, color, sizeof(_switchColor) - 1);
        _switchColor[sizeof(_switchColor) - 1] = '\0';
        return *this;
    }

    // 결과물을 외부 버퍼에 담아주는 안전한 build 함수
    void build(char* outBuffer, size_t bufferSize) {
        char valBuffer[4] = {0};
        
        // _value가 -1이면 빈 문자열, 0이나 1이면 해당 숫자를 임시 버퍼에 생성
        if (_value != -1) {
            snprintf(valBuffer, sizeof(valBuffer), "%d", _value);
        }

        // 최종 조립 (valBuffer가 비어있으면 SW:1,,text... 형태로 자연스럽게 콤마만 찍힙니다)
        snprintf(outBuffer, bufferSize, "SW:%d,%s,%s,%s,%s\n", 
                 _id, valBuffer, _text, _textColor, _switchColor);
    }
};