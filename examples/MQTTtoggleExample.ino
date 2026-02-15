//Click 1: toggle state แล้ว publish ไป MQTT
//Click 2: reconnect MQTT
//Long 2s: พิมพ์สถานะการเชื่อมต่อ
#include <WiFi.h>
#include <PubSubClient.h>
#include <AdvancedButton.h>

#define BTN_PIN 13
#define LED_PIN 2

// ===== ใส่ค่าของคุณ =====
const char* WIFI_SSID = "YOUR_WIFI";
const char* WIFI_PASS = "YOUR_PASS";

const char* MQTT_HOST = "192.168.1.10";
const uint16_t MQTT_PORT = 1883;

const char* TOPIC = "device/demo/toggle";

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

AdvancedButton btn({BTN_PIN});

bool stateOn = false;
unsigned long lastWiFiAttempt = 0;
unsigned long lastMqttAttempt = 0;

void ensureWiFi(unsigned long now) {
  if (WiFi.status() == WL_CONNECTED) return;
  if (now - lastWiFiAttempt < 3000) return;

  lastWiFiAttempt = now;
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("WiFi connecting...");
}

void ensureMQTT(unsigned long now) {
  if (WiFi.status() != WL_CONNECTED) return;
  if (mqtt.connected()) return;
  if (now - lastMqttAttempt < 3000) return;

  lastMqttAttempt = now;
  mqtt.setServer(MQTT_HOST, MQTT_PORT);

  String clientId = "esp32-" + String((uint32_t)ESP.getEfuseMac(), HEX);
  bool ok = mqtt.connect(clientId.c_str());
  Serial.printf("MQTT connect: %s\n", ok ? "OK" : "FAIL");
}

void publishState() {
  if (!mqtt.connected()) {
    Serial.println("MQTT not connected. Skip publish.");
    return;
  }
  const char* payload = stateOn ? "ON" : "OFF";
  bool ok = mqtt.publish(TOPIC, payload, true);
  Serial.printf("Publish %s -> %s (%s)\n", TOPIC, payload, ok ? "OK" : "FAIL");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  btn.begin();
  WiFi.mode(WIFI_STA);

  Serial.println("MQTT Toggle Ready");
  Serial.println("- Click1: toggle & publish");
  Serial.println("- Click2: force mqtt reconnect");
  Serial.println("- Long1(2s): print status");
}

void loop() {
  unsigned long now = millis();

  ensureWiFi(now);
  ensureMQTT(now);

  if (mqtt.connected()) mqtt.loop();

  btn.update();
  auto e = btn.getEvent();

  if (e == AdvancedButton::Event::Click1) {
    stateOn = !stateOn;
    digitalWrite(LED_PIN, stateOn ? HIGH : LOW);
    publishState();
  }

  if (e == AdvancedButton::Event::Click2) {
    if (mqtt.connected()) mqtt.disconnect();
    Serial.println("MQTT reconnect requested");
  }

  if (e == AdvancedButton::Event::Long1) {
    Serial.printf("WiFi=%s, MQTT=%s, state=%s\n",
                  (WiFi.status() == WL_CONNECTED) ? "CONNECTED" : "DISCONNECTED",
                  mqtt.connected() ? "CONNECTED" : "DISCONNECTED",
                  stateOn ? "ON" : "OFF");
  }
}