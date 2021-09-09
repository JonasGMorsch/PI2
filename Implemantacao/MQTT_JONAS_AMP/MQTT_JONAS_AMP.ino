#define HOST_NAME "MQTT_JONAS_AMP" //OTA NAME
#define DTIM 10 // 0 FULL ON - 1 LESS LATENCY - 10 LESS POWER
//define DEBUG

//////////////////////////////////// MQTT SUB TOPICS ////////////////////////////////////
#define TOPIC_SUB1  "test1"
//////////////////////////////////// MQTT PUB TOPICS ////////////////////////////////////
#define TOPIC_PUB1  "test2"
/////////////////////////////////////////////////////////////////////////////////////////

#include "JONAS_MQTT.h"  //MY LIBRARY WRAPER
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <ir_Samsung.h>

#define VOLUME_SCL_BASS D1
#define VOLUME_SCL_MAIN D2
#define VOLUME_SDA    D3

#define SCL D5
#define SDA D6

#define IR_SENDER   D7
#define IR_RECORDER D9

// VARIABLES & GLOBAL OBJECTS
void setVolume(uint32_t);
void setDataFM(uint32_t, uint32_t, uint32_t);

uint32_t amp_volume = 45;
uint32_t sub_level_adj = 10;
uint32_t amp_mute_control;
const uint32_t amp_mute_volume = 13;

//IRrecv IR_REC(IR_RECORDER, 128, 5, false); //TEST THIS!!!
IRrecv IR_REC(IR_RECORDER);
IRsend IR_SEND(IR_SENDER);  // An IR LED at this pin
IRSamsungAc IR_SEND_AC(IR_SENDER);
decode_results results;  // Somewhere to store the results

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

void IR_Loop()
{
  //delay(50);
  digitalWrite(LED, LOW);
  switch (results.value)
  {
    case 0xE0E0E01F:
      amp_volume++;
      /*amp_volume = constrain(amp_volume, 0, 83);
        MQTT.publish(TOPIC_SUB3, String(amp_volume).c_str(), true);*/
      setVolume(amp_volume);
      break;

    case 0xE0E0D02F:
      amp_volume--;
      /*amp_volume = constrain(amp_volume, 0, 83);
        MQTT.publish(TOPIC_SUB3, String(amp_volume).c_str(), true);*/
      setVolume(amp_volume);
      break;

    case 0xE0E0F00F:
      if (amp_mute_control)
      {
        setVolume(amp_mute_volume);
      }
      else
      {
        setVolume(amp_volume);
      }
      amp_mute_control = !amp_mute_control;
      delay(10);
      break;
    default:
      //MQTT.publish("mqtt/debug", (String("0x") + String((uint32_t)results.value, HEX)).c_str() , false);
      //MQTT.publish("mqtt/debug", (String("0x") + String((uint32_t)results.value, HEX).toUpperCase()).c_str() , false);

      /*String hex = String((uint32_t)results.value, HEX);
        hex.toUpperCase();
        MQTT.publish("mqtt/debug", (String("0x") + hex).c_str() , false);*/
      break;
  }
  digitalWrite(LED, HIGH);
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
  IR_REC.setUnknownThreshold(32); // MINIMUM READABLE BYTE SET
  IR_REC.enableIRIn();  // Start the receiver
  IR_SEND.begin(); // Start the sender
}

void loop()
{
  if (IR_REC.decode(&results)) // Check if the IR code has been received.
  {
    IR_REC.resume();  // Receive the next value
    IR_Loop();
  }
  
  servicesLoop();
}

// this function does the job
void setVolume(uint32_t v)
{
  v = min(v, 83U);
  setDataFM(v, VOLUME_SCL_MAIN, VOLUME_SDA);

  v += sub_level_adj - 10;
  v = min(v, 83U);
  setDataFM(v, VOLUME_SCL_BASS, VOLUME_SDA);
}
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
