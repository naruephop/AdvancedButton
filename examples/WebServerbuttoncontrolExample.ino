//	Click1: toggle LED
//	Long2(5s): เปิด AP setup (ตัวอย่าง) แล้วรันเว็บ
//	Web:
//   •	/ แสดงหน้า HTML
//   •	/api/status คืนค่า JSON
//   •	/api/toggle toggle LED

#include <WiFi.h>
#include <WebServer.h>
#include <AdvancedButton.h>

#define BTN_PIN 13
#define LED_PIN 2

AdvancedButton btn({BTN_PIN});
WebServer server(80);

bool ledOn = false;
bool apMode = false;

void applyLed() {
  digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
}

void startAP() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP32-WEB", "12345678");
  apMode = true;
  Serial.printf("AP started IP=%s\n", WiFi.softAPIP().toString().c_str());
}

void handleRoot() {
  // HTML ง่าย ๆ (ไม่ต้องมีไฟล์)
  String html;
  html.reserve(600);
  html += "<!doctype html><html><head><meta charset='utf-8'>";
  html += "<meta name='viewport' content='width=device-width,initial-scale=1'>";
  html += "<title>ESP32 Button Web</title></head><body>";
  html += "<h2>ESP32 Button + WebServer</h2>";
  html += "<p>LED: <b>";
  html += (ledOn ? "ON" : "OFF");
  html += "</b></p>";
  html += "<button onclick=\"fetch('/api/toggle',{method:'POST'}).then(()=>location.reload())\">Toggle</button>";
  html += "<p><a href='/api/status'>/api/status</a></p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleStatus() {
  String json = String("{\"led\":") + (ledOn ? "true" : "false") +
                ",\"mode\":\"" + (apMode ? "AP" : "STA/OFF") + "\"}";
  server.send(200, "application/json", json);
}

void handleToggle() {
  ledOn = !ledOn;
  applyLed();
  server.send(200, "application/json", String("{\"ok\":true,\"led\":") + (ledOn ? "true" : "false") + "}");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  applyLed();

  btn.begin();

  // เริ่มเป็น OFF ไว้ก่อน แล้วค่อยเปิด AP ด้วยปุ่ม (Long2)
  WiFi.mode(WIFI_OFF);

  server.on("/", HTTP_GET, handleRoot);
  server.on("/api/status", HTTP_GET, handleStatus);
  server.on("/api/toggle", HTTP_POST, handleToggle);
  server.begin();

  Serial.println("WebServer Button Control Ready");
  Serial.println("- Click1: toggle LED");
  Serial.println("- Long2(5s): start AP (ESP32-WEB / 12345678)");
  Serial.println("- Open http://192.168.4.1 after AP start");
}

void loop() {
  btn.update();
  auto e = btn.getEvent();

  if (e == AdvancedButton::Event::Click1) {
    ledOn = !ledOn;
    applyLed();
    Serial.printf("LED=%s\n", ledOn ? "ON" : "OFF");
  }

  if (e == AdvancedButton::Event::Long2) {
    if (!apMode) startAP();
  }

  server.handleClient();
}