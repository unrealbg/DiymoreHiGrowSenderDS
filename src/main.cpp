/*
 Name:		Diymore HiGrow Sender
 Created:	16-Mar-22 12:13:38
 Author:	Zhelyazkov (unrealbg)
*/

#include <Arduino.h>
#include <mqtt-auth.h>
#include <user-variables.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>
#include <PubSubClient.h>
#include <DHTesp.h>

WiFiClient espClient;
PubSubClient client(espClient);
DHTesp dht;
AsyncWebServer server(80);
DNSServer dns;

void setup_mqtt()
{
  client.setServer(mqttServer, mqttPort);

  while (!client.connected())
  {
    Serial.println("Connecting to MQTT...");

    if (client.connect("DiyMoreModule", mqttUser, mqttPassword))
    {
      Serial.println("connected");
    }
    else
    {

      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void setup()
{
  sleepus = TIME_TO_SLEEP * uS_TO_S_FACTOR;
  esp_sleep_enable_timer_wakeup(sleepus);

  // Debug console
  Serial.begin(115200);

  AsyncWiFiManager wifiManager(&server, &dns);
  wifiManager.autoConnect();
  setup_mqtt();

  dht.setup(22, DHTesp::DHT11);
}

void loop()
{
  Humidity = dht.getHumidity();
  Temperature = dht.getTemperature();

  client.publish("home/wemos_new/temp", String(Temperature).c_str(), true);
  client.publish("home/wemos_new/humidity", String(Humidity).c_str(), true);

  soilMoistureValue = analogRead(SensorPin);
  soilmoisturepercent = map(soilMoistureValue, Dry, Wet, 0, 100);

  if (soilmoisturepercent > 100)
  {
    client.publish("home/wemos_new/soil", "100", true);
  }
  else if (soilmoisturepercent < 0)
  {
    client.publish("home/wemos_new/soil", "0", true);
  }
  else if (soilmoisturepercent >= 0 && soilmoisturepercent <= 100)
  {
    client.publish("home/wemos_new/soil", String(soilmoisturepercent).c_str(), true);
  }

  Serial.flush();
  delay(2000);

  esp_deep_sleep_start();
}