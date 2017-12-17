#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D3
#define EEPROM_SIZE  4096
#define RECORD_SIZE  8

//-------- Connection variables ----------
const char WiFiAPPSK[] = "mrndajebuh";
ESP8266WebServer server(80);
//----------------------------------------

//-------- Temperature variables ---------
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
float tempDev;
long lastMeasure = 0;
long eepromIndex = 0;
long measuringDelay = 2000;
long baseTime = 0;
//----------------------------------------


void setup()
{
  initHardware();
  setupWiFi();
  setupServer();
}

void loop()
{
  int elapsedTime = millis() - lastMeasure;

  if (elapsedTime > measuringDelay)
  {
    measure();
  }
  server.handleClient();
}

void measure()
{
  DS18B20.setWaitForConversion(false);
  DS18B20.requestTemperatures();
  float tempC = DS18B20.getTempCByIndex(0);
  lastMeasure = (int) millis();
  storeTemperature(lastMeasure, tempC);
  tempDev = tempC;
}

void storeTemperature(int timestamp, float temperature)
{
  if (eepromIndex >= EEPROM_SIZE - RECORD_SIZE) eepromIndex = 0;

  Serial.print("Log timestamp");
  for (int i = 0; i < 2; i++)
  {
    EEPROM.write(eepromIndex + i, (timestamp >> (8 * i)) & 255);
  }
  Serial.println(timestamp);
  eepromIndex += 2;
  String tempString = String(temperature);
  Serial.print("Log temperature");
  for (int i = 0; i < 6; i++)
  {
    Serial.print(tempString[i]);
    EEPROM.write(eepromIndex + i, tempString[i]);
  }
  Serial.println("");
  eepromIndex += 6;
}

void handleRTC(){
  server.send(200, "text/plain", String(millis()));
}

void handleHistory(){
  String s = "";
  s += "[";
  for (int i = 0; i < eepromIndex / RECORD_SIZE;)
  {
    int timestamp = 0;
    String temperature = "";
    for (int y = 0; y < 2; y++)
    {
      byte value = EEPROM.read(i + y);
      timestamp |= (value << (y * 8));
    }
    i += 2;
    for (int y = 0; y < 5; y++)
    {
      char value = EEPROM.read(i + y);
      temperature += value;
    }
    s += "{\n";
    s += "  \"time\": " + String(timestamp + baseTime) + ",\n";
    s += "  \"value\": " + temperature + "\n";
    s += "},\n";
    i += 6;
  }
  s += "]";
  server.send(200, "application/json", s);
}

void handleTemperature(){
  server.send(200, "application/json", "Text");
}

void setupServer()
{
  server.on("/rtc", HTTP_GET, handleRTC);
  server.on("/history", HTTP_GET, handleHistory);
  server.on("/temperature", HTTP_GET, handleTemperature);
  server.onNotFound([](){server.send(404, "text/plain", "URI not found" + server.uri());});
  server.begin();
}

void setupWiFi()
{
  WiFi.mode(WIFI_AP);

  String AP_NameString = "Temperature station";

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i = 0; i < AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, WiFiAPPSK);
}

void initHardware()
{
  Serial.begin(115200);
  EEPROM.begin(4096);
  DS18B20.begin();
}
