# AdvancedButton

AdvancedButton is a production-ready, non-blocking button handling library for Arduino, ESP32, and ESP8266.

It supports:

- Debounce
- Single / Double / Triple click
- 3-level long press (2s / 5s / 10s)
- Multiple buttons
- Non-blocking (millis-based)
- Safe event latch system (no missed events)

Designed for real-world IoT applications such as MQTT devices, WebServer configuration panels, WiFi provisioning, and embedded control systems.

---

## ✨ Features

| Feature | Supported |
|----------|------------|
| Debounce | ✅ |
| Single Click | ✅ |
| Double Click | ✅ |
| Triple Click | ✅ |
| Long Press 2s | ✅ |
| Long Press 5s | ✅ |
| Long Press 10s | ✅ |
| Multi-button | ✅ |
| Non-blocking | ✅ |
| Production-ready | ✅ |

---

## 📦 Installation (Arduino IDE)

1. Download or clone this repository.
2. Place the folder inside: Documents/Arduino/libraries/
3. Restart Arduino IDE.
4. Include the library:

```cpp
#include <AdvancedButton.h>

---
## 🔌 Hardware Wiring

Recommended wiring:
GPIO ---- Button ---- GND
Library uses INPUT_PULLUP.

Pressed = LOW
Released = HIGH

## 🚀 Quick Example

```cpp
#include <AdvancedButton.h>

#define BTN_PIN 13
#define LED_PIN 2

AdvancedButton btn({BTN_PIN});

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  btn.begin();
}

void loop() {
  btn.update();

  auto e = btn.getEvent();

  if (e == AdvancedButton::Event::Press)
    digitalWrite(LED_PIN, HIGH);

  if (e == AdvancedButton::Event::Release)
    digitalWrite(LED_PIN, LOW);

  if (e == AdvancedButton::Event::Long1)
    Serial.println("Held 2 seconds");
}

---