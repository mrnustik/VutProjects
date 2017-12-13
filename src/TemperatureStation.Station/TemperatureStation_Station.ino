#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define MEASURING_DELAY 2500
#define ONE_WIRE_BUS D3

const char WiFiAPPSK[] = "mrndajebuh";

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
float tempDev;
long lastMeasure = 0;
long eepromIndex = 0;
WiFiServer server(80);

void setup()
{
  initHardware();
  setupWiFi();
  server.begin();
}

void loop()
{
  // Check if a client has connected
  int elapsedTime = millis() - lastMeasure;

  if (elapsedTime > MEASURING_DELAY)
  {
    measure();
  }

  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  if (req.indexOf("/rtc") != -1)
  {
    sendRTC(&client);
  }
  else if (req.indexOf("/temperature") != -1)
  {
    sendLastMeasuredTemperature(&client);
  }
  else if (req.indexOf("/history") != -1)
  {
    sendHistory(&client);
  }
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
  if (eepromIndex >= 512 - 8) eepromIndex = 0;

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

void sendRTC(WiFiClient *client)
{
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";
  s += String(millis());
  client->print(s);
}

void sendLastMeasuredTemperature(WiFiClient *client)
{
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";
  s += String(tempDev);
  client->print(s);
}


void sendHistory(WiFiClient *client)
{
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: application/json\r\n\r\n";
  s += "[";
  for (int i = 0; i < eepromIndex / 10;)
  {
    int timestamp = 0;
    char temperature[5];
    for (int y = 0; y < 2; y++)
    {
      byte value = EEPROM.read(i + y);
      timestamp |= (value << (y * 8));
    }
    i += 2;
    for (int y = 0; y < 5; y++)
    {
      byte value = EEPROM.read(i + y);
      temperature[y] = value;
    }
    s += "{\n";
    s += "  \"time\": " + String(timestamp) + ",\n";
    s += "  \"value\": " + String(temperature) + "\n";
    s += "},\n";
    i += 6;
  }
  s += "]";
  client->print(s);
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
  EEPROM.begin(512);
  DS18B20.begin();
}
