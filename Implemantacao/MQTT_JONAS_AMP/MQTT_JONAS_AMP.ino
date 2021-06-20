#define HOST_NAME "TESTNODE" //OTA NAME
#define DTIM 10 // 0 FULL ON - 1 LESS LATENCY - 10 LESS POWER
#include "lala.h"  //MY LIBRARY WRAPER
//define DEBUG

//////////////////////////////////// MQTT SUB TOPICS ////////////////////////////////////
#define TOPIC_SUB1  "test1"
//////////////////////////////////// MQTT PUB TOPICS ////////////////////////////////////
#define TOPIC_PUB1  "test2"
/////////////////////////////////////////////////////////////////////////////////////////

void MQTT_Handler(String topic, String msg)
{
  if (topic == TOPIC_SUB1)
  {
    if (msg == "blink")
    {
      delay (100);
    }
  }
}

void setup()
{
  pinMode(D0, INPUT_PULLUP);
  pinMode(D1, INPUT_PULLUP);
  pinMode(D2, INPUT_PULLUP);
  pinMode(D3, INPUT_PULLUP);
  // D4 RESERVED FOR LED
  pinMode(D5, INPUT_PULLUP);
  pinMode(D6, INPUT_PULLUP);
  pinMode(D7, INPUT_PULLUP);
  pinMode(D8, INPUT_PULLUP);
  pinMode(D9, INPUT_PULLUP);
  pinMode(D10, INPUT_PULLUP);

  servicesSetup();
  initMQTT();
}

void loop()
{
  servicesLoop();
}
