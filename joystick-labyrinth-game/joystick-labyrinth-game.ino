#include <Servo.h>
#include <LiquidCrystal.h> // for lcd
#include <QTRSensors.h> // for sensor
#include "pitches.h"

#define VRX_PIN  A0 // Arduino pin connected to VRX pin
#define VRY_PIN  A1 // Arduino pin connected to VRY pin
int JOYSTICK_BUTTON_PIN = 2; // Arduino pin connected to button pushed pin

const int rs = 52, en = 50, d4 = 53, d5 = 51, d6 = 49, d7 = 47; // pin nums for lcd
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); // created lcd object

QTRSensors qtr; // created reflective sensor object
const uint8_t SensorCount = 1; // 1 bc we only have 1 sensor
uint16_t sensorValues[SensorCount];

int xVal = 0; // To store value of the X axis
int yVal = 0; // To store value of the Y axis

Servo s0;
Servo s1;

int pos0 = 0;
int pos1 = 0;

unsigned long currentTime = 0;
unsigned long offset = 0;

int ct = 0; // counter for how many cycles the button is held for

int melody[] = {
  E5, E5, E5, C5, E5, E5, G5, G5, C5, C5
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 8, 8, 12, 8, 8, 4, 4
};

void setup() {
  s0.attach(8);
  s1.attach(9);
  Serial.begin(9600);

  pinMode(JOYSTICK_BUTTON_PIN, INPUT); // for joystick click

  lcd.begin(16, 2); // LCD's number of columns and rows

  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A8}, SensorCount);
  qtr.setEmitterPin(2);
}

void loop() {
  if (ct >= 5000) {
    Serial.println("button pressed");
    ct = 0;
    resetTimer();
  }

  xVal = analogRead(VRX_PIN);
  yVal = analogRead(VRY_PIN);
  // Serial.print("xVal: ");
  // Serial.println(xVal);
  // Serial.print("yVal: ");
  // Serial.println(yVal);
  // delay(100);

  if (xVal > 482 && xVal < 542) {
    xVal = 512;
  }
  if (yVal > 482 && yVal < 542) {
    yVal = 512;
  }

  pos0 = map(xVal, 0, 1012, 87, 93);
  pos1 = map(yVal, 0, 1012, 87, 93);

  s0.write(pos0);
  s1.write(pos1);

  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  // print the number of seconds since reset:
  currentTime = millis() - offset;
  lcd.print(currentTime / 1000);

  Serial.println(digitalRead(2));
  delay(100);
  if (digitalRead(JOYSTICK_BUTTON_PIN) == 0) {
    ct += 1;
  } else {
    ct = 0;
  }

  // read raw sensor values
  qtr.read(sensorValues);
  // Serial.println(sensorValues[0]);
  // delay(100);
  // print the sensor values as numbers from 0 to 1023, where 0 means maximum
  // reflectance and 1023 means minimum reflectance
  if (sensorValues[0] < 200) {
    gameWin();
  } else {
    lcd.setCursor(0, 1);
    if (currentTime < 10) {
      lcd.print("MOVE THE BALL!");
    }
    if (currentTime >= 10) {
      lcd.print("HURRY UP!");
    }
  }
}

void gameWin() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("YOU WON!");
  for (int thisNote = 0; thisNote < 10; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(8, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(8);
  }
}

void resetTimer() {
  offset = millis();
}
