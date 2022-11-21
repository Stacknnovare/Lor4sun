/**
 * Copyright (c) 2022 Data Tech Team
 * Copyright (c) 2022 Gabriel Correa <gabrielcorreasb@protonmail.com>
 */


// BIBLIOTECAS
#include "RoboCore_SMW_SX1276M0.h"
#include "HardwareSerial.h"
#include "ArduinoJson.h"

// BATERIA 
#define A_BAT 13
#define CONV_FACTOR 4.623
int valorBat = 0;
int valorReal = 0;
int valorDC = 0;


// LEDS
#define LED_VERDE 27
#define LED_VERMELHO 32
#define LED_BRANCO 25
#define LED_AZUL 27
#define LED_AMARELO 12

//LORA
#define RXD2_DIN 17
#define TXD2_DOUT 16
HardwareSerial LoRaSerial(2);
SMW_SX1276M0 lorawan(LoRaSerial);
const char APP_EUI[] = "7e23cfde31a18ea9";
const char APP_KEY[] = "28cccbddd5240b543f1b119def25c8a2";

CommandResponse response;
const unsigned long PAUSE_TIME = 15000;
unsigned long timeout;

void event_handler(Event);

// Informações Bateria
int check_battery(){
 
 while(true) { 
  valorBat = analogRead(A_BAT);
  valorReal = ((3.3 * valorBat)/4095 ) * CONV_FACTOR;
  delay(1000);
  
  return valorReal;
  }  
}

// Funcionamento LEDS
void green() {
  digitalWrite(LED_VERDE, HIGH);
}

void fatal_red() {
  digitalWrite(LED_VERMELHO, HIGH);
}

void blinking_red() {
   digitalWrite(LED_VERMELHO, HIGH);
   delay(1000);
   digitalWrite(LED_VERMELHO, LOW);
   delay(1000);   
}

void blink_white() {
  digitalWrite(LED_BRANCO, HIGH);
  delay(1000);
  digitalWrite(LED_BRANCO, LOW);
}

void blink_yellow() {
  digitalWrite(LED_AMARELO, HIGH);
  delay(1000);
  digitalWrite(LED_AMARELO, LOW);
}

void blink_blue() {
  digitalWrite(LED_AZUL, HIGH);
  delay(1000);
  digitalWrite(LED_AZUL, LOW);
}

void blink_green() {
   digitalWrite(LED_VERDE, HIGH);
   delay(1000);
   digitalWrite(LED_VERDE, LOW);
}

// Configuração
void setup() {
  Serial.begin(115200);
  //Iniciando Leds
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_BRANCO, OUTPUT);

  LoRaSerial.begin(115200, SERIAL_8N1, RXD2_DIN, TXD2_DOUT);
  lorawan.event_listener = &event_handler;
  delay(1000);
  lorawan.setPinReset(5);
  lorawan.reset();
  
  char deveui[16];
  
  bool connection = false;
  while (connection == false) {
    response = lorawan.get_DevEUI(deveui);
    if(response == CommandResponse::OK){
      blink_green();
      connection = !connection;
    } else {
      blinking_red();
  }
 }

  connection = false;
  while (connection == false) {
    response = lorawan.set_AppEUI(APP_EUI);
    if(response == CommandResponse::OK){
      blink_green();
      connection = !connection;
    } else {
      blinking_red();
    }
  }

  connection = false;
  while (connection == false) {
    response = lorawan.set_AppKey(APP_KEY);
    if(response == CommandResponse::OK){
      blink_green();
      connection = !connection;
    } else {
      blinking_red();
    }
  }

  connection = false;
  while (connection == false) {
    if(response == CommandResponse::OK){
      response = lorawan.set_JoinMode(SMW_SX1276M0_JOIN_MODE_OTAA);
      blink_green();
      connection = !connection;
     
    } else {
      blinking_red();
      }
  }

  lorawan.join();
    
}

void loop() {

  valorDC = check_battery();
    
  lorawan.listen();

  if(lorawan.isConnected()){
    if(timeout < millis()){
      //send_data(valorDC); // NEED TO CREATE
      timeout = millis() + PAUSE_TIME;
      blink_green();
      } 
    } else {

    if(timeout < millis()){
      blink_white();
      timeout = millis() + 5000; // 5 s     
    }
  }
}
void event_handler(Event type){
  if(type == Event::JOINED){
    blink_blue();
  }
}
