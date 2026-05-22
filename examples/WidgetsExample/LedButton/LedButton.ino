#include <Linker.h>

#define TX 2
#define RX 3

#define LED_BUTTON_ID 0

Linker linker;

ButtonStyleBuilder ledButton;

String ledButtonStyle;

void callback(int ch) {

  if (linker.isButtonUpdated()) {

    if (linker.getButtonId() == LED_BUTTON_ID) {

      if (linker.buttonRead(LED_BUTTON_ID) == 0) {

        digitalWrite(LED_BUILTIN, HIGH);

        ledButtonStyle = ledButton.setId(LED_BUTTON_ID).setText("ON").setButtonColor("C2C2C2").build();
        linker.write(ledButtonStyle);

      } else {

        digitalWrite(LED_BUILTIN, LOW);

        ledButtonStyle = ledButton.setId(LED_BUTTON_ID).setText("OFF").setButtonColor("3D3D3D").build();
        linker.write(ledButtonStyle);

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

  // linker.begin(Serial1, 0, 9600);// UART, channel, Baud Rate
  /*
  Arduino Mega2560(Serial1, Serial2, Serial3)
  Arduino Due(Serial1, Serial2, Serial3)
  Arduino Uno R4(Serial1)
  */

  //=====================================

  // linker.begin(18, 17, 0, 9600);// RX, TX, channel, Baud Rate (라이브러리 선언 구조에 맞춤)
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