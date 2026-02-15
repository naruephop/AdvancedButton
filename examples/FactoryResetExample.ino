// Long 10s: ล้าง WiFi credentials ใน NVS แล้วรีสตาร์ท
// Long 2s: เตือน “กำลังจะรีเซ็ต” (กันกดพลาด)

#include <WiFi.h>
#include <AdvancedButton.h>

#define BTN_PIN 13
#define LED_PIN 2

AdvancedButton btn(AdvancedButton::Config{
  .pin = BTN_PIN,
  .usePullup = true,
  .debounceMs = 30,
  .clickGapMs = 350,
  .long1Ms = 2000,
  .long2Ms = 5000,
  .long3Ms = 10000
});

void factoryReset() {
  Serial.println("FACTORY RESET: clearing WiFi credentials...");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true, true); // erase + wifioff
  WiFi.mode(WIFI_OFF);

  Serial.println("Done. Restarting...");
  ESP.restart();
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  btn.begin();

  Serial.println("Factory Reset Ready");
  Serial.println("- Long1(2s): warning");
  Serial.println("- Long3(10s): factory reset + restart");
}

void loop() {
  btn.update();
  auto e = btn.getEvent();

  if (e == AdvancedButton::Event::Long1) {
    Serial.println("Hold 10s to factory reset!");
  }

  if (e == AdvancedButton::Event::Long3) {
    factoryReset();
  }

  // เพิ่ม feedback: กดอยู่ให้ไฟติด
  if (btn.isPressed()) digitalWrite(LED_PIN, HIGH);
  else                 digitalWrite(LED_PIN, LOW);
}