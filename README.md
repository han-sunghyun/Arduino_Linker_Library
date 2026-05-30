# Linker Library for Arduino & ESP32

A powerful embedded library for multi-channel communication and dynamic widget styling with the Linker App.  
Linker 앱과의 다중 통신 채널 연결 및 동적 위젯 스타일링을 위한 임베디드 라이브러리입니다.


## Supported MCUs and Hardware Interfaces

The Linker library supports a total of four types of initialization methods depending on the hardware architecture, and can connect up to three independent communication channels (`Ch 0 ~ 2`) simultaneously.

### 1. Hardware-specific optimized initialization functions

| Initialization function | MCU | Function Description |
| :--- | :--- | :--- |
| **`.beginInternal(String deviceName, int ch)`** | **ESP32 | Enables the ESP32 built-in Classic Bluetooth. |
| **`.begin(int rx, int tx, int ch, long baud)`** | **(Arduino UNO R3, NANO ...) | Communicates with an external Bluetooth module via software serial. |
| **`.begin(HardwareSerial& hwSerial, int ch, long baud)`** | **(Arduino MEGA 2560, Due, UNO R4 ...) | Communicate by assigning hardware serial ports (`Serial1`, `Serial2` ...). |
| **`.begin(int uart_nr, int rx, int tx, int ch, long baud)`** | **(ESP32, S2, S3, C2, C3 ...) | Communicate by directly defining the hardware serial pin mapping. |

### 2. Reception Mechanism Differences and Notes

* **`.update()`**

    * **AVR Series (UNO, NANO, etc.):** Since it is not hardware interrupt-based, this function must be manually called on every `loop()` iteration to poll the receive buffer.

    * **ESP32 Series:** Since reception is automatically scheduled within an internal FreeRTOS background task, `update()` can be omitted or its call overhead can be reduced.

---

## Data Reception and Callback Functions

When data is received from the Linker app, the registered callback function is called. Identify the received channel ID (`ch`) to check the received data.

> **Callback Writing Note (RTOS):** In the ESP32 architecture, callback functions are executed in a separate RTOS task context independent of the main loop. Therefore, avoid calling `delay()` or performing heavy computations inside the callback, and keep the code lightweight for quick return.

### Callback Registration (Example)
```
Linker linker;

void callback(int ch) {

}

void setup(){
    linker.onDataReceived(callback);
}
```


### Widget-Specific Data Reception Check Functions

### 1. `Button`

| Function | Return Type | Description |
|--|--|--|
| `.isButtonUpdated()` | bool | Returns true if the received data is a button, otherwise returns false |
| `.getButtonId()` | int8_t | Determines whether the received data is a button and returns the ID value, otherwise returns -1 |
| `.buttonRead(int8_t)` | int | Returns the received value of the button data for the specified ID (pressed = 1, released = 0, no data = -1) |

### 1. `Switch`

| Function | Return Type | Description |
|--|--|--|
| `.isSwitchUpdated()` | bool | Returns true if the received data is a switch, otherwise returns false |
| `.getSwitchId()` | int8_t | Determines whether the received data is a switch and returns the ID value, otherwise returns -1 |
| `.switchRead(int8_t)` | int | Returns the received value of the switch data for the specified ID (on = 1, off = 0, no data = -1) |

### 1. `Slider`

| Function | Return Type | Description |
|--|--|--|
| `.isSliderUpdated()` | bool | Returns true if the received data is a slider, otherwise returns false |
| `.getSliderId()` | int8_t | Determines whether the received data is a slider and returns the ID value, otherwise returns -1 |
| `.sliderRead(int8_t)` | int | Returns the received value of the slider data for the specified ID (n ~ m, no data = -1) |

---

## Dynamic UI Style Builder

Description of builder objects that allow real-time control of text, color, and layout properties of widgets inside the Linker app from the hardware side.

> **Instance Declaration Note:** It is recommended to store string data generated through style builder objects (`ButtonStyleBuilder`, `SwitchStyleBuilder`, `SliderStyleBuilder`) using the `String64` type.
>
> **State Retention Characteristic:** When reusing the same builder object and calling `build()`, existing properties (color, text, etc.) that are not explicitly changed retain their previous state (State retention).

`String64` = char String64[64]

### 1. `ButtonStyleBuilder`

| Function | Description |
|---|--|
| **`.setId(int id)`** | Specify target button widget ID |
| **`.setText(int val)`** | Button label text (integer) |
| **`.setText(float val, int precision = 2)`**| Button label text (float, decimal precision) |
| **`.setText(char val)`** | Button label text (character) |
| **`.setText(const char* val)`** | Button label text (string) |
| **`.setTextColor(const char* color)`** | Button label text color (6-digit HEX format string) |
| **`.setButtonColor(const char* color)`** | Button color (6-digit HEX format string) |
| **`.build(char* outBuffer, size_t bufferSize)`** | Final style builder string address and length |

---

### 2. `SwitchStyleBuilder`

| Function | Description |
|---|--|
| **`.setId(int id)`** | Specify target switch widget ID |
| **`.setValue(bool val)`**| Switch on/off state |
| **`.setText(int val)`** | Switch label text (integer) |
| **`.setText(float val, int precision = 2)`**| Switch label text (float, decimal precision) |
| **`.setText(char val)`** | Switch label text (character) |
| **`.setText(const char* val)`** | Switch label text (string) |
| **`.setTextColor(const char* color)`** | Switch label text color (6-digit HEX format string) |
| **`.setSwitchColor(const char* color)`** | Switch color (6-digit HEX format string) |
| **`.build(char* outBuffer, size_t bufferSize)`** | Final style builder string address and length |

---

### 3. `SliderStyleBuilder`

| Function | Description |
|---|--|
| **`.setId(int id)`** | Specify target slider widget ID |
| **`.setValue(int val)`**| Slider knob position state |
| **`.setText(int val)`** | Slider label text (integer) |
| **`.setText(float val, int precision = 2)`**| Slider label text (float, decimal precision) |
| **`.setText(char val)`** | Slider label text (character) |
| **`.setText(const char* val)`** | Slider label text (string) |
| **`.setTextColor(const char* color)`** | Slider label text color (6-digit HEX format string) |
| **`.setSliderHandColor(const char* color)`** | Slider handle (Thumb/Knob) color (6-digit HEX format string) |
| **`.setSliderLineColor(const char* color)`** | Slider track bar (Track/Line) color (6-digit HEX format string) |
| **`.build(char* outBuffer, size_t bufferSize)`** | Final style builder string address and length |

---

## Data Transmission and Traffic Control

| Function | Description |
|--|--|
| **`write(const char* value, int ch)`** | Specify target slider widget unique ID |

* Style data generated through the builder is transmitted to the target channel (`ch`) to update the app screen.

### Data Transmission Interval Control Guidelines

Continuous transmission (Flood) of raw hardware data without filtering is strictly restricted.

1. **Bandwidth Limitation:** If the hardware transmission speed is faster than the physical Bluetooth data transmission limit, data loss may occur due to congestion in the chipset's internal transmit buffer.

2. **Rendering Bottleneck:** If the receiving speed exceeds the processing speed of the Linker mobile application's UI thread and graphics renderer, packets may accumulate inside the app, increasing latency and causing UI rendering frame drops.

3. **Recommended Specification:** A **minimum hardware time delay (Interval) of 10 ms (0.01 seconds)** should be guaranteed between packet transmission function calls to ensure physical bandwidth stability and rendering stability.

---

## 🇰🇷 한국어 가이드

---

## 지원 MCU 및 하드웨어 인터페이스

Linker 라이브러리는 하드웨어 아키텍처에 따라 총 4가지 형태의 초기화(Initialization) 방식을 지원하며, 최대 3개의 독립된 통신 채널(`Ch 0 ~ 2`)을 동시에 연결할 수 있습니다.

### 1. 하드웨어별 최적화 초기화 함수

| 초기화 함수 | 지원 MCU | 기능 설명 |
| :--- | :--- | :--- |
| **`.beginInternal(String deviceName, int ch)`** | **ESP32 | ESP32내장 클래식 블루투스 를 활성화합니다. |
| **`.begin(int rx, int tx, int ch, long baud)`** | **(Arduino UNO R3, NANO 등) | 소프트웨어 시리얼을 통해 외부 블루투스 모듈과 통신합니다. |
| **`.begin(HardwareSerial& hwSerial, int ch, long baud)`** | **(Arduino MEGA 2560, Due, UNO R4 등) | 하드웨어 시리얼 포트(`Serial1`, `Serial2` 등)를 할당하여 통신합니다. |
| **`.begin(int uart_nr, int rx, int tx, int ch, long baud)`** | **(ESP32, S2, S3, C2, C3 등) | 하드웨어 시리얼 핀 매핑을 직접 정의하여 통신합니다. |

### 2. 수신 메커니즘 차이 및 주의사항
*   **`.update()`**
    *   **AVR 계열 (UNO, NANO 등):** 하드웨어 인터럽트 기반이 아니므로 매 `loop()` 반복마다 이 함수를 수동 호출하여 수신 버퍼를 폴링해야 합니다.
    *   **ESP32 계열:** 내부 프리 RTOS(FreeRTOS)의 백그라운드 태스크 내부에서 수신 여부를 자동 스케줄링하므로 `update()`를 생략하거나 호출 오버헤드를 낮출 수 있습니다.

---

## 데이터 수신 및 콜백함수

Linker 앱으로부터 데이터가 수신되면 등록된 콜백함수가 호출됩니다. 수신 채널 ID(`ch`)를 식별하여 수신된 데이터를 확인합니다.

> **콜백 작성 주의사항 (RTOS):** ESP32 아키텍처에서는 콜백 함수가 메인 루프와 독립된 별도의 RTOS 태스크 콘텍스트 내에서 실행됩니다. 따라서 콜백 내부에서는 `delay()` 호출이나 무거운 연산을 지양하고 빠른 리턴이 가능하도록 코드를 가볍게 유지해야 합니다.


### 콜백 등록(예시)

```
Linker linker;

void callback(int ch) {

}

void setup(){
    linker.onDataReceived(callback);
}
```


### 위젯별 데이터 수신확인 함수

### 1. `버튼`
| 함수 | 반환타입 | 설명 |
|--|--|--|
| `.isButtonUpdated()` | bool | 수신된 데이터가 버튼이라면 true 아니라면 false 반환 |
| `.getButtonId()` | int8_t | 수신된 데이터가 버튼인지 판별 후 id 값을 반환 아닌경우 -1 을 반환 |
| `.buttonRead(int8_t)` | int | 가져올 버튼 데이터의 id를 인자로 넘겨주면 수신된 값을 반환(누름 = 1, 땜 = 0, 데이터 없음 = -1) |

### 1. `스위치`
| 함수 | 반환타입 | 설명 |
|--|--|--|
| `.isSwitchUpdated()` | bool | 수신된 데이터가 스위치라면 true 아니라면 false 반환 |
| `.getSwitchId()` | int8_t | 수신된 데이터가 스위치인지 판별 후 id 값을 반환 아닌경우 -1 을 반환 |
| `.switchRead(int8_t)` | int | 가져올 스위치 데이터의 id를 인자로 넘겨주면 수신된 값을 반환(on = 1, off = 0, 데이터 없음 = -1) |

### 1. `슬라이더`
| 함수 | 반환타입 | 설명 |
|--|--|--|
| `.isSliderUpdated()` | bool | 수신된 데이터가 슬라이더라면 true 아니라면 false 반환 |
| `.getSliderId()` | int8_t | 수신된 데이터가 슬라이더인지 판별 후 id 값을 반환 아닌경우 -1 을 반환 |
| `.sliderRead(int8_t)` | int | 가져올 슬라이더 데이터의 id를 인자로 넘겨주면 수신된 값을 반환(n ~ m, 데이터 없음 = -1) |

---

## 동적 UI 스타일 빌더

Linker 앱 내부 위젯들의 텍스트, 색상, 레이아웃 속성을 하드웨어 단에서 실시간 제어할 수 있는 빌더 객체 설명

> **인스턴스 선언 주의사항:** 스타일 빌더 객체(`ButtonStyleBuilder`, `SwitchStyleBuilder`, `SliderStyleBuilder`) 를 통해 생성하는 문자열 데이터는 `String64` 타입을 사용하여 저장하는것을 권장합니다.
> 
> **메모리 유지 특성:** 동일한 빌더 객체를 재사용하여 `build()`를 호출하는 경우, 명시적으로 변경하지 않은 기존 속성(색상, 텍스트 등)은 이전 설정 상태를 그대로 유지(State retention)합니다.

`String64` = char String64[64]

### 1. `ButtonStyleBuilder`

| 함수 | 설명 |
|---|--|
| **`.setId(int id)`** | 변경 대상 버튼 위젯 ID 지정 |
| **`.setText(int val)`** | 버튼 라벨 텍스트 (정수) |
| **`.setText(float val, int precision = 2)`**| 버튼 라벨 텍스트 (실수, 소수점 자리수) |
| **`.setText(char val)`** | 버튼 라벨 텍스트 (문자) |
| **`.setText(const char* val)`** | 버튼 라벨 텍스트 (문자열) |
| **`.setTextColor(const char* color)`** | 버튼 라벨 텍스트의 색상 (6자리 HEX 포맷 문자열) |
| **`.setButtonColor(const char* color)`** | 버튼 색상 (6자리 HEX 포맷 문자열) |
| **`.build(char* outBuffer, size_t bufferSize)`** | 최종 스타일 빌더 문자열 주소와 길이 |

---

### 2. `SwitchStyleBuilder`

| 함수 | 설명 |
|---|--|
| **`.setId(int id)`** | 변경 대상 스위치 위젯 ID |
| **`.setValue(bool val)`**| 스위치 on off 상태 |
| **`.setText(int val)`** | 스위치 라벨 텍스트 (정수) |
| **`.setText(float val, int precision = 2)`**| 스위치 라벨 텍스트 (실수, 소수점 자리수) |
| **`.setText(char val)`** | 스위치 라벨 텍스트 (문자) |
| **`.setText(const char* val)`** | 스위치 라벨 텍스트 (문자열) |
| **`.setTextColor(const char* color)`** | 스위치 라벨 텍스트의 색상 (6자리 HEX 포맷 문자열) |
| **`.setSwitchColor(const char* color)`** | 스위치 색상 (6자리 HEX 포맷 문자열) |
| **`.build(char* outBuffer, size_t bufferSize)`** | 최종 스타일 빌더 문자열 주소와 길이 |

---

### 3. `SliderStyleBuilder`

| 함수 | 설명 |
|---|--|
| **`.setId(int id)`** | 변경 대상 슬라이더 위젯 ID |
| **`.setValue(int val)`**| 슬라이더 노브의 위치 상태 |
| **`.setText(int val)`** | 슬라이더 라벨 텍스트 (정수) |
| **`.setText(float val, int precision = 2)`**| 슬라이더 라벨 텍스트 (실수, 소수점 자리수) |
| **`.setText(char val)`** | 슬라이더 라벨 텍스트 (문자) |
| **`.setText(const char* val)`** | 슬라이더 라벨 텍스트 (문자열) |
| **`.setTextColor(const char* color)`** | 슬라이더 라벨 텍스트의 색상 (6자리 HEX 포맷 문자열) |
| **`.setSliderHandColor(const char* color)`** | 슬라이더 손잡이(Thumb/Knob)의 색상 (6자리 HEX 포맷 문자열) |
| **`.setSliderLineColor(const char* color)`** | 슬라이더 트랙 바(Track/Line)의 색상 (6자리 HEX 포맷 문자열) |
| **`.build(char* outBuffer, size_t bufferSize)`** | 최종 스타일 빌더 문자열 주소와 길이 |

---

## 데이터 전송 및 트래픽 제어

| 함수 | 설명 |
|--|--|
| **`write(const char* value, int ch)`** | 변경 대상 슬라이더 위젯 문자열 주소와 고유 ID 지정 |
*   빌더를 통해 생성된 스타일 데이터 를 타깃 채널(`ch`)로 송신하여 앱의 화면을 갱신합니다.

### 데이터 송신 간격 제어 지침
하드웨어에서 발생하는 원시 데이터를 필터링 없이 연속 전송(Flood)하는 행위는 엄격히 제한됩니다.
1.  **대역폭 제한:** 물리적인 블루투스 데이터 전송 한계 속도보다 하드웨어의 전송 속도가 더 빠르면 칩셋 내부 송신 버퍼 적체로 데이터 누락이 발생합니다.
2.  **렌더링 병목:** Linker 모바일 애플리케이션의 UI 스레드 및 그래픽 렌더러가 화면을 갱신하는 처리 속도보다 수신 속도가 과도하게 빠르면 앱 내부에 패킷이 쌓여 레이턴시(지연)가 늘어나거나 UI 렌더링 프레임이 드롭됩니다.
3.  **권장 사양:** 패킷 전송 함수 호출 간에는 **최소 10ms(0.01초) 이상의 하드웨어적 시간 딜레이(Interval)를 보장**하여 물리 대역폭과 렌더링 안정성을 확보해야 합니다.
