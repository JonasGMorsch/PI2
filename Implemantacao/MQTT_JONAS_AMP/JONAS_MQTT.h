#include <PubSubClient.h> // MQTT
#include <PolledTimeout.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <J_IDs.h> //ID/Password for Wifi/MQTT

#define LED D4  //LED!
#define CHIP_ID String(ESP.getChipId()).c_str()
#define RESET "reset"

WiFiClient espClient; // Make object espClient
PubSubClient MQTT(espClient); // PASS WIFI OBJECT TO MQTT HANDLER
uint32_t mqttConnectionTimeStamp;

void mqttCallback(char*, byte*, unsigned int);
void MQTT_Handler(String, String);

void initWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.setPhyMode(WIFI_PHY_MODE_11N); // WiFi N mode, best speed, lower range
  WiFi.setOutputPower(25); //output power of WiFi, dBm range: max +20.5dBm  min 0dBm
#ifdef HOST_NAME
  WiFi.hostname(HOST_NAME);
#endif
  WiFi.persistent(true);
  WiFi.setAutoReconnect(true);

#if DTIM >= 1
  WiFi.setSleepMode(WIFI_LIGHT_SLEEP, DTIM);
  //Serial.println("DTIM 2 TO 10 - POWERSAVE");
#endif

#if DTIM == 0
  WiFi.setSleepMode(WIFI_MODEM_SLEEP);
  //Serial.println("DTIM 1 - BALANCED");
#endif

#ifndef DTIM
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  //Serial.println("DTIM 0 - PERFORMANCE");
#endif

  WiFi.begin(WIFI_ID, WIFI_PASS); // Conecta na rede WI-FI

#ifdef DEBUG
  Serial.println("------ WI-FI ------");
  Serial.print("Conecting to: ");
  Serial.println(WIFI_ID);
  Serial.println("Wait");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Conection successful");
  WiFi.printDiag(Serial);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
#endif

  ArduinoOTA.setHostname(HOST_NAME);
  ArduinoOTA.begin();
}

void mqttCallback(char* t, byte* payload, uint32_t length)
{
  String msg;
  String topic = t;
  digitalWrite(LED, LOW);

  //Mount string from payload
  for (uint32_t i = 0; i < length; i++)
    msg += (char)payload[i];

  MQTT_Handler(topic, msg);

  if (topic == RESET)
  {
    ESP.restart();
  }
  digitalWrite(LED, HIGH);
}

void initMQTT()
{
  MQTT.setServer(MQTT_BROKER, MQTT_PORT);   //informa qual broker e porta deve ser conectado
  MQTT.setCallback(mqttCallback);    //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}

void MQTT_Whatchdog()
{
  if (!MQTT.connected())
  {
#ifdef DEBUG
    Serial.print("Trying to connect to MQTT Broker: ");
    Serial.println(MQTT_BROKER);
#endif

    if  (MQTT.connect(CHIP_ID, MQTT_USER, MQTT_PASSWORD)) //if umprotected use (MQTT.connect(MAC))
    {

#ifdef DEBUG
      Serial.println("Successfully Connected to MQTT Broker!");
#endif

      MQTT.subscribe(RESET, 1);

#ifdef TOPIC_SUB1
      MQTT.subscribe(TOPIC_SUB1, 1);
#endif
#ifdef TOPIC_SUB2
      MQTT.subscribe(TOPIC_SUB2, 1);
#endif
#ifdef TOPIC_SUB3
      MQTT.subscribe(TOPIC_SUB3, 1);
#endif
#ifdef TOPIC_SUB4
      MQTT.subscribe(TOPIC_SUB4, 1);
#endif
#ifdef TOPIC_SUB5
      MQTT.subscribe(TOPIC_SUB5, 1);
#endif
#ifdef TOPIC_SUB6
      MQTT.subscribe(TOPIC_SUB6, 1);
#endif
#ifdef TOPIC_SUB7
      MQTT.subscribe(TOPIC_SUB7, 1);
#endif
#ifdef TOPIC_SUB8
      MQTT.subscribe(TOPIC_SUB8, 1);
#endif
#ifdef TOPIC_SUB9
      MQTT.subscribe(TOPIC_SUB9, 1);
#endif
#ifdef TOPIC_SUB10
      MQTT.subscribe(TOPIC_SUB10, 1);
#endif
#ifdef TOPIC_SUB11
      MQTT.subscribe(TOPIC_SUB11, 1);
#endif
#ifdef TOPIC_SUB12
      MQTT.subscribe(TOPIC_SUB12, 1);
#endif
      mqttConnectionTimeStamp = millis();
    }
#ifdef DEBUG
    else
      Serial.println("Failed to Connect to MQTT Broker");
#endif
  }
}

void servicesSetup()
{
#ifdef DEBUG
  Serial.begin(2000000, SERIAL_8N1, SERIAL_TX_ONLY); //115200 (or faster) is recommended.
  while (!Serial);  // Wait a bit for the serial connection to be establised.
#endif

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  initWiFi();
  initMQTT();
  ESP.wdtEnable(WDTO_2S);
}

void servicesLoop()
{
  esp8266::polledTimeout::periodic static services_timer(100);
  if (services_timer)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      ArduinoOTA.handle();    // OTA Wrapper
      if (MQTT.connected())
        MQTT.loop();            // MQTT I/O
      else
        MQTT_Whatchdog();       // MQTT Whatchdog
    }
  }
  delay(1);
}
void serviceDelay(uint64_t delay)

{
  uint64_t start = millis();
  while (delay > millis() - start)
    servicesLoop();
}
