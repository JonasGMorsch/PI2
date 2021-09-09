#define HOST_NAME "MQTT_JONAS_AMP" //OTA NAME
#define DTIM 10 // 0 FULL ON - 1 LESS LATENCY - 10 LESS POWER
//define DEBUG

//////////////////////////////////// MQTT SUB TOPICS ////////////////////////////////////
#define TOPIC_SUB1  "house/jonasbedroom/tv/switch/status"
#define TOPIC_SUB2  "house/jonasbedroom/tv/control"

#define TOPIC_SUB3  "house/jonasbedroom/sound/volume/0t100"
#define TOPIC_SUB4  "house/jonasbedroom/sound/mute/switch"

#define TOPIC_SUB5  "house/jonasbedroom/ac/mode/0t4"
#define TOPIC_SUB6  "house/jonasbedroom/ac/fan/0t7"
#define TOPIC_SUB7  "house/jonasbedroom/ac/swing/switch"
#define TOPIC_SUB8  "house/jonasbedroom/ac/temperature/16t30"
#define TOPIC_SUB9  "house/jonasbedroom/ac/beep/switch"
#define TOPIC_SUB10 "house/jonasbedroom/ac/power/switch"
//////////////////////////////////// MQTT PUB TOPICS ////////////////////////////////////
#define TOPIC_PUB_TEMP  "house/jonasbedroom/bedroom/temperature/status"
#define TOPIC_PUB_HUM   "house/jonasbedroom/bedroom/humidity/status"
/////////////////////////////////////////////////////////////////////////////////////////

#include "JONAS_MQTT.h"  //MY LIBRARY WRAPER
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <ir_Samsung.h>
#include <GY21.h>

#define VOLUME_SCL_BASS D1
#define VOLUME_SCL_MAIN D2
#define VOLUME_SDA    D3

#define SCL D5
#define SDA D6

#define IR_SENDER   D7
#define IR_RECORDER D9

// PROTOTYPES
void setVolume(uint32_t);
void setDataFM(uint32_t, uint32_t, uint32_t);
void Temp_Check(void);

// VARIABLES & GLOBAL OBJECTS
GY21 sensor;
uint32_t amp_volume = 45;
uint32_t sub_level_adj = 10; // Sub Volume is the same as the main volume
uint32_t amp_mute_control;
const uint32_t amp_mute_volume = 13;

//IRrecv IR_REC(IR_RECORDER, 128, 5, false); //TEST THIS!!!
IRrecv IR_REC(IR_RECORDER);
IRsend IR_SEND(IR_SENDER);  // An IR LED at this pin
IRSamsungAc IR_SEND_AC(IR_SENDER);
decode_results results;  // Somewhere to store the results

void MQTT_Handler(String topic, String msg)
{
  //DEVICES STATUS
  static bool tvpower = 0; //INITIAL STATE

  if (topic == TOPIC_SUB1)
  {
    if (tvpower == 0 && msg.toInt() == 1)
    {
      tvpower = 1;
    }
    else if (tvpower == 1 && msg.toInt() == 0)
    {
      tvpower = 0;
    }
  }

  else if (topic == TOPIC_SUB2)
  {
    if (msg == "power" || msg ==  "0" || msg ==  "1")
      IR_SEND.sendSAMSUNG(0xE0E040BF); // POWER
    else if (msg == "source")
      IR_SEND.sendSAMSUNG(0xE0E0807F); // SOURCE
    else if (msg == "channelup")
      IR_SEND.sendSAMSUNG(0xE0E048B7); // CH +
    else if (msg == "channeldown")
      IR_SEND.sendSAMSUNG(0xE0E008F7); // CH -
    else if (msg == "content")
      IR_SEND.sendSAMSUNG(0xE0E09E61); // CONTENT
    else if (msg == "menu")
      IR_SEND.sendSAMSUNG(0xE0E058A7); // MENU
    else if (msg == "guide")
      IR_SEND.sendSAMSUNG(0xE0E0F20D); // GUIDE
    else if (msg == "tools")
      IR_SEND.sendSAMSUNG(0xE0E0D22D); // TOOLS
    else if (msg == "info")
      IR_SEND.sendSAMSUNG(0xE0E0F807); // INFO
    else if (msg == "enter")
      IR_SEND.sendSAMSUNG(0xE0E016E9); // ENTER
    else if (msg == "up")
      IR_SEND.sendSAMSUNG(0xE0E006F9); // UP
    else if (msg == "down")
      IR_SEND.sendSAMSUNG(0xE0E08679); // DOWN
    else if (msg == "left")
      IR_SEND.sendSAMSUNG(0xE0E0A659); // LEFT
    else if (msg == "right")
      IR_SEND.sendSAMSUNG(0xE0E046B9); // RIGHT
    else if (msg == "return")
      IR_SEND.sendSAMSUNG(0xE0E01AE5); // RETURN
    else if (msg == "exit")
      IR_SEND.sendSAMSUNG(0xE0E0B44B); // EXIT
    else if (msg == "back")
      IR_SEND.sendSAMSUNG(0xE0E0A25D); // BACK
  }
  else if (topic == TOPIC_SUB3) //VOLUME
  {
    amp_volume = msg.toInt();
    setVolume(amp_volume);
    MQTT.publish(TOPIC_SUB3 "/status", String(amp_volume).c_str(), true);
  }

  else if (topic == TOPIC_SUB4)  //MUTE
  {
    //static uint32_t amp_volume_temp;

    if (msg.toInt())
      setVolume(amp_mute_volume);
    else
      setVolume(amp_volume);

    MQTT.publish(TOPIC_SUB4 "/status", msg.c_str(), true);
    MQTT.publish(TOPIC_SUB3 "/status", String(amp_volume).c_str(), true);
  }
  else if (topic == TOPIC_SUB5)
  {
    if (msg.toInt() >= 0 && msg.toInt() <= 4)
    {
      MQTT.publish(TOPIC_SUB5 "/status", msg.c_str(), true);
      IR_SEND_AC.setMode(msg.toInt());

      if (IR_SEND_AC.getPower())
        IR_SEND_AC.send();
    }
  }

  else if (topic == TOPIC_SUB6)
  {
    if (msg.toInt() >= 0 && msg.toInt() <= 7)
    {
      MQTT.publish(TOPIC_SUB6 "/status", msg.c_str(), true);
      IR_SEND_AC.setFan(msg.toInt());

      if (IR_SEND_AC.getPower())
        IR_SEND_AC.send();
    }
  }

  else if (topic == TOPIC_SUB7)
  {
    MQTT.publish(TOPIC_SUB7 "/status", msg.c_str(), true);
    //    if ()
    //      IR_SEND_AC.setSwing(1);
    //    else
    //      IR_SEND_AC.setSwing(0);

    IR_SEND_AC.setSwing(msg.toInt());

    if (IR_SEND_AC.getPower())
      IR_SEND_AC.send();
  }

  else if (topic == TOPIC_SUB8)
  {
    if (msg.toInt() >= 16 && msg.toInt() <= 30)
    {
      MQTT.publish(TOPIC_SUB8 "/status", msg.c_str(), true);
      IR_SEND_AC.setTemp(msg.toInt());
      if (IR_SEND_AC.getPower())
        IR_SEND_AC.send();
    }
  }
  else if (topic == TOPIC_SUB9) // BEEP
  {
    MQTT.publish(TOPIC_SUB9 "/status", msg.c_str(), true);
    //    if (IR_SEND_AC.getPower())
    //      if (msg.toInt())
    //        IR_SEND_AC.setIon(1);
    //      else
    //        IR_SEND_AC.setIon(0);
  }
  else if (topic == TOPIC_SUB10)
  {
    MQTT.publish(TOPIC_SUB10 "/status", msg.c_str(), true);
    if (msg.toInt())
    {
      IR_SEND_AC.setPower(1);
      IR_SEND_AC.send();
    }
    else
    {
      IR_SEND_AC.setPower(0);
      IR_SEND_AC.sendOff();
      //IR_SEND_AC.send(); // NOT WORKING
    }
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
  pinMode(VOLUME_SCL_MAIN, OUTPUT);
  pinMode(VOLUME_SCL_BASS, OUTPUT);
  pinMode(VOLUME_SDA, OUTPUT);

  servicesSetup();

  IR_REC.setUnknownThreshold(32); // MINIMUM READABLE BYTE SET
  IR_REC.enableIRIn();  // Start the receiver
  IR_SEND.begin(); // Start the sender
  IR_SEND_AC.begin(); // Start AC sender
  IR_SEND_AC.setIon(1); // VIRUS DOCTOR
  IR_SEND_AC.setPower(0);

  Wire.TwoWire::begin(SDA, SCL);
  Wire.TwoWire::setClock(400000);
}

void loop()
{
  if (IR_REC.decode(&results)) // Check if the IR code has been received.
  {
    IR_REC.resume();  // Receive the next value
    IR_Loop();
  }

  esp8266::polledTimeout::periodic static temp_timer(10000);
  if (temp_timer)
    Temp_Check();

  esp8266::polledTimeout::periodic static amp_volume_timer(250);
  if (amp_volume_timer)
  {
    static uint32_t amp_volume_temp;
    if (amp_volume != amp_volume_temp)
    {
      amp_volume_temp = amp_volume;
      MQTT.publish(TOPIC_SUB4 "/status", "0", true); // UNMUTE
      MQTT.publish(TOPIC_SUB3, String(amp_volume).c_str(), true);
    }
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

void Temp_Check(void)
{
  static float published_temp;
  static float published_hum;
  float hum = sensor.GY21_Humidity();
  float temp = sensor.GY21_Temperature();

  if (roundf(published_temp * 10.0) != roundf(temp * 10.0))
  {
    MQTT.publish(TOPIC_PUB_TEMP, (String(temp, 1)).c_str(), true);
    published_temp = roundf(temp * 10.0) / 10.0;
  }

  if (roundf(published_hum * 10.0) != roundf(hum * 10.0))
  {
    MQTT.publish(TOPIC_PUB_HUM, (String(hum, 1)).c_str(), true);
    published_hum = roundf(hum * 10.0) / 10.0;
  }
  if (IR_SEND_AC.getPower())
  {
    esp8266::polledTimeout::periodic static ac_send(300000); // resend ac value every 5 minutes
    if (ac_send)
      IR_SEND_AC.send();
  }
}
