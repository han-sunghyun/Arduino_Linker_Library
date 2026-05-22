#include <Linker.h>

/*
 * Copyright 2026 [han-sunghyun]
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * */

Linker linker;

// =======================================================================
// [Global Instance Declarations / 전역 객체 선언]
// =======================================================================

ButtonStyleBuilder myButton;
/*
[en] Creates an object named myButton.
[ko] myButton 이라는 이름으로 객체가 생성됩니다.
*/

SwitchStyleBuilder mySwitch;
/*
[en] Creates an object named mySwitch.
[ko] mySwitch 라는 이름으로 객체가 생성됩니다.
*/

SliderStyleBuilder mySlider;
/*
[en] Creates an object named mySlider.
[ko] mySlider 라는 이름으로 객체가 생성됩니다.
*/

/*
[en]
For properties of widgets that can be style changed in the Linker app, you can easily generate style change data strings using the object above.
*** [Caution] Please create objects only globally whenever possible. Never create them within loops, for statements, or while statements! ***
[ko]
Linker 앱에서 스타일 변경이 가능한 위젯의 속성들은 위 객체를 사용해서 쉽게 스타일 변경데이터 문자열을 생성할 수 있습니다.
*** [주의] 객체는 전역 에서만 생성하도록 하세요. 절대 loop 또는 for, while 의 반복문 에서는 생성하지 마세요! ***
*/

String buttonStyle;
/*
[en] String variables that store style change data must be declared as global variables.
[ko] 스타일 변경 데이터를 저장하는 문자열 변수는 전역 변수로 선언해야 합니다.
*/


// =======================================================================
// [Data Received Callback / 데이터 수신 콜백]
// =======================================================================

void callback(int ch) {
  /*
  [en] This function is a callback function registered in the setup function. It is automatically called when data is received.
  [ko] 이 함수는 setup 함수에서 등록한 콜백 함수 입니다. 데이터가 수신되면 자동으로 호출됩니다.

  [en] The 'ch' parameter represents the received channel number.
  [ko] 'ch' 매개변수는 수신된 채널 번호 입니다.

  [en]
  It is recommended not to write complex or time-consuming logic in this space.
  The ESP32 series operates within a separate task utilizing an RTOS.
  [ko]
  이 공간에는 복잡하거나 시간이 많이 소요되는 로직을 작성하지 않는 것을 권장합니다.
  ESP32 시리즈는 RTOS를 사용하는 별도의 태스크 내에서 작동합니다.
  */


  /*
  bool isButtonUpdated();
  [en] Determines if the received data is from a button. Returns true if it is, false otherwise.
  [ko] 수신된 데이터가 버튼에서 온 것인지 판별합니다. 버튼에서 온 데이터라면 true, 아니라면 false를 반환합니다.

  int8_t getButtonId();
  [en] If the received data is from a button, it returns the button's id. If not, it returns -1.
  [ko] 수신된 데이터가 버튼에서 온 것이라면 해당 버튼의 id를 반환합니다. 아니라면 -1을 반환합니다.

  int buttonRead(int8_t button_id);
  [en] returns the value of the button with the specified ID (0 button up, 1 button push). Returns -1 if the data did not come from the button or if the ID does not match.
  [ko] 특정 id를 가진 버튼의 값을 반환합니다.(0 버튼 해제, 1 버튼 누름) 데이터가 버튼에서 온 것이 아니거나 id가 일치하지 않으면 -1을 반환합니다.
  */

  /*
  bool isSwitchUpdated();
  [en] Determines if the received data is from a switch. Returns true if it is, false otherwise.
  [ko] 수신된 데이터가 스위치에서 온 것인지 판별합니다. 스위치에서 온 데이터라면 true, 아니라면 false를 반환합니다.

  int8_t getSwitchId();
  [en] If the received data is from a switch, it returns the switch's id. If not, it returns -1.
  [ko] 수신된 데이터가 스위치에서 온 것이라면 해당 스위치의 id를 반환합니다. 아니라면 -1을 반환합니다.

  int switchRead(int8_t switch_id);
  [en] Returns the value of the switch with the specified ID. (0 switch off, 1 switch on) Returns -1 if the data did not come from the switch or if the ID does not match.
  [ko] 특정 id를 가진 스위치의 값을 반환합니다.(0 스위치off, 1 스위치 on) 데이터가 스위치에서 온 것이 아니거나 id가 일치하지 않으면 -1을 반환합니다.
  */

  /*
  bool isSliderUpdated();
  [en] Determines if the received data is from a slider. Returns true if it is, false otherwise.
  [ko] 수신된 데이터가 슬라이더에서 온 것인지 판별합니다. 슬라이더에서 온 데이터라면 true, 아니라면 false를 반환합니다.

  int8_t getSliderId();
  [en] If the received data is from a slider, it returns the slider's id. If not, it returns -1.
  [ko] 수신된 데이터가 슬라이더에서 온 것이라면 해당 슬라이더의 id를 반환합니다. 아니라면 -1을 반환합니다.

  int sliderRead(int8_t slider_id);
  [en] Returns the value of the slider with the specified ID (positive integer n ~ m). Returns -1 if the data is not from the slider or if the ID does not match.
  [ko] 특정 id를 가진 슬라이더의 값을 반환합니다.(양의정수 n ~ m) 데이터가 슬라이더에서 온 것이 아니거나 id가 일치하지 않으면 -1을 반환합니다.
  */

  if (ch == 0) {
    /*
    [en] Filters data to process only when received through channel 0.
    [ko] 0번 채널을 통해 수신된 데이터인 경우에만 진입합니다.
    */

    if (linker.isButtonUpdated()) {
      /*
      [en] Checks if the updated data is triggered by a button widget.
      [ko] 수신된 데이터가 버튼 위젯의 데이터가 맞는지 판별합니다.
      */

      switch (linker.getButtonId()) {
          /*
        [en] Branches the logic based on the unique ID of the operated button.
        [ko] 수신된 버튼의 고유 ID 값에 따라 각 케이스로 분기합니다.
        */

        case 0:
          {
            /*
            [en] Target: Button ID 0
            [ko] 처리 대상: 0번 ID를 가진 버튼
            */

            if (linker.buttonRead(0) == 1) {
              /*
              [en] Executed when Button 0 is pressed (Push state).
              [ko] 0번ID 버튼을 누른 순간(Push) 실행될 로직 입니다.
              */
            }
            if (linker.buttonRead(0) == 0) {
              /*
              [en] Executed when Button 0 is released (Up state).
              [ko] 0번ID 버튼에서 손을 뗀 순간(Release) 실행될 로직 입니다.
              */
            }
            break;
          }

        case 1:
          {
            /*
            [en] Target: Button ID 1
            [ko] 처리 대상: 1번 ID를 가진 버튼
            */

            if (linker.buttonRead(1) == 1) {
              /*
              [en] Executed when Button 1 is pressed (Push state).
              [ko] 1번ID 버튼을 누른 순간(Push) 실행될 로직 입니다.
              */
            }
            if (linker.buttonRead(1) == 0) {
              /*
              [en] Executed when Button 1 is released (Up state).
              [ko] 1번ID 버튼에서 손을 뗀 순간(Release) 실행될 로직 입니다.
              */
            }
            break;
          }
      }
    }
  }
}

// =======================================================================
// [Setup & Loop / 초기화 및 루프]
// =======================================================================

void setup() {
  // void beginInternal(String deviceName = "ESP32_Linker", int ch = 0);
  // linker.beginInternal();
  // linker.beginInternal("MyDevice");
  // linker.beginInternal("MyDevice", 0);
  /*
  [en] Please use the above function when using Classic Bluetooth built into the ESP32.
  [ko] ESP32 에 내장되어 있는 클래식 블루투스 사용 시 위 함수를 사용하세요
  */

  //==============================================================================

  // void begin(int rx, int tx, int ch = 0, long baud = 9600);
  // linker.begin(2, 3);
  // linker.begin(2, 3, 0);
  // linker.begin(2, 3, 0, 9600);
  /*
  [en] Use this function for Classic Bluetooth via SoftwareSerial on boards with limited hardware serial ports (Arduino UNO R3, NANO ...)
  [ko] 하드웨어 시리얼 포트가 제한적인 보드(Arduino UNO R3, NANO 등)의 경우 위 함수를 사용하세요
  */

  //==============================================================================

  // void begin(HardwareSerial& hwSerial, int ch = 0, long baud = 9600);
  // linker.begin(Serial1);
  // linker.begin(Serial1, 0);
  // linker.begin(Serial1, 0, 9600);
  /*
  [en] Use this function for Arduino boards supporting hardware serial (Arduino Mega, Arduino Due, Arduino UNO R4)
  [ko] 하드웨어 시리얼을 지원하는 아두이노 보드(Arduino Mega, Arduino Due, Arduino UNO_R4)의 경우 위 함수를 사용하세요

  Arduino Mega2560(Serial1, Serial2, Serial3)
  Arduino Due(Serial1, Serial2, Serial3)
  Arduino Uno R4(Serial1)
  */

  //==============================================================================

  // void begin(int uart_nr, int rx, int tx, int ch = 0, long baud = 9600);
  // linker.begin(1, 6, 7);
  // linker.begin(1, 6, 7, 0);
  // linker.begin(1, 6, 7, 0, 9600);

  /*
  [en] Use this function for ESP32, ESP32-S2, and ESP32-S3 boards.
  [ko] ESP32, ESP32_S2, ESP32_S3, ESP32_C3, ESP32_C2 ... 보드 의 경우 위 함수를 사용하세요

  ESP32
  ESP32_S2(UART1 = 1)
  ESP32_S3(UART1 = 1, UART2 = 2)
  ESP32_C2(UART1 = 1)
  ESP32_C3(UART1 = 1)
  */

  //==============================================================================

  /*
  [en] Ch (0~2): Supports up to 3 concurrent connections for multiple comms.
  [ko] 채널(0~2): 여러 통신 방식을 사용 시 최대 3개 동시 연결 지원.
  
  Ex) Ch 0 = External BT Module / Ch 1 = Built-in Classic BT
  */

  // linker.beginInternal("MyDevice", 0); //ESP32
  // linker.begin(2, 3, 0, 9600); //Arduino Uno R3, Arduino Nano ...
  // linker.begin(Serial1, 0, 9600); //Arduino Mega2560, Arduino Due, Arduino Uno R4 ...
  // linker.begin(1, 6, 7, 0, 9600); //ESP32, ESP32 S2, ESP32 S3 ...

  linker.onDataReceived(callback);
  /*
  [en] A callback function is executed when data is received.
  [ko] 데이터가 수신되면 콜백 함수가 실행됩니다.
  */
}

void loop() {
  linker.update();
  /*
  [en]
  The ESP32 series automatically checks for reception via its internal RTOS, so the update() function is not necessary.
  However, boards such as Arduino (UNO R3, UNO R4, NANO, MEGA, DUE) must call the update() function in every loop() iteration to check for received data.
  [ko]
  ESP32 시리즈는 내부 RTOS를 통해 수신 여부를 자동으로 확인하지만, Arduino(UNO R3, UNO R4, NANO, MEGA, DUE)와 같은 보드는 매 loop() 반복마다
  update() 함수를 호출하여 수신된 데이터가 있는지 확인해야 합니다.
  */

  // =======================================================================
  // [Widget Builders Guide / 위젯 빌더 사용법]
  // =======================================================================

  /*
[ButtonBuilder] 사용법 / Usage

함수 종류 / Methods
setId(0) 
[en] Sets the ID of the button to be modified.
[ko] 변경할 버튼의 ID 값을 인자값으로 넣어주세요.

setText("text") or setText('A') or setText(10) or setText(3.14)
[en] Sets the text label of the button. (Supports int, float, char, and String types.)
[ko] 변경할 버튼의 텍스트 값을 인자로 넣어주세요. (인자 타입은 정수, 실수, 문자, 문자열을 지원합니다.)

setTextColor("FFFFFF") 
[en] Sets the text color using a HEX format string.
[ko] 변경할 버튼의 텍스트 색상 값을 16진수 형태의 문자열로 인자에 넣어주세요.

setButtonColor("000000")
[en] Sets the button background color using a HEX format string.
[ko] 변경할 버튼의 배경 색상 값을 16진수 형태의 문자열로 인자에 넣어주세요.

build()
[en] Generates and returns the final configuration protocol string based on the current settings.
[ko] 마지막으로 모든 설정을 마치고 위 함수를 호출하면 프로토콜에 맞는 변경 데이터 문자열을 반환합니다.
*/

  /*
[SwitchBuilder] 사용법 / Usage

함수 종류 / Methods
setId(0) 
[en] Sets the ID of the switch to be modified.
[ko] 변경할 스위치의 ID 값을 인자값으로 넣어주세요.

setValue(true) or setValue(false)
[en] Sets the initial state of the switch using a boolean value (true/false).
[ko] 스위치의 초기 On/Off 상태를 불리언(bool) 값으로 설정합니다.

setText("text") or setText('A') or setText(10) or setText(3.14)
[en] Sets the text label of the switch. (Supports int, float, char, and String types.)
[ko] 변경할 스위치의 텍스트 라벨 값을 인자로 넣어주세요. (인자 타입은 정수, 실수, 문자, 문자열을 지원합니다.)

setTextColor("FFFFFF") 
[en] Sets the switch text color using a HEX format string.
[ko] 변경할 스위치 텍스트의 색상 값을 16진수 형태의 문자열로 인자에 넣어주세요.

setSwitchColor("000000")
[en] Sets the main/active color of the switch using a HEX format string.
[ko] 변경할 스위치의 고유 색상(배경/액티브 색상 등)을 16진수 형태의 문자열로 인자에 넣어주세요.

build()
[en] Generates and returns the final configuration protocol string based on the current settings.
[ko] 마지막으로 모든 설정을 마치고 위 함수를 호출하면 프로토콜에 맞는 변경 데이터 문자열을 반환합니다.
*/

  /*
[SliderBuilder] 사용법 / Usage

함수 종류 / Methods
setId(0) 
[en] Sets the ID of the slider to be modified.
[ko] 변경할 슬라이더의 ID 값을 인자값으로 넣어주세요.

setValue(50)
[en] Sets the initial value/position of the slider using an integer.
[ko] 슬라이더의 초기 위치/값(정수형)을 인자로 넣어주세요.

setText("text") or setText('A') or setText(10) or setText(3.14)
[en] Sets the text label of the slider. (Supports int, float, char, and String types.)
[ko] 변경할 슬라이더의 텍스트 라벨 값을 인자로 넣어주세요. (인자 타입은 정수, 실수, 문자, 문자열을 지원합니다.)

setTextColor("FFFFFF") 
[en] Sets the slider text color using a HEX format string.
[ko] 변경할 슬라이더 텍스트의 색상 값을 16진수 형태의 문자열로 인자에 넣어주세요.

setSliderHandColor("000000")
[en] Sets the color of the slider knob (hand/thumb) using a HEX format string.
[ko] 슬라이더의 조절 손잡이(Thumb/Hand) 색상 값을 16진수 형태의 문자열로 인자에 넣어주세요.

setSliderLineColor("FFFFFF")
[en] Sets the color of the slider track (line) using a HEX format string.
[ko] 슬라이더 바(Line/Track)의 색상 값을 16진수 형태의 문자열로 인자에 넣어주세요.

build()
[en] Generates and returns the final configuration protocol string based on the current settings.
[ko] 마지막으로 모든 설정을 마치고 위 함수를 호출하면 프로토콜에 맞는 변경 데이터 문자열을 반환합니다.
*/

  // 방법 1 (Method 1: Method Chaining)
  buttonStyle = myButton.setId(1).setText(10).setTextColor("000000").setButtonColor("9D4EDD").build();

  buttonStyle = myButton.setId(1).setText(10).build();
  // [ko] 동일한 객체를 사용하는 경우 텍스트 색상 값과 버튼 색상 값은 위에서 설정한 값 그대로 유지됩니다.
  // [en] If the same object is used, the text color and button color values will remain exactly as set above.

  // 방법 2 (Method 2: Line-by-Line)
  myButton.setId(1);
  myButton.setText(10);
  myButton.setTextColor("000000");
  buttonStyle = myButton.build();

  // =======================================================================
  // [Data Transmission Example / 데이터 전송 예시]
  // =======================================================================

  linker.write(buttonStyle, 0);
  delay(100);

  // =======================================================================
  // [Transmission Interval Guide / 데이터 전송 간격 주의사항]
  // =======================================================================

  /*
  [en]
  Transmitting data too rapidly and continuously is highly discouraged.
  If the rate of calling the transmission function exceeds the Bluetooth transfer speed, data will accumulate in the Bluetooth buffer, leading to potential data loss. 
  Furthermore, if the reception rate surpasses the Linker application's UI update speed, data will pile up in the app's buffer as well, causing data loss and rendering issues. 
  Therefore, it is strongly recommended to maintain an interval of at least 10ms (0.01 seconds) between transmissions.

  [ko]
  데이터 너무 빠르게, 그리고 지속적으로 전송하는 것은 권장하지 않습니다.
  전송 함수를 호출하는 속도가 블루투스 전송 속도보다 빠르면 블루투스 버퍼에 데이터가 계속 누적되어 데이터 손실이 발생할 수 있습니다. 
  또한, 수신 속도가 Linker 애플리케이션의 UI 업데이트 속도보다 빠르면 애플리케이션의 버퍼에도 데이터가 누적되어 
  데이터 손실이 발생하고 UI 업데이트가 제대로 수행되지 않을 수 있습니다. 
  따라서 데이터를 전송할 때는 최소 10ms(0.01초) 이상의 간격을 두는 것을 권장합니다.
  */
}
