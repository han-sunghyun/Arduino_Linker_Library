# Linker Library for Arduino & ESP32

A powerful embedded library for multi-channel communication and dynamic widget styling with the Linker App.  
Linker 앱과의 다중 통신 채널 연결 및 동적 위젯 스타일링을 위한 임베디드 라이브러리입니다.


## Supported MCU & Hardware Interfaces

The Linker library supports 4 types of initialization functions depending on the hardware architecture, and can connect up to 3 independent communication channels (`Ch 0 ~ 2`) simultaneously.

### 1. Optimized Initialization Functions by Hardware

| Initialization Function | Supported MCU | Description |
| :--- | :--- | :--- |
| **`.beginInternal(String deviceName, int ch)`** | **ESP32** | Activates the built-in Classic Bluetooth stack of the ESP32. |
| **`.begin(int rx, int tx, int ch, long baud)`** | **AVR-based boards**<br>(Arduino UNO R3, NANO, etc.) | Communicates with an external Bluetooth module via SoftwareSerial. |
| **`.begin(HardwareSerial& hwSerial, int ch, long baud)`** | **Multi-Serial boards**<br>(Arduino MEGA 2560, Due, UNO R4, etc.) | Allocates and communicates through hardware serial ports (`Serial1`, `Serial2`, etc.). |
| **`.begin(int uart_nr, int rx, int tx, int ch, long baud)`** | **ESP32 family**<br>(ESP32, S2, S3, C2, C3, etc.) | Communicates by explicitly defining the hardware serial pin mapping. |

### 2. Reception Mechanism Differences & Precautions
*   **`linker.update()`**
    *   **AVR Series (UNO, NANO, etc.):** Since it is not hardware-interrupt-driven, this function must be called manually inside every `loop()` iteration to poll the receive buffer.
    *   **ESP32 Series:** The reception is automatically scheduled inside a background task of the internal FreeRTOS. Therefore, `update()` can be omitted or called less frequently to reduce overhead.

---

## Data Reception & Callback Functions

When data arrives from the Linker app, it is processed asynchronously by the registered callback structure. It identifies the communication channel ID (`ch`), verifies the target data, and branches to hardware control.

> **Callback Precaution (RTOS):** In the ESP32 architecture, the callback function executes within a separate, independent RTOS task context rather than the main loop. Therefore, you should avoid calling `delay()` or performing heavy computations inside the callback to ensure a quick return and keep the code lightweight.


### Callback Registration

```cpp
void callback(int ch) {

}

void setup(){
    linker.onDataReceived(callback);
}
```

### Update Verification Functions by Widget

| Widget Type | Update Verification | Widget ID Function | Read State Function |
| :--- | :--- | :--- | :--- |
| **Button** | `.isButtonUpdated()` | `.getButtonId()` | `.buttonRead(int8_t button_id)`<br>• `1`: Pressed (Push)<br>• `0`: Released (Release)<br>• `-1`: ID Mismatch/Fail |
| **Switch** | `.isSwitchUpdated()` | `.getSwitchId()` | `.switchRead(int8_t switch_id)`<br>• `1`: ON<br>• `0`: OFF<br>• `-1`: ID Mismatch/Fail |
| **Slider** | `.isSliderUpdated()` | `.getSliderId()` | `.sliderRead(int8_t slider_id)`<br>• `n ~ m`: Positive integer value within range<br>• `-1`: ID Mismatch/Fail |

---

## Dynamic UI Style Builders

Description of builder objects used to control the text, color, and layout properties of widgets inside the Linker app in real-time from the hardware side.

> **Instance Declaration Precaution:** Style builder objects (`ButtonStyleBuilder`, `SwitchStyleBuilder`, `SliderStyleBuilder`) and the `String` variables holding the final protocol packet must be declared **strictly in the global space**. Repeated dynamic allocation and destruction inside `loop()` or conditional/loop statements will cause heap memory fragmentation and system crashes.
> 
> **State Retention Feature:** When reusing the same builder object to call `build()`, any existing properties (color, text, etc.) that are not explicitly changed will retain their previously configured states.

### 1. `ButtonStyleBuilder` Methods
*   **`setId(int8_t id)`**: Specifies the unique ID of the target button widget to identify the target change.
*   **`setText(Value)`**: Changes the button's text label (Supports overloading for int, float, char, and String).
*   **`setTextColor(String hex)`**: Changes text color (6-character HEX format string, e.g., `"FFFFFF"`).
*   **`setButtonColor(String hex)`**: Changes button background color (6-character HEX format string, e.g., `"000000"`).
*   **`build()`**: Combines the configured properties and returns the final style protocol `String` matching the Linker app specification.

### 2. `SwitchStyleBuilder` Methods
*   **`setId(int8_t id)`**: Specifies the unique ID of the target switch widget.
*   **`setValue(bool state)`**: Controls the initial logical state of the switch (`true`/`false`).
*   **`setText(Value)`**: Specifies the switch label text (Supports int, float, char, and String).
*   **`setTextColor(String hex)`**: Changes the label text color (6-character HEX format string).
*   **`setSwitchColor(String hex)`**: Specifies the background color when the switch is active (6-character HEX format string).
*   **`build()`**: Returns the final style protocol `String`.

### 3. `SliderStyleBuilder` Methods
*   **`setId(int8_t id)`**: Specifies the unique ID of the target slider widget.
*   **`setValue(int value)`**: Sets the initial position value (integer) of the slider adjustment knob.
*   **`setText(Value)`**: Specifies the slider label text (Supports int, float, char, and String).
*   **`setTextColor(String hex)`**: Changes the slider label text color (6-character HEX format string).
*   **`setSliderHandColor(String hex)`**: Changes the color of the slider adjustment knob (Thumb/Knob) (6-character HEX format string).
*   **`setSliderLineColor(String hex)`**: Changes the background color of the slider track bar (Track/Line) (6-character HEX format string).
*   **`build()`**: Returns the final style protocol `String`.

---

## Data Transmission & Traffic Control

*   **`linker.write(String packet, int ch)`**
    *   Transmits the style protocol data (`packet`) generated by the builder to the target channel (`ch`) to refresh the app screen.

### Transmission Interval Guide
Flooding raw data continuously from the hardware without filtering is strictly prohibited.
1.  **Bandwidth Limitation:** If the transmission rate from the hardware exceeds the physical Bluetooth data transfer speed limit, data loss occurs due to transmission buffer accumulation inside the chipset.
---
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
*   **`linker.update()`**
    *   **AVR 계열 (UNO, NANO 등):** 하드웨어 인터럽트 기반이 아니므로 매 `loop()` 반복마다 이 함수를 수동 호출하여 수신 버퍼를 폴링해야 합니다.
    *   **ESP32 계열:** 내부 프리 RTOS(FreeRTOS)의 백그라운드 태스크 내부에서 수신 여부를 자동 스케줄링하므로 `update()`를 생략하거나 호출 오버헤드를 낮출 수 있습니다.

---

## 데이터 수신 및 콜백함수

Linker 앱으로부터 데이터가 유입되면 등록된 콜백 구조에 의해 비동기 처리됩니다. 수신 채널 ID(`ch`)를 식별하여 타깃 데이터 유무를 검증하고 하드웨어 제어로 분기합니다.

> **콜백 작성 주의사항 (RTOS):** ESP32 아키텍처에서는 콜백 함수가 메인 루프와 독립된 별도의 RTOS 태스크 콘텍스트 내에서 실행됩니다. 따라서 콜백 내부에서는 `delay()` 호출이나 무거운 연산을 지양하고 빠른 리턴이 가능하도록 코드를 가볍게 유지해야 합니다.


### 콜백 등록

```cpp
void callback(int ch) {

}

void setup(){
    linker.onDataReceived(callback);
}
```


### 위젯별 데이터 수신확인 함수

| 위젯 구분 | 수신 확인 함수 | 위젯 ID 반환 함수 | 상태 값 반환 함수 |
| :--- | :--- | :--- | :--- |
| **Button** | `.isButtonUpdated()` | `.getButtonId()` | `.buttonRead(int8_t button_id)`<br>• `1`: 누름 (Push)<br>• `0`: 뗌 (Release)<br>• `-1`: ID 불일치/실패 |
| **Switch** | `.isSwitchUpdated()` | `.getSwitchId()` | `.switchRead(int8_t switch_id)`<br>• `1`: ON<br>• `0`: OFF<br>• `-1`: ID 불일치/실패 |
| **Slider** | `.isSliderUpdated()` | `.getSliderId()` | `.sliderRead(int8_t slider_id)`<br>• `n ~ m`: 설정된 범위의 양의 정수<br>• `-1`: ID 불일치/실패 |

---

## 동적 UI 스타일 빌더

Linker 앱 내부 위젯들의 텍스트, 색상, 레이아웃 속성을 하드웨어 단에서 실시간 제어할 수 있는 빌더 객체 설명

> **인스턴스 선언 주의사항:** 스타일 빌더 객체(`ButtonStyleBuilder`, `SwitchStyleBuilder`, `SliderStyleBuilder`) 및 최종 프로토콜 패킷을 담을 `String` 변수는 반드시 **전역(Global) 공간에만 선언**해야 합니다. `loop()`나 조건/반복문 내부에서 동적 할당 및 소멸을 반복하면 힙 메모리 단편화 및 시스템 다운을 유발합니다.
> 
> **메모리 유지 특성:** 동일한 빌더 객체를 재사용하여 `build()`를 호출하는 경우, 명시적으로 변경하지 않은 기존 속성(색상, 텍스트 등)은 이전 설정 상태를 그대로 유지(State retention)합니다.

### 1. `ButtonStyleBuilder` 메서드 구조
*   **`setId(int8_t id)`**: 변경 대상을 식별할 버튼 위젯 고유 ID 지정
*   **`setText(Value)`**: 버튼의 텍스트 라벨 변경 (정수, 실수, 문자, 문자열 타입 오버로딩 지원)
*   **`setTextColor(String hex)`**: 텍스트 색상 변경 (6자리 HEX 포맷 문자열, 예: `"FFFFFF"`)
*   **`setButtonColor(String hex)`**: 버튼의 배경 색상 변경 (6자리 HEX 포맷 문자열, 예: `"000000"`)
*   **`build()`**: 설정된 속성들을 조합하여 Linker 앱 규격에 맞는 최종 스타일 프로토콜 `String`을 반환

### 2. `SwitchStyleBuilder` 메서드 구조
*   **`setId(int8_t id)`**: 변경 대상 스위치 위젯 고유 ID 지정
*   **`setValue(bool state)`**: 스위치의 On/Off 초기 논리 상태 제어 (`true`/`false`)
*   **`setText(Value)`**: 스위치 라벨 텍스트 지정 (정수, 실수, 문자, 문자열 지원)
*   **`setTextColor(String hex)`**: 라벨 텍스트의 색상 변경 (6자리 HEX 포맷 문자열)
*   **`setSwitchColor(String hex)`**: 스위치 활성화(Active) 시의 고유 배경색 지정 (6자리 HEX 포맷 문자열)
*   **`build()`**: 최종 스타일 프로토콜 `String` 반환

### 3. `SliderStyleBuilder` 메서드 구조
*   **`setId(int8_t id)`**: 변경 대상 슬라이더 위젯 고유 ID 지정
*   **`setValue(int value)`**: 슬라이더 조절 노브의 초기 위치 값(정수형) 설정
*   **`setText(Value)`**: 슬라이더 라벨 텍스트 지정 (정수, 실수, 문자, 문자열 지원)
*   **`setTextColor(String hex)`**: 슬라이더 라벨 텍스트의 색상 변경 (6자리 HEX 포맷 문자열)
*   **`setSliderHandColor(String hex)`**: 슬라이더 움직이는 손잡이(Thumb/Knob)의 색상 변경 (6자리 HEX 포맷 문자열)
*   **`setSliderLineColor(String hex)`**: 슬라이더 트랙 바(Track/Line)의 바탕 색상 변경 (6자리 HEX 포맷 문자열)
*   **`build()`**: 최종 스타일 프로토콜 `String` 반환

---

## 데이터 전송 및 트래픽 제어

*   **`linker.write(String packet, int ch)`**
    *   빌더를 통해 생성된 스타일 프로토콜 데이터(`packet`)를 타깃 채널(`ch`)로 송신하여 앱의 화면을 갱신합니다.

### 패킷 송신 간격 제어 지침
하드웨어에서 발생하는 원시 데이터를 필터링 없이 연속 전송(Flood)하는 행위는 엄격히 제한됩니다.
1.  **대역폭 제한:** 물리적인 블루투스 데이터 전송 한계 속도보다 하드웨어의 전송 속도가 더 빠르면 칩셋 내부 송신 버퍼 적체로 데이터 누락이 발생합니다.
2.  **렌더링 병목:** Linker 모바일 애플리케이션의 UI 스레드 및 그래픽 렌더러가 화면을 갱신하는 처리 속도보다 수신 속도가 과도하게 빠르면 앱 내부에 패킷이 쌓여 레이턴시(지연)가 늘어나거나 UI 렌더링 프레임이 드롭됩니다.
3.  **권장 사양:** 패킷 전송 함수 호출 간에는 **최소 10ms(0.01초) 이상의 하드웨어적 시간 딜레이(Interval)를 보장**하여 물리 대역폭과 렌더링 안정성을 확보해야 합니다.
