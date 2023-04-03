#include <SPI.h>
#include <LoRa.h>


#define LORA_SS_PIN 6
#define LORA_RST_PIN 7
#define LORA_DIO0_PIN 2
#define LORA_FREQUENCY 433E6
#define LORA_BANDWIDTH 125E3
#define LORA_SPREADING_FACTOR 7
#define LORA_CODING_RATE 5
#define LORA_SYNC_WORD 0x12

void setup()
{
  Serial.begin(9600);
  LoRa.setPins(LORA_SS_PIN, LORA_RST_PIN, LORA_DIO0_PIN);
  if (!LoRa.begin(LORA_FREQUENCY))
  {
    Serial.println("LoRa initialization failed!!!!");
    while (1);
  }
  LoRa.setSignalBandwidth(LORA_BANDWIDTH);
  LoRa.setSpreadingFactor(LORA_SPREADING_FACTOR);
  LoRa.setCodingRate4(LORA_CODING_RATE);
  LoRa.setSyncWord(LORA_SYNC_WORD);
  Serial.println("LoRa Repeater Ready.");
}

void loop()
{
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    String gpsData = "";
    while (LoRa.available())
    {
      gpsData += (char)LoRa.read();
    }
    Serial.println("Received GPS Data: " + gpsData);
    LoRa.beginPacket();
    LoRa.print(gpsData);
    LoRa.endPacket();
    Serial.println("Forwarding GPS Data.");
  }
}
