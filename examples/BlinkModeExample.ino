#include <AdvancedButton.h>

#define BTN_PIN 13
#define LED_PIN 2

// ===== สร้างปุ่ม =====
AdvancedButton btn({BTN_PIN});

// ===== โหมดไฟ =====
enum class LedMode {
  Off,
  On,
  BlinkSlow,
  BlinkFast
};

LedMode currentMode = LedMode::Off;

// ===== ตัวแปรกระพริบ =====
unsigned long lastBlinkMs = 0;
bool ledState = false;

// ===== ตั้งค่าเวลา =====
const unsigned long BLINK_SLOW_MS = 1000;
const unsigned long BLINK_FAST_MS = 200;

void setMode(LedMode mode) {
  currentMode = mode;
  Serial.print("Mode -> ");
  switch (mode) {
    case LedMode::Off: Serial.println("OFF"); break;
    case LedMode::On: Serial.println("ON"); break;
    case LedMode::BlinkSlow: Serial.println("BLINK SLOW"); break;
    case LedMode::BlinkFast: Serial.println("BLINK FAST"); break;
  }
}

void handleButton(AdvancedButton::Event e) {

  if (e == AdvancedButton::Event::Click1) {
    // Toggle LED ON/OFF
    if (currentMode == LedMode::Off)
      setMode(LedMode::On);
    else
      setMode(LedMode::Off);
  }

  if (e == AdvancedButton::Event::Click2) {
    setMode(LedMode::BlinkSlow);
  }

  if (e == AdvancedButton::Event::Click3) {
    setMode(LedMode::Off);
  }

  if (e == AdvancedButton::Event::Long1) {
    setMode(LedMode::BlinkSlow);
  }

  if (e == AdvancedButton::Event::Long2) {
    setMode(LedMode::BlinkFast);
  }

  if (e == AdvancedButton::Event::Long3) {
    setMode(LedMode::On);
  }

  if (e != AdvancedButton::Event::None) {
    Serial.println(AdvancedButton::eventName(e));
  }
}

void updateLed() {
  unsigned long now = millis();

  switch (currentMode) {

    case LedMode::Off:
      digitalWrite(LED_PIN, LOW);
      break;

    case LedMode::On:
      digitalWrite(LED_PIN, HIGH);
      break;

    case LedMode::BlinkSlow:
      if (now - lastBlinkMs >= BLINK_SLOW_MS) {
        lastBlinkMs = now;
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
      }
      break;

    case LedMode::BlinkFast:
      if (now - lastBlinkMs >= BLINK_FAST_MS) {
        lastBlinkMs = now;
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
      }
      break;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  btn.begin();

  setMode(LedMode::Off);
  Serial.println("System Ready");
}

void loop() {

  // อัปเดตสถานะปุ่ม
  btn.update();

  // ดึง event
  auto e = btn.getEvent();
  handleButton(e);

  // อัปเดตไฟ
  updateLed();
}