#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D3
#define EEPROM_SIZE  4096
#define RECORD_SIZE  10

#define AP_NAME "Temperature station"
#define AP_PWD  "mrndajebuh"

//-------- Connection variables ----------
ESP8266WebServer server(80);
//----------------------------------------

//-------- Temperature variables ---------
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
int numberOfValues = 0;
long lastMeasure = 0;
long measuringDelay = 2000;
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
  Serial.print("Measuring");
  DS18B20.setWaitForConversion(false);
  DS18B20.requestTemperatures();
  float tempC = DS18B20.getTempCByIndex(0);
  lastMeasure = millis();
  Serial.print(" at " + String(lastMeasure));
  Serial.println(" value " + String(tempC));
  storeTemperature(lastMeasure, tempC);
}

void storeTemperature(long timestamp, float temperature)
{
  int eepromIndex = numberOfValues * RECORD_SIZE;
  if (eepromIndex >= EEPROM_SIZE) {
    numberOfValues = 0;
  }
  for (int i = 0; i < 4; i++)
  {
    EEPROM.write(eepromIndex + i, (timestamp >> (8 * i)) & 255);
  }
  eepromIndex += 4;
  String tempString = String(temperature);
  for (int i = 0; i < 6; i++)
  {
    EEPROM.write(eepromIndex + i, tempString[i]);
  }
  EEPROM.commit();
  eepromIndex += 6;
  numberOfValues++;
}

void handleRTC(){
  server.send(200, "text/plain", String(millis()));
}

void handleHistory(){
  String s = "";
  s += "[";
  Serial.println("Getting history of " + String(numberOfValues));
  for (int i = 0; i < numberOfValues; i++)
  {
    int timestamp = 0;
    String temperature = "";
    int index = i * RECORD_SIZE;
    for (int y = 0; y < 4; y++)
    {
      byte value = EEPROM.read(index + y);
      timestamp |= (value << (y * 8));
    }
    index += 4;
    for (int y = 0; y < 5; y++)
    {
      char value = EEPROM.read(index + y);
      temperature += value;
    }
    s += "{\n";
    s += "  \"time\": " + String(timestamp) + ",\n";
    s += "  \"value\": " + temperature + "\n";
    s += "},\n";
  }
  s += "]";
  server.send(200, "application/json", s);
}

void handleRefresh(){
  if(server.args() == 0) {
    server.send(500, "text/plain", "Missing argument");
  }
  String refreshArg = server.arg(0);
  measuringDelay = refreshArg.toInt();
  server.send(200, "text/plain", "OK");
}

void handleGetRefresh()
{
  server.send(200, "text/plain", String(measuringDelay));
}

void setupServer()
{
  server.on("/rtc", HTTP_GET, handleRTC);
  server.on("/history", HTTP_GET, handleHistory);
  server.on("/refresh", HTTP_GET, handleGetRefresh);
  server.on("/refresh", HTTP_PUT, handleRefresh);
  server.onNotFound([](){server.send(404, "text/plain", "URI not found" + server.uri());});
  server.begin();
}

void setupWiFi()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_NAME, AP_PWD);
}

void initHardware()
{
  Serial.begin(115200);
  EEPROM.begin(4096);
  DS18B20.begin();
}
