#include <LiquidCrystal.h>

// LCD pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Speed potentiometer
const int speedPot = A0;

// LEDs
const int yellowLED = 8;
const int whiteLED = 9;

// Motor driver L293D
const int motorEnable = 10;
const int motorIN1 = 7;
const int motorIN2 = 6;

void setup()
{
  // LCD
  lcd.begin(16, 2);

  // LED pins
  pinMode(yellowLED, OUTPUT);
  pinMode(whiteLED, OUTPUT);

  // Motor pins
  pinMode(motorEnable, OUTPUT);
  pinMode(motorIN1, OUTPUT);
  pinMode(motorIN2, OUTPUT);

  // Motor ON
  digitalWrite(motorEnable, HIGH);
  digitalWrite(motorIN1, HIGH);
  digitalWrite(motorIN2, LOW);

  // Initial LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SpeedCue System");
  delay(1500);
  lcd.clear();
}

void loop()
{
  // Read potentiometer
  int potValue = analogRead(speedPot);

  // Convert POT value to speed
  int speed = map(potValue, 0, 1023, 0, 100);

  // Display speed
  lcd.setCursor(0, 0);
  lcd.print("Speed: ");
  lcd.print(speed);
  lcd.print(" km/h ");

  // Speed-based lighting
  if (speed < 40)
  {
    // LOW SPEED
    digitalWrite(yellowLED, HIGH);
    digitalWrite(whiteLED, LOW);

    lcd.setCursor(0, 1);
    lcd.print("Mode: YELLOW    ");
  }
  else
  {
    // HIGH SPEED
    digitalWrite(yellowLED, LOW);
    digitalWrite(whiteLED, HIGH);

    lcd.setCursor(0, 1);
    lcd.print("Mode: WHITE     ");
  }

  // Motor continuously ON
  digitalWrite(motorEnable, HIGH);
  digitalWrite(motorIN1, HIGH);
  digitalWrite(motorIN2, LOW);

  delay(100);
}
