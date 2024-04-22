
#include <LiquidCrystal.h> // for lcd
#include <QTRSensors.h> // for sensor

const int rs = 52, en = 53, d4 = 48, d5 = 50, d6 = 51, d7 = 49; // pin nums for lcd
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); // created lcd object

QTRSensors qtr; // created reflective sensor object
const uint8_t SensorCount = 1; // 1 bc we only have 1 sensor
uint16_t sensorValues[SensorCount];

enum GameState {
  BEGIN_STATE,
  IN_GAME_STATE
};

GameState currState = BEGIN_STATE;



void setup() {
  lcd.begin(16, 2); // LCD's number of columns and rows
  // lcd.print("hello, world!");

  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A1}, SensorCount);
  qtr.setEmitterPin(2);

  Serial.begin(9600);
}



void loop() {

  switch(currState) {
    case BEGIN_STATE:
      // code for starting new game
      Serial.println("we are in begin state");
      lcd.print("Welcome to Labyrinth");
    
    case IN_GAME_STATE:
      Serial.println("in, in game state");

      if (sensorValues[0] < 200) {
        lcd.setCursor(0, 1);
        lcd.print("you won");
        delay(1200);
      }
  }








  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);
  lcd.print("Score: ");
  // print the number of seconds since reset:
  // lcd.print(millis() / 1000);

  // read raw sensor values
  qtr.read(sensorValues);

  // print the sensor values as numbers from 0 to 1023, where 0 means maximum
  // reflectance and 1023 means minimum reflectance
  if (sensorValues[0] < 200) {
    lcd.setCursor(0, 1);
    lcd.print("you won");
    delay(1200);
  } else {
    lcd.setCursor(0, 1);
    lcd.print("in game");
  }

  Serial.println();

  delay(10);









}

