#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <SPI.h>
#include <LoRa.h>


#define GPS_RX_PIN 10
#define GPS_TX_PIN 11
#define GPS_BAUD_RATE 9600


#define LORA_SS_PIN 6
#define LORA_RST_PIN 7
#define LORA_DIO0_PIN 2
#define LORA_FREQUENCY 915E6
#define LORA_BANDWIDTH 125E3
#define LORA_SPREADING_FACTOR 7
#define LORA_CODING_RATE 5
#define LORA_SYNC_WORD 0x12

SoftwareSerial gpsSerial(GPS_RX_PIN, GPS_TX_PIN);
TinyGPS gps;

void setup()
{
  Serial.begin(9600);
  gpsSerial.begin(GPS_BAUD_RATE);
  LoRa.setPins(LORA_SS_PIN, LORA_RST_PIN, LORA_DIO0_PIN);
  if (!LoRa.begin(LORA_FREQUENCY))
  {
    Serial.println("Check your connections.");
    while (1);
  }
  LoRa.setTxPower(20, PA_OUTPUT_PA_BOOST_PIN);
  LoRa.setSignalBandwidth(LORA_BANDWIDTH);
  LoRa.setSpreadingFactor(LORA_SPREADING_FACTOR);
  LoRa.setCodingRate4(LORA_CODING_RATE);
  LoRa.setSyncWord(LORA_SYNC_WORD);
}

void loop()
{
  float lat, lng;
  unsigned long age;

  while (gpsSerial.available())
  {
    if (gps.encode(gpsSerial.read()))
    {
      gps.f_get_position(&lat, &lng, &age);
      Serial.print("Latitude: ");
      Serial.println(lat, 6);
      Serial.print("Longitude: ");
      Serial.println(lng, 6);

      String gpsData = String(lat, 6) + "," + String(lng, 6);
      LoRa.beginPacket();
      LoRa.print(gpsData);
      LoRa.endPacket();
      delay(1000);
    }
  }
}
