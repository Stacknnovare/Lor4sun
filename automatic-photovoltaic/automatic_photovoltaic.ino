#include "ESP32Servo.h"

#define LDRDC 4
#define LDRDB 26
#define LDREC 2
#define LDREB 14

int LDC = 0;
int LEC = 0;
int LDB = 0;
int LEB = 0;

int tol = 0;
int ValorSup = 0;
int ValorInf = 0;
int DifSupInf = 0;

int ServoVertical = 0;     

int LimiteServoVerticalMax = 180;    
int LimiteServoVerticalMin = 0;     

#define servo_pin 15

Servo Vertical;

void setup()
{
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  Vertical.setPeriodHertz(50);
  Vertical.attach(servo_pin, 1000, 2000);
  Serial.begin(115200);
  delay(3000);
}

void loop() {
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

}
