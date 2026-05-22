#include <Linker.h>

#define TX 2
#define RX 3

#define SLIDER_ID 0
#define PWM_LED 11

Linker linker;
ButtonStyleBuilder number;

int sliderValue = 0;

void callback(int ch) {

  if (linker.isSliderUpdated()) {

    if (linker.getSliderId() == SLIDER_ID) {

      sliderValue = linker.sliderRead(SLIDER_ID);

      analogWrite(PWM_LED, sliderValue);

    }
  }
}
void setup() {
  // linker.begin(RX, TX, 0, 9600);// RX, TX, channel, Baud Rate
  /*
  Arduino Uno R3
  Arduino Nano
  */

  //=====================================

  // linker.begin(Serial1, 0, 9600);// UART, channel, Baud Rate
  /*
  Arduino Mega2560(Serial1, Serial2, Serial3)
  Arduino Due(Serial1, Serial2, Serial3)
  Arduino Uno R4(Serial1)
  */
  

  linker.onDataReceived(callback);
}

void loop() {
  linker.update();
  /*
  [en]
  The ESP32 series automatically checks for incoming data via its internal RTOS, so the update() function is not necessary.
  However, boards such as Arduino (UNO R3, UNO R4, NANO, MEGA, DUE) must call the update() function in every loop() iteration to check for received data.
  [ko]
  ESP32 시리즈는 내부 RTOS를 통해 수신 여부를 자동으로 확인하지만, Arduino(UNO R3, UNO R4, NANO, MEGA, DUE)와 같은 보드는 매 loop() 반복마다
  update() 함수를 호출하여 수신된 데이터가 있는지 확인해야 합니다.
  */
}
