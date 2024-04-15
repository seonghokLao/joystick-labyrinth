
// include the library code:
#include <LiquidCrystal.h> // for lcd
#include <QTRSensors.h> // for sensor

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 53, en = 51, d4 = 44, d5 = 45, d6 = 47, d7 = 46;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

QTRSensors qtr;

const uint8_t SensorCount = 1; // 1 bc we only have 1 sensor
uint16_t sensorValues[SensorCount];

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  // lcd.print("hello, world!");

  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A1}, SensorCount);
  qtr.setEmitterPin(2);

  Serial.begin(9600);
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  // lcd.print(millis() / 1000);

  // read raw sensor values
  qtr.read(sensorValues);

  // print the sensor values as numbers from 0 to 1023, where 0 means maximum
  // reflectance and 1023 means minimum reflectance
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
    if (sensorValues[i] < 200) {
      lcd.print("you won");
      delay(1200);
    } else {
      lcd.print("in game");
    }
  }
  Serial.println();

  delay(10);
}

