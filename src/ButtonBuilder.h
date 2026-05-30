/*
 * Copyright 2026 [han-sunghyun]
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * https://www.apache.org/licenses/LICENSE-2.0
 */

class ButtonStyleBuilder {
private:
    int _id = 0;
    char _text[21] = {0};         // 텍스트를 저장할 버퍼 크기 지정
    char _textColor[7] = {0};    // #FFFFFF 같은 HEX 코드나 색상명 저장
    char _buttonColor[7] = {0};

public:
    ButtonStyleBuilder& setId(int id) {
        _id = id;
        return *this;
    }

    // 정수형 처리
    ButtonStyleBuilder& setText(int val) {
        snprintf(_text, sizeof(_text), "%d", val);
        return *this;
    }

    // 실수형 처리 (아두이노 snprintf는 %f를 지원하지 않는 경우가 많아 dtostrf 사용)
    ButtonStyleBuilder& setText(float val, int precision = 2) {
        dtostrf(val, 1, precision, _text);
        return *this;
    }

    // 단일 문자 처리
    ButtonStyleBuilder& setText(char val) {
        _text[0] = val;
        _text[1] = '\0';
        return *this;
    }

    // 문자열(char 배열) 처리
    ButtonStyleBuilder& setText(const char* val) {
        strncpy(_text, val, sizeof(_text) - 1);
        _text[sizeof(_text) - 1] = '\0'; // 안정성을 위한 널 종료 문자 보장
        return *this;
    }

    ButtonStyleBuilder& setTextColor(const char* color) {
        strncpy(_textColor, color, sizeof(_textColor) - 1);
        _textColor[sizeof(_textColor) - 1] = '\0';
        return *this;
    }

    ButtonStyleBuilder& setButtonColor(const char* color) {
        strncpy(_buttonColor, color, sizeof(_buttonColor) - 1);
        _buttonColor[sizeof(_buttonColor) - 1] = '\0';
        return *this;
    }

    // 결과물을 외부에서 제공한 버퍼에 안전하게 담아주는 방식
    void build(char* outBuffer, size_t bufferSize) {
        snprintf(outBuffer, bufferSize, "B:%d,%s,%s,%s\n", 
                 _id, _text, _textColor, _buttonColor);
    }
};