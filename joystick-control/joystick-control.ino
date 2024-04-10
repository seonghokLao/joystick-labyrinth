#include <Servo.h>

#define VRX_PIN  A0 // Arduino pin connected to VRX pin
#define VRY_PIN  A1 // Arduino pin connected to VRY pin

int xVal = 0; // To store value of the X axis
int yVal = 0; // To store value of the Y axis

Servo s0;
Servo s1;

int pos0 = 0;
int pos1 = 0;

void setup() {
  s0.attach(8);
  s1.attach(9);
  Serial.begin(9600);
}

void loop() {

  xVal = analogRead(VRX_PIN);
  yVal = analogRead(VRY_PIN);

  if (xVal > 482 && xVal < 542) {
    xVal = 512;
  }
  if (yVal > 482 && yVal < 542) {
    yVal = 512;
  }

  pos0 = map(xVal, 0, 1023, 87, 93);
  pos1 = map(yVal, 0, 1023, 87, 93);

  s0.write(pos0);
  s1.write(pos1);

  Serial.print("x = ");
  Serial.print(xVal);
  Serial.print(", y = ");
  Serial.println(yVal);
  delay(100);
}