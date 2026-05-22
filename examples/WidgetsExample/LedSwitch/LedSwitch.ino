#include <Linker.h>

#define LED_SWITCH_ID 0

Linker linker;

SwitchStyleBuilder ledSwitch;
String ledSwitchStyle;

void callback(int ch) {

  if (linker.isSwitchUpdated()) {

    if (linker.getSwitchId() == LED_SWITCH_ID) {

      int switchState = linker.switchRead(LED_SWITCH_ID);

      digitalWrite(LED_BUILTIN, switchState);

      if (switchState) {

        ledSwitchStyle = ledSwitch.setId(LED_SWITCH_ID).setValue(true).setSwitchColor("C2C2C2").build();
        linker.write(ledSwitchStyle);

      } else {

        ledSwitchStyle = ledSwitch.setId(LED_SWITCH_ID).setValue(false).setSwitchColor("3D3D3D").build();
        linker.write(ledSwitchStyle);
        
      }
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

  // linker.begin(Serial1, 0, 9600);  // UART, channel, Baud Rate
  /*
  Arduino Mega2560(Serial1, Serial2, Serial3)
  Arduino Due(Serial1, Serial2, Serial3)
  Arduino Uno R4(Serial1)
  */

  //=====================================

  // linker.begin(1, 18, 17, 0, 9600);// UART, RX, TX, channel, Baud Rate
  /*
  ESP32
  ESP32_S2(UART1 = 1)
  ESP32_S3(UART1 = 1, UART2 = 2)
  ESP32_C2(UART1 = 1)
  ESP32_C3(UART1 = 1)
  */

  //=====================================

  // linker.beginInternal("MyESP32");// Name, channel
  /*
  ESP32
  */

  linker.onDataReceived(callback);

  pinMode(LED_BUILTIN, OUTPUT);
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
