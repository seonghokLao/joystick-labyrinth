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

// int idleTune[] = {
//   E5, G5, E5, G5, C5, C5, A5, C5, D5, E5, C5, E5, G5, E5, G5, C5,
// };

// // note durations: 4 = quarter note, 8 = eighth note, etc.:
// int idleTuneDurations[] = {
//   4, 8, 4, 4, 2, 16, 16, 8, 8, 8, 8, 4, 8, 4, 4, 1.5
// };

int startTune[] = {
  A4, A4, A4, A5
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
float startTuneDurations[] = {
  1.5, 1.5, 1.5, 1.5
};

// int winTune[] = {
//   E5, E5, E5, C5, E5, E5, G5, G5, C6, C6
// };

// // note durations: 4 = quarter note, 8 = eighth note, etc.:
// int winTuneDurations[] = {
//   8, 4, 4, 4, 4, 3, 4, 4, 8, 8
// };

int winTune[] = {
  G3, C4, E4, G4, C5, E5, G5, E5, GS3, C4, DS4, GS4, C5, DS5, GS5, DS5, AS3, D4, F4, AS4, D5, F5, AS5, AS5, AS5, AS5, C6
};

int winTuneDurations[] = {
  9, 9, 9, 9, 9, 9, 3, 3, 9, 9, 9, 9, 9, 9, 3, 3, 9, 9, 9, 9, 9, 9, 3, 9, 9, 9, 3
};

bool ready = false;

bool needClear = true;

bool printed = false;

bool transitioned = false;

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
  // if (ct >= 5000) {
  //   // Serial.println("button pressed");
  //   ct = 0;
  //   resetTimer();
  // }

  xVal = analogRead(VRX_PIN);
  yVal = analogRead(VRY_PIN);
  // Serial.print("xVal: ");
  // Serial.println(xVal);
  // Serial.print("yVal: ");
  // Serial.println(yVal);
  // delay(100);
  if (!ready) {
    s0.write(90);
    s1.write(90);
    if (!printed) {
      lcd.print("Move Joystick");
      lcd.setCursor(0, 1);
      lcd.print("To Start.");
      printed = true;
    }
    if (xVal < 50 || xVal > 950 || yVal < 50 || yVal > 950) {
      gameStart();
    }
  }

  if (ready) {
    // Serial.println("game started");
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

    // Serial.println(digitalRead(2));
    // delay(100);
    // if (digitalRead(JOYSTICK_BUTTON_PIN) == 0) {
    //   ct += 1;
    // } else {
    //   ct = 0;
    // }

    // read raw sensor values
    qtr.read(sensorValues);
    // Serial.println(sensorValues[0]);
    // delay(100);
    // print the sensor values as numbers from 0 to 1023, where 0 means maximum
    // reflectance and 1023 means minimum reflectance
    currentTime = (millis() - offset)/1000;
    if (sensorValues[0] < 180) {
      needClear = true;
      printed = false;
      gameWin();
    } else {
      if (!printed) {
        lcd.setCursor(0, 0);
        lcd.print("Time: ");
        lcd.setCursor(0,1);
        lcd.print("MOVE THE BALL!");
      }
      gameInProgress();
    }
  }
}

void gameStart() {
  ready = true;
  delay(500);
  for (int i = 0; i < 4; i++) {
    lcd.clear();
    lcd.setCursor(7, 0);
    if (i == 3) {
      lcd.print("GO!!!");
    } else {
      lcd.print(3 - i);
    }
    int noteDuration = 1000 / startTuneDurations[i];
    tone(2, startTune[i], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(2);
  }
  delay(100);
  resetTimer();
  printed = false;
}

void gameWin() {
  ready = false;
  s0.write(100);
  s1.write(90);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  lcd.print(currentTime);
  lcd.setCursor(0, 1);
  lcd.print("YOU WON!");
  for (int i = 0; i < 27; i++) {
    if (i % 3 == 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Time: ");
      lcd.print(currentTime);
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Time: ");
      lcd.print(currentTime);
      lcd.setCursor(0, 1);
      lcd.print("YOU WON!");
    }
    int noteDuration = 1000 / winTuneDurations[i];
    tone(2, winTune[i], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(2);
  }
  s0.write(90);
  delay(3000);
  lcd.clear();
}

void gameInProgress() {
  if (needClear) {
    lcd.clear();
    needClear = false;
  }
  if (!printed) {
    lcd.print("Time: ");
    lcd.setCursor(0,1);
    if (currentTime < 30) {
      lcd.print("MOVE THE BALL!");
    }
    if (currentTime >= 30) {
      lcd.print("HURRY UP!");
    }
    printed = true;
  }
  if (currentTime == 30 && !transitioned) {
    transitioned = true;
    needClear = true;
    printed = false;
  }
  lcd.setCursor(6, 0);
  // print the number of seconds since reset:
  lcd.print(currentTime);
  
}

void resetTimer() {
  offset = millis();
}
