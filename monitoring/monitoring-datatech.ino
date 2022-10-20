#include <Buffer.h>
#include <RoboCore_SMW_SX1276M0.h>


#include <HardwareSerial.h>
HardwareSerial LoRaSerial(2);
#define RXD2 16
#define TXD2 17


SMW_SX1276M0 lorawan(LoRaSerial);


void setup() {
  Serial.begin(115200);
  Serial.println(F("--- SMW_SX12760M Bridge ---"));

  lorawan.setPinReset(5);
  lorawan.reset();

  LoRaSerial.begin(115200, SERIAL_8N1, RXD2, TXD2);

}

void loop() {
  if(LoRaSerial.available()){
    Serial.write(LoRaSerial.read());
  }

  if(Serial.available()){
    LoRaSerial.write(Serial.read());
  }

}
