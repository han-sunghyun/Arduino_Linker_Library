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
SwitchStyleBuilder mySwitch;
SliderStyleBuilder mySlider;
/*
[en]
For properties of widgets that can be style changed in the Linker app, you can easily generate style change data strings using the object above.
*** It is recommended to create objects only globally. ***
[ko]
Linker 앱에서 스타일 변경이 가능한 위젯의 속성들은 위 객체를 사용해서 쉽게 스타일 변경데이터 문자열을 생성할 수 있습니다.
*** 객체는 전역 에서만 생성 하는것을 권장합니다. ***
*/

String64 buttonStyle;
String64 switchStyle;
String64 sliderStyle;
/*
String64 = char String64[64]

[en] Declare a variable to hold style string data generated using String64.
[ko] String64 를 사용해 생성된 스타일문자열 데이터를 담는 변수를 선언합니다.
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



  bool isButtonUpdated = linker.isButtonUpdated();
  // [en] Determines if the received data is from a button. Returns true if it is, false otherwise.
  // [ko] 수신된 데이터가 버튼에서 온 것인지 판별합니다. 버튼에서 온 데이터라면 true, 아니라면 false를 반환합니다.

  int buttonId = linker.getButtonId();
  // [en] If the received data is from a button, it returns the button's id. If not, it returns -1.
  // [ko] 수신된 데이터가 버튼에서 온 것이라면 해당 버튼의 id를 반환합니다. 아니라면 -1을 반환합니다.

  int buttonState = linker.buttonRead(buttonId);
  // [en] returns the value of the button with the specified ID (0 button up, 1 button push). Returns -1 if the data did not come from the button or if the ID does not match.
  // [ko] 특정 id를 가진 버튼의 값을 반환합니다.(0 버튼 해제, 1 버튼 누름) 데이터가 버튼에서 온 것이 아니거나 id가 일치하지 않으면 -1을 반환합니다.



  bool isSwitchUpdated = linker.isSwitchUpdated();
  // [en] Determines if the received data is from a switch. Returns true if it is, false otherwise.
  // [ko] 수신된 데이터가 스위치에서 온 것인지 판별합니다. 스위치에서 온 데이터라면 true, 아니라면 false를 반환합니다.

  int switchId = linker.getSwitchId();
  // [en] If the received data is from a switch, it returns the switch's id. If not, it returns -1.
  // [ko] 수신된 데이터가 스위치에서 온 것이라면 해당 스위치의 id를 반환합니다. 아니라면 -1을 반환합니다.

  int switchState = linker.switchRead(switchId);
  // [en] Returns the value of the switch with the specified ID. (0 switch off, 1 switch on) Returns -1 if the data did not come from the switch or if the ID does not match.
  // [ko] 특정 id를 가진 스위치의 값을 반환합니다.(0 스위치off, 1 스위치 on) 데이터가 스위치에서 온 것이 아니거나 id가 일치하지 않으면 -1을 반환합니다.



  bool isSliderUpdated = linker.isSliderUpdated();
  // [en] Determines if the received data is from a slider. Returns true if it is, false otherwise.
  // [ko] 수신된 데이터가 슬라이더에서 온 것인지 판별합니다. 슬라이더에서 온 데이터라면 true, 아니라면 false를 반환합니다.

  int sliderId = linker.getSliderId();
  // [en] If the received data is from a slider, it returns the slider's id. If not, it returns -1.
  // [ko] 수신된 데이터가 슬라이더에서 온 것이라면 해당 슬라이더의 id를 반환합니다. 아니라면 -1을 반환합니다.

  int sliderState = linker.sliderRead(sliderId);
  // [en] Returns the value of the slider with the specified ID (positive integer n ~ m). Returns -1 if the data is not from the slider or if the ID does not match.
  // [ko] 특정 id를 가진 슬라이더의 값을 반환합니다.(양의정수 n ~ m) 데이터가 슬라이더에서 온 것이 아니거나 id가 일치하지 않으면 -1을 반환합니다.
}

void setup() {

  /* ==============================================================================
  void beginInternal(String deviceName = "ESP32_Linker", int ch = 0);
  [en] Please use the above function when using Classic Bluetooth built into the ESP32.
  [ko] ESP32 에 내장되어 있는 클래식 블루투스 사용 시 위 함수를 사용하세요
  */

  // linker.beginInternal(); //.beginInternal("MyDevice"); .beginInternal("MyDevice", 0);


  /* ==============================================================================
  void begin(int rx, int tx, int ch = 0, long baud = 9600);
  [en] Use this function for Classic Bluetooth via SoftwareSerial on boards with limited hardware serial ports (Arduino UNO R3, NANO ...)
  [ko] 하드웨어 시리얼 포트가 제한적인 보드(Arduino UNO R3, NANO 등)의 경우 위 함수를 사용하세요
  */

  // linker.begin(2, 3); //.begin(2, 3, 0); .begin(2, 3, 0, 9600);


  /* ==============================================================================
  void begin(HardwareSerial& hwSerial, int ch = 0, long baud = 9600);
  [en] Use this function for Arduino boards supporting hardware serial (Arduino Mega, Arduino Due, Arduino UNO R4)
  [ko] 하드웨어 시리얼을 지원하는 아두이노 보드(Arduino Mega, Arduino Due, Arduino UNO_R4)의 경우 위 함수를 사용하세요

  Arduino Mega2560(Serial1, Serial2, Serial3)
  Arduino Due(Serial1, Serial2, Serial3)
  Arduino Uno R4(Serial1)
  */

  // linker.begin(Serial1);//.begin(Serial1, 0); .begin(Serial1, 0, 9600);


  /* ==============================================================================
  void begin(int uart_nr, int rx, int tx, int ch = 0, long baud = 9600);
  [en] Use this function for ESP32, ESP32-S2, and ESP32-S3 boards.
  [ko] ESP32, ESP32_S2, ESP32_S3, ESP32_C3, ESP32_C2 ... 보드 의 경우 위 함수를 사용하세요

  ESP32
  ESP32_S2(UART1 = 1)
  ESP32_S3(UART1 = 1, UART2 = 2)
  ESP32_C2(UART1 = 1)
  ESP32_C3(UART1 = 1)
  */

  // linker.begin(1, 6, 7); //.begin(1, 6, 7, 0); .begin(1, 6, 7, 0, 9600);


  /* ==============================================================================
  [en] Ch (0~2): Supports up to 3 concurrent connections for multiple comms.
  [ko] 채널(0~2): 여러 통신 방식을 사용 시 최대 3개 동시 연결 지원.
  
  Ex) Ch 0 = External BT Module / Ch 1 = Built-in Classic BT
  */

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
  ESP32 시리즈는 내부 RTOS를 통해 수신 여부를 자동으로 확인하지만,
  Arduino(UNO R3, UNO R4, NANO, MEGA, DUE)와 같은 보드는 매 loop() 반복마다 update() 함수를 호출하여 수신된 데이터가 있는지 확인해야 합니다.
  */


  //[ButtonBuilder]==================== 사용법1 / Usage
  myButton.setId(0);
  // [en] Sets the ID of the button to be modified.
  // [ko] 변경할 버튼의 ID 값을 인자값으로 넣어주세요.

  myButton.setText("text");  // .setText('A'); .setText(10); .setText(3.14);
  // [en] Sets the text label of the button. (Supports int, float, char, and String types.)
  // [ko] 변경할 버튼의 텍스트 값을 인자로 넣어주세요. (인자 타입은 정수, 실수, 문자, 문자열을 지원합니다.)

  myButton.setTextColor("FFFFFF");
  // [en] Sets the text color using a HEX format string.
  // [ko] 변경할 버튼의 텍스트 색상 값을 16진수 형태의 문자열로 인자에 넣어주세요.

  myButton.setButtonColor("000000");
  // [en] Sets the button background color using a HEX format string.
  // [ko] 변경할 버튼의 배경 색상 값을 16진수 형태의 문자열로 인자에 넣어주세요.

  myButton.build(buttonStyle, sizeof(buttonStyle));
  // [en] Once all settings are complete and the above function is called, the change data matching the protocol is entered into the string.
  // [ko] 모든 설정을 마치고 위 함수를 호출하면 프로토콜에 맞는 변경 데이터가 문자열에 들어갑니다.

  //[ButtonBuilder]==================== 사용법2 / Usage
  myButton.setId(0).setText("text").setTextColor("FFFFFF").setButtonColor("000000").build(buttonStyle, sizeof(buttonStyle));





  // [SwitchBuilder]==================== 사용법1 / Usage
  mySwitch.setId(0);
  // [en] Sets the ID of the switch to be modified.
  // [ko] 변경할 스위치의 ID 값을 인자값으로 넣어주세요.

  mySwitch.setValue(true);
  // [en] Sets the initial state of the switch using a boolean value (true/false).
  // [ko] 스위치의 초기 On/Off 상태를 불리언(bool) 값으로 설정합니다.

  mySwitch.setText("text");  // .setText('A'); .setText(10); .setText(3.14);
  // [en] Sets the text label of the switch. (Supports int, float, char, and String types.)
  // [ko] 변경할 스위치의 텍스트 라벨 값을 인자로 넣어주세요. (인자 타입은 정수, 실수, 문자, 문자열을 지원합니다.)

  mySwitch.setTextColor("FFFFFF");
  // [en] Sets the switch text color using a HEX format string.
  // [ko] 변경할 스위치 텍스트의 색상 값을 16진수 형태의 문자열로 인자에 넣어주세요.

  mySwitch.setSwitchColor("000000");
  // [en] Sets the main/active color of the switch using a HEX format string.
  // [ko] 변경할 스위치의 고유 색상(배경/액티브 색상 등)을 16진수 형태의 문자열로 인자에 넣어주세요.

  mySwitch.build(switchStyle, sizeof(switchStyle));
  // [en] Once all settings are complete and the above function is called, the change data matching the protocol is entered into the string.
  // [ko] 마지막으로 모든 설정을 마치고 위 함수를 호출하면 프로토콜에 맞는 변경 데이터가 문자열에 들어갑니다.

  // [SwitchBuilder]==================== 사용법2 / Usage
  mySwitch.setId(0).setValue(true).setText("text").setTextColor("FFFFFF").setSwitchColor("000000").build(switchStyle, sizeof(switchStyle));




  // [SliderBuilder]==================== 사용법1 / Usage
  mySlider.setId(0);
  // [en] Sets the ID of the slider to be modified.
  // [ko] 변경할 슬라이더의 ID 값을 인자값으로 넣어주세요.

  mySlider.setValue(0);
  // [en] Sets the initial value/position of the slider using an integer.
  // [ko] 슬라이더의 초기 위치/값(정수형)을 인자로 넣어주세요.

  mySlider.setText("text");
  // [en] Sets the text label of the slider. (Supports int, float, char, and String types.)
  // [ko] 변경할 슬라이더의 텍스트 라벨 값을 인자로 넣어주세요. (인자 타입은 정수, 실수, 문자, 문자열을 지원합니다.)

  mySlider.setTextColor("FFFFFF");
  // [en] Sets the slider text color using a HEX format string.
  // [ko] 변경할 슬라이더 텍스트의 색상 값을 16진수 형태의 문자열로 인자에 넣어주세요.

  mySlider.setSliderHandColor("000000");
  // [en] Sets the color of the slider knob (hand/thumb) using a HEX format string.
  // [ko] 슬라이더의 조절 손잡이(Thumb/Hand) 색상 값을 16진수 형태의 문자열로 인자에 넣어주세요.

  mySlider.setSliderLineColor("FFFFFF");
  // [en] Sets the color of the slider track (line) using a HEX format string.
  // [ko] 슬라이더 바(Line/Track)의 색상 값을 16진수 형태의 문자열로 인자에 넣어주세요.

  mySlider.build(sliderStyle, sizeof(sliderStyle));
  // [en] Once all settings are complete and the above function is called, the change data matching the protocol is entered into the string.
  // [ko] 마지막으로 모든 설정을 마치고 위 함수를 호출하면 프로토콜에 맞는 변경 데이터가 문자열에 들어갑니다.

  // [SliderBuilder]==================== 사용법2 / Usage
  mySlider.setId(0).setValue(0).setText("text").setTextColor("FFFFFF").setSliderHandColor("000000").setSliderLineColor("FFFFFF").build(sliderStyle, sizeof(sliderStyle));



  // linker.write(buttonStyle, 0);
  /*
  [en] Please pass the style string and channel number as arguments to the data transmission function.
  [ko] 스타일 문자열과 채널 번호를 데이터 전송 함수에 인수로 전달해 주십시오.
  */


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
