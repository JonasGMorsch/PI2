#define HOST_NAME "TESTNODE" //OTA NAME
#define DTIM 10 // 0 FULL ON - 1 LESS LATENCY - 10 LESS POWER
#include "JONAS_MQTT.h"  //MY LIBRARY WRAPER
//define DEBUG

//////////////////////////////////// MQTT SUB TOPICS ////////////////////////////////////
#define TOPIC_SUB1  "test1"
//////////////////////////////////// MQTT PUB TOPICS ////////////////////////////////////
#define TOPIC_PUB1  "test2"
/////////////////////////////////////////////////////////////////////////////////////////

#define VOLUME_SCL_BASS D1
#define VOLUME_SCL_MAIN D2
#define VOLUME_SDA    D3

#define SCL D5
#define SDA D6

#define IR_SENDER   D7
#define IR_RECORDER D9

// VARIABLES & GLOBAL OBJECTS
void setDataFM(uint32_t, uint32_t, uint32_t);

void MQTT_Handler(String topic, String msg)
{
  if (topic == "blink")
  {
    if (msg == "blink")
    {
      delay (100);
    }
  }
  
  else if (topic == TOPIC_SUB1) //VOLUME
  {
    int amp_volume = msg.toInt();
    setDataFM(amp_volume, VOLUME_SCL_MAIN, VOLUME_SDA);
    MQTT.publish(TOPIC_SUB1 "/status", String(amp_volume).c_str(), true);
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

// this function does the job
void setDataFM (uint32_t volume, uint32_t serial_clock, uint32_t serial_data)
{
  //pinMode(serial_data, OUTPUT);
  //uint32_t bits;
  uint32_t data = 0; // control word is built by OR-ing in the bits

  // convert attenuation to volume
  volume = (volume > 83) ? 0 : (-volume  + 83); // remember 0 is full volume!
  // generate 10 bits of data
  //data |= (0 << 0); // D0 (channel select: 0=ch1, 1=ch2)
  data |= (0 << 1); // D1 (individual/both select: 0=both, 1=individual)
  data |= ((21 - (volume / 4)) << 2); // D2...D6 (ATT1: coarse attenuator: 0,-4dB,-8dB, etc.. steps of 4dB)
  data |= ((3 - (volume % 4)) << 7); // D7...D8 (ATT2: fine attenuator: 0...-1dB... steps of 1dB)
  data |= (3 << 9); // D9...D10 // D9 & D10 must both be 1

  for (uint32_t bits = 0; bits < 11; bits++) // send out 11 control bits
  {
    digitalWrite (serial_data, 0); // pg.4 - M62429P/FP datasheet
    delayMicroseconds (2);
    digitalWrite (serial_clock, 0);
    delayMicroseconds (2);
    digitalWrite (serial_data, (data >> bits) & 0x01);
    delayMicroseconds (2);
    digitalWrite (serial_clock, 1);
    delayMicroseconds (2);
  }
  delayMicroseconds (2);
  digitalWrite (serial_data, 1);
  delayMicroseconds (2);
  digitalWrite (serial_clock, 0); //final clock latches data in
}
