#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// =======================
// CONFIGURATION
// =======================
const char* ssid = "Avik";
const char* password = "Avikmallick27";

// Correct Firebase URL for your project
String firebaseBase = "https://sleepmonitor-9bc49-default-rtdb.asia-southeast1.firebasedatabase.app";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800); 

#define DHTPIN 4
#define DHTTYPE DHT11
#define ONE_WIRE_BUS 5

DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ds18b20(&oneWire);

// R2 rotation for healthy screen area
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R2, U8X8_PIN_NONE);

// =======================
// SENSORS & STATS
// =======================
float avgTemp, humidity;
float minTemp = 100, maxTemp = -100, sumTemp = 0;
float minHum = 100, maxHum = 0, sumHum = 0;
int count = 0;

unsigned long lastUpload = 0;
const unsigned long interval = 30000; // 30 seconds

void connectWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  timeClient.begin();
}

void readSensors() {
  float h = dht.readHumidity();
  float t1 = dht.readTemperature();
  ds18b20.requestTemperatures();
  float t2 = ds18b20.getTempCByIndex(0);

  if (isnan(h) || isnan(t1) || t2 == DEVICE_DISCONNECTED_C) return;

  humidity = h;
  avgTemp = (t1 + t2) / 2.0;

  // Track Min/Max
  if (avgTemp < minTemp) minTemp = avgTemp;
  if (avgTemp > maxTemp) maxTemp = avgTemp;
  if (humidity < minHum) minHum = humidity;
  if (humidity > maxHum) maxHum = humidity;

  sumTemp += avgTemp;
  sumHum += humidity;
  count++;
}

void displayData() {
  timeClient.update();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tr);

  // Horizontal Layout
  String timeStr = timeClient.getFormattedTime();
  u8g2.drawStr((128 - u8g2.getStrWidth(timeStr.c_str())) / 2, 15, timeStr.c_str());

  char row2[30];
  sprintf(row2, "T:%.1fC | H:%.1f%%", avgTemp, humidity);
  u8g2.drawStr((128 - u8g2.getStrWidth(row2)) / 2, 35, row2);

  char row3[20];
  sprintf(row3, "SAMPLES: %d", count);
  u8g2.drawStr((128 - u8g2.getStrWidth(row3)) / 2, 55, row3);
  
  u8g2.sendBuffer();
}

void uploadData() {
  if (count == 0) return;
  
  StaticJsonDocument<512> doc;
  JsonObject root = doc.createNestedObject("sensorData");
  
  root["date"] = timeClient.getFormattedTime(); // Simplified for now
  root["timeRange"] = "30-Second-Interval";
  
  JsonObject temp = root.createNestedObject("temperature");
  temp["minimum"] = minTemp;
  temp["maximum"] = maxTemp;
  temp["average"] = sumTemp / count;
  
  JsonObject hum = root.createNestedObject("humidity");
  hum["minimum"] = minHum;
  hum["maximum"] = maxHum;
  hum["average"] = sumHum / count;
  
  root["samplingInterval"] = "30 Seconds";

  String json;
  serializeJson(doc, json);

  HTTPClient http;
  http.begin(firebaseBase + "/SleepMonitor/sensorData.json");
  http.addHeader("Content-Type", "application/json");
  
  int httpCode = http.POST(json);
  Serial.print("Upload Status: "); Serial.println(httpCode);
  
  http.end();
  
  // Reset
  minTemp = 100; maxTemp = -100; minHum = 100; maxHum = 0;
  sumTemp = 0; sumHum = 0; count = 0;
}

void setup() {
  Serial.begin(115200);
  dht.begin(); ds18b20.begin(); u8g2.begin();
  connectWiFi();
}

void loop() {
  readSensors();
  displayData();
  if (millis() - lastUpload >= interval) {
    uploadData();
    lastUpload = millis();
  }
  delay(1000);
}
