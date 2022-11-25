#include "ESP32Servo.h"

#define LDRDC 26
#define LDRDB 27
#define LDREC 14
#define LDREB 12

int LDC = 0;
int LEC = 0;
int LDB = 0;
int LEB = 0;

int tol = 0;
int ValorSup = 0;
int ValorInf = 0;
int DifSupInf = 0;

int ServoVertical = 90;     

int LimiteServoVerticalMax = 120;    
int LimiteServoVerticalMin = 15;     

#define servo_pin 2

Servo Vertical;

void setup()
{
  Vertical.attach(servo_pin);
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
      if (ServoVertical > LimiteServoVerticalMax)  {
        ServoVertical = LimiteServoVerticalMax;
      }
    }
    else if (ValorSup < ValorInf)  {
      ServoVertical = --ServoVertical;
      if (ServoVertical < LimiteServoVerticalMin)  {
        ServoVertical = LimiteServoVerticalMin;
      }
    }
    Vertical.write(ServoVertical);
  }

}
