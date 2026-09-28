#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---------------- LCD ----------------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------------- Hall Sensor ----------------
#define HALL_PIN 2       // Interrupt pin on Arduino UNO

// ---------------- Headlights ----------------
#define YELLOW_LAMP 8
#define WHITE_LED 9

// ---------------- Motors ----------------
#define MOTOR1_EN 5
#define MOTOR1_IN1 6
#define MOTOR1_IN2 7

#define MOTOR2_EN 10
#define MOTOR2_IN1 11
#define MOTOR2_IN2 12

// ---------------- Speed settings ----------------
const float WHEEL_DIAMETER = 0.20;  // 20 cm wheel
const int SPEED_THRESHOLD = 40;

// Hall sensor variables
volatile unsigned long pulseCount = 0;

unsigned long previousMillis = 0;
float speedKmph = 0;

// ------------------------------------------------
// Hall sensor interrupt
// ------------------------------------------------
void hallSensor()
{
  pulseCount++;
}

// ------------------------------------------------
// Setup
// ------------------------------------------------
void setup()
{
  Serial.begin(9600);

  // LCD
  lcd.init();
  lcd.backlight();

  // Hall sensor
  pinMode(HALL_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(HALL_PIN), hallSensor, FALLING);

  // Headlights
  pinMode(YELLOW_LAMP, OUTPUT);
  pinMode(WHITE_LED, OUTPUT);

  // Motor pins
  pinMode(MOTOR1_EN, OUTPUT);
  pinMode(MOTOR1_IN1, OUTPUT);
  pinMode(MOTOR1_IN2, OUTPUT);

  pinMode(MOTOR2_EN, OUTPUT);
  pinMode(MOTOR2_IN1, OUTPUT);
  pinMode(MOTOR2_IN2, OUTPUT);

  // Start motors
  digitalWrite(MOTOR1_IN1, HIGH);
  digitalWrite(MOTOR1_IN2, LOW);

  digitalWrite(MOTOR2_IN1, HIGH);
  digitalWrite(MOTOR2_IN2, LOW);

  // Motor speed
  analogWrite(MOTOR1_EN, 180);
  analogWrite(MOTOR2_EN, 180);

  // Initial display
  lcd.setCursor(0, 0);
  lcd.print("Speed Warning");
  lcd.setCursor(0, 1);
  lcd.print("System Ready");

  delay(2000);
  lcd.clear();
}

// ------------------------------------------------
// Main loop
// ------------------------------------------------
void loop()
{
  unsigned long currentMillis = millis();

  // Calculate speed every 1 second
  if (currentMillis - previousMillis >= 1000)
  {
    previousMillis = currentMillis;

    // Temporarily disable interrupt
    noInterrupts();
    unsigned long pulses = pulseCount;
    pulseCount = 0;
    interrupts();

    // ------------------------------------------
    // Calculate wheel revolutions per second
    // ------------------------------------------
    float revolutionsPerSecond = pulses / 1.0;

    // Wheel circumference
    float circumference = 3.14159 * WHEEL_DIAMETER;

    // Distance travelled per second in meters
    float distancePerSecond = revolutionsPerSecond * circumference;

    // Convert m/s to km/h
    speedKmph = distancePerSecond * 3.6;

    // ------------------------------------------
    // Display speed
    // ------------------------------------------
    lcd.setCursor(0, 0);
    lcd.print("Speed: ");
    lcd.print(speedKmph, 1);
    lcd.print(" km/h ");

    // ------------------------------------------
    // Speed-based headlight control
    // ------------------------------------------

    if (speedKmph < SPEED_THRESHOLD)
    {
      // LOW SPEED
      digitalWrite(YELLOW_LAMP, HIGH);
      digitalWrite(WHITE_LED, LOW);

      lcd.setCursor(0, 1);
      lcd.print("LOW: YELLOW     ");
    }
    else
    {
      // HIGH SPEED
      digitalWrite(YELLOW_LAMP, LOW);
      digitalWrite(WHITE_LED, HIGH);

      lcd.setCursor(0, 1);
      lcd.print("HIGH: WHITE     ");
    }

    // Serial monitor
    Serial.print("Speed: ");
    Serial.print(speedKmph);
    Serial.println(" km/h");
  }
}