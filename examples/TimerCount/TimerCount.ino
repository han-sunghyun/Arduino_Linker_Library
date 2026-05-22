#include <Linker.h>

#define TX 2
#define RX 3

#define UP_BUTTON_ID 0
#define DOWN_BUTTON_ID 1
#define START_BUTTON_ID 2
#define COUNT_TEXT_ID 3

Linker linker;

ButtonStyleBuilder countText;
String countTextStyle;

ButtonStyleBuilder startButton;
String startButtonStyle;

int count = 0;
bool start = false;

long long timer = 0;

void callback(int ch) {

  if (linker.isButtonUpdated()) {

    switch (linker.getButtonId()) {

      case UP_BUTTON_ID:
        {
          if (linker.buttonRead(UP_BUTTON_ID) == 0) {

            if (count < 100) count++;
            countTextStyle = countText.setText(count).setTextColor("000000").build();
            linker.write(countTextStyle);
          }
          break;
        }
      case DOWN_BUTTON_ID:
        {
          if (linker.buttonRead(DOWN_BUTTON_ID) == 0) {

            if (count > 0) count--;
            countTextStyle = countText.setText(count).setTextColor("000000").build();
            linker.write(countTextStyle);
          }
          break;
        }
      case START_BUTTON_ID:
        {
          if (linker.buttonRead(START_BUTTON_ID) == 0) {

            start = !start;

            if (start) {
              startButtonStyle = startButton.setId(START_BUTTON_ID).setText("stop").build();
            } else {
              startButtonStyle = startButton.setId(START_BUTTON_ID).setText("start").build();
            }

            linker.write(startButtonStyle);
          }
          break;
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

  countText.setId(COUNT_TEXT_ID).setTextColor("000000");

  timer = millis();
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

  if (start) {
    if (1000 <= millis() - timer) {

      timer = millis();
      count--;

      if (count <= 0) {
        count = 0;
        start = false;

        startButtonStyle = startButton.setId(START_BUTTON_ID).setText("start").build();
        countTextStyle = countText.setText(count).setTextColor("000000").build();

        linker.write(countTextStyle);
        linker.write(startButtonStyle);

        return;
      }

      if (count <= 5) {
        countTextStyle = countText.setText(count).setTextColor("FF0000").build();
      } else {
        countTextStyle = countText.setText(count).setTextColor("000000").build();
      }

      linker.write(countTextStyle);
    }
  }
}
