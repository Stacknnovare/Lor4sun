/**
 * Copyright (c) 2022 Data Tech Team
 * Copyright (c) 2022 Gabriel Correa <gabrielcorreasb@protonmail.com>
 */


// BIBLIOTECAS
#include "RoboCore_SMW_SX1276M0.h"
#include "HardwareSerial.h"
#include "ArduinoJson.h"
#include "ESP32Servo.h"

// BATERIA 
#define A_BAT 13
#define CONV_FACTOR 4.623
int valorBat = 0;
int valorReal = 0;
int valorDC = 0;


// LEDS
#define LED_VERDE 23
#define LED_VERMELHO 32
#define LED_BRANCO 25
#define LED_AZUL 27
#define LED_AMARELO 12

//LORA
#define RXD2_DIN 17
#define TXD2_DOUT 16

//LDR
#define LDRDC 4
#define LDRDB 26
#define LDREC 2
#define LDREB 14

int LDC = 0;
int LEC = 0;
int LDB = 0;
int LEB = 0;


//MOTOR
#define servo_pin 15
int tol = 0;
int ValorSup = 0;
int ValorInf = 0;
int DifSupInf = 0;
int ServoVertical = 0;     
int LimiteServoVerticalMax = 180;    
int LimiteServoVerticalMin = 0;

Servo Vertical;

const unsigned long motorInterval = 1000;
unsigned long motorTimer;

HardwareSerial LoRaSerial(2);
SMW_SX1276M0 lorawan(LoRaSerial);
const char APP_EUI[] = "7e23cfde31a18ea9";
const char APP_KEY[] = "28cccbddd5240b543f1b119def25c8a2";

CommandResponse response;
const unsigned long PAUSE_TIME = 15000;
unsigned long timeout;

void event_handler(Event);

// Placa automatica
void motor() {
  LDC = analogRead(LDRDC);      
  LEC = analogRead(LDREC);
  LDB = analogRead(LDRDB);      
  LEB = analogRead(LDREB);
  
  tol = 50;

  ValorSup = (LDC + LEC) / 2;   
  ValorInf = (LDB + LEB) / 2;   

  DifSupInf = ValorSup - ValorInf;      

  if (-1 * tol > DifSupInf || DifSupInf > tol)  {
    if (ValorSup > ValorInf)  {
      ServoVertical = ++ServoVertical;
      delay(30);
      if (ServoVertical > LimiteServoVerticalMax)  {
        ServoVertical = LimiteServoVerticalMax;
      }
    }
  
    else if (ValorSup < ValorInf)  {
      ServoVertical = --ServoVertical;
      delay(30);
      if (ServoVertical < LimiteServoVerticalMin)  {
        ServoVertical = LimiteServoVerticalMin;
      }
    }
    Vertical.write(ServoVertical);
  }
  motorTimer = millis();
}
//Função Send Data
void send_data(int Valor){
  DynamicJsonDocument json(JSON_OBJECT_SIZE(1));

  json["B"] = Valor;

  String payload = "";
  serializeJson(json, payload);

  lorawan.sendT(1, payload);
  }
// Informações Bateria
int check_battery(){
 
 while(true) { 
  valorReal = ((3.3 * 3853)/4095 ) * CONV_FACTOR;
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
  // Iniciando Motor
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  Vertical.setPeriodHertz(50);
  Vertical.attach(servo_pin, 1000, 2000);
  
  //Iniciando Leds
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_BRANCO, OUTPUT);
  motorTimer = millis();

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
    response = lorawan.set_JoinMode(SMW_SX1276M0_JOIN_MODE_OTAA);
    if(response == CommandResponse::OK){
      blink_green();
      connection = !connection;
     
    } else {
      blinking_red();
      }
  }

  lorawan.join();
    
}

void loop() {

  if ((millis() - motorTimer) >= motorInterval) {
    motor();
    motorTimer = millis();
    }
  
  valorDC = check_battery();
    
  lorawan.listen();

  if(lorawan.isConnected()){
    if(timeout < millis()){
      send_data(valorDC); // NEED TO CREATE
      timeout = millis() + PAUSE_TIME;
      green();
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
    green();
  }
}
