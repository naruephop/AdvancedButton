//example ควบคุม WiFi AP Mode ด้วยปุ่มกด โดยใช้ไลบรารี AdvancedButton 
//Click 1 ครั้ง: สลับ AP ON/OFF
//Long 2s: พิมพ์สถานะ WiFi ปัจจุบัน
#include <WiFi.h>
#include <AdvancedButton.h>

#define BTN_PIN 13
#define LED_PIN 2

AdvancedButton btn({BTN_PIN});

bool apOn = false;

void startAP() {
  // ตั้งชื่อ AP แบบง่าย ๆ
  WiFi.mode(WIFI_AP);
  const char* ssid = "ESP32-SETUP";
  const char* pass = "12345678"; // อย่างน้อย 8 ตัว
  bool ok = WiFi.softAP(ssid, pass);

  Serial.printf("AP start: %s, SSID=%s, IP=%s\n",
                ok ? "OK" : "FAIL",
                ssid,
                WiFi.softAPIP().toString().c_str());
}

void stopAP() {
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_OFF);
  Serial.println("AP stopped");
}

void printWiFiStatus() {
  wifi_mode_t m = WiFi.getMode();
  Serial.printf("WiFi mode=%d (0=OFF,1=STA,2=AP,3=AP+STA)\n", (int)m);
  if (m == WIFI_AP || m == WIFI_AP_STA) {
    Serial.printf("AP IP=%s, clients=%d\n",
                  WiFi.softAPIP().toString().c_str(),
                  WiFi.softAPgetStationNum());
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  btn.begin();
  WiFi.mode(WIFI_OFF);

  Serial.println("AP Mode Control Ready");
  Serial.println("- Click1: Toggle AP ON/OFF");
  Serial.println("- Long1(2s): Print WiFi status");
}

void loop() {
  btn.update();
  auto e = btn.getEvent();

  if (e == AdvancedButton::Event::Click1) {
    apOn = !apOn;
    if (apOn) { startAP(); digitalWrite(LED_PIN, HIGH); }
    else      { stopAP();  digitalWrite(LED_PIN, LOW);  }
  }

  if (e == AdvancedButton::Event::Long1) {
    printWiFiStatus();
  }
}