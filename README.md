# AdvancedButton

AdvancedButton is a production-ready, non-blocking button handling library for Arduino, ESP32, and ESP8266.

It supports:

- Debounce
- Single / Double / Triple click
- 3-level long press (2s / 5s / 10s)
- Multiple buttons
- Non-blocking operation (millis-based)
- Event latch system (no missed events)

Designed for real-world IoT systems such as MQTT devices, WebServer-based configuration panels, WiFi provisioning, and industrial controllers.

---

## 🚀 Why AdvancedButton?

Most button examples use `delay()` or simple edge detection.

This library provides:

- Deterministic behavior
- Clean state machine logic
- No blocking code
- Safe event handling
- Production stability

Perfect for systems where timing matters.