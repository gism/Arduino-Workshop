#include <Arduino.h>
#include <Servo.h>

#define DELAY_SERVOS_MOVE 50
int ByteReceived;

Servo servo_m1;
Servo servo_m2;
Servo servo_m3;
Servo servo_m4;

int sensorPinX1 = A0;
int sensorPinY1 = A1;
int sensorPinX2 = A2;
int sensorPinY2 = A3;

int sensorValueX1 = 0;
int sensorValueY1 = 0;
int sensorValueX2 = 0;
int sensorValueY2 = 0;

int pwm_m1 = 100;
int pwm_m2 = 100;
int pwm_m3 = 100;
int pwm_m4 = 100;

void setup()
{
  Serial.begin(115200);
  Serial.println("--- Joystick controlled ARM v0.0---");
  Serial.println();

  servo_m1.attach(11);
  servo_m2.attach(10);
  servo_m3.attach(9);
  servo_m4.attach(8);
}

void check_joystic()
{
  sensorValueX1 = analogRead(sensorPinX1);
  sensorValueY1 = analogRead(sensorPinY1);
  sensorValueX2 = analogRead(sensorPinX2);
  sensorValueY2 = analogRead(sensorPinY2);

  int val = 0;
  if ((sensorValueX1>550)||(sensorValueX1<450)){
    val = map(sensorValueX1, 20, 1000, 2, -2);
    pwm_m1 = pwm_m1 + val;
    if (pwm_m1<0) pwm_m1 = 0;
    if (pwm_m1>255) pwm_m1 = 255;
  }
  servo_m1.write(pwm_m1);

  if ((sensorValueY1>550)||(sensorValueY1<450)){
    val = map(sensorValueY1, 20, 1000, 2, -2);
    pwm_m2 = pwm_m2 + val;
    if (pwm_m2 < 0) pwm_m2 = 0;
    if (pwm_m2>255) pwm_m2 = 255;
  }
  servo_m2.write(pwm_m2);

  if ((sensorValueY2 > 550)||(sensorValueY2 < 450)){
    val = map(sensorValueY2, 20, 1000, 2, -2);
    pwm_m3 = pwm_m3 + val;
    if (pwm_m3 < 20) pwm_m3 = 20;
    if (pwm_m3 > 120) pwm_m3 = 120;
  }
  servo_m3.write(pwm_m3);

  if ((sensorValueX2 > 550)||(sensorValueX2 < 450)){
    val = map(sensorValueX2, 20, 1000, 2, -2);
    pwm_m4 = pwm_m4 + val;
    if (pwm_m4 < 20) pwm_m4 = 20;
    if (pwm_m4 > 120) pwm_m4 = 120;
  }
  servo_m4.write(pwm_m4);

  delay(DELAY_SERVOS_MOVE);
}

void report_status(){

  Serial.print("X1:\t");
  Serial.print(sensorValueX1);
  Serial.print("\tY1:\t");
  Serial.print(sensorValueY1);
  Serial.print("\tX2:\t");
  Serial.print(sensorValueX2);
  Serial.print("\tY2:\t");
  Serial.println(sensorValueY2);

  Serial.print("PWM_M1:\t");
  Serial.print(pwm_m1);
  Serial.print("\tPWM_M2:\t");
  Serial.print(pwm_m2);
  Serial.print("\tPWM_M3:\t");
  Serial.print(pwm_m3);
  Serial.print("\tPWM_M4:\t");
  Serial.println(pwm_m4);
  Serial.println("------------------------------");

}

void loop()
{
  check_joystic();
  report_status();
}
