// Authors: Sofia Gratny, Jake Herweg, Domminic Mayer
// Purpose: Final Project- Cpe 301
// Date: 5/9/23


#include <LiquidCrystal.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Stepper.h>

// Define constants and pins
#define DHTPIN 7
#define DHTTYPE DHT11
#define TEMP_MAX 78
const int BUTTON_PIN = 2;
const int MOTOR_PIN = 18;
const int FAN_PIN = 20;
const int RED_PIN = 14;
const int GREEN_PIN = 15;
const int BLUE_PIN = 16;

// Define variables
bool buttonPressed;
int lastButtonState;
int currentButtonState;

// Initialize objects
LiquidCrystal lcd(3, 4, 5, 6, 12, 13);
DHT dht(DHTPIN, DHTTYPE);
const int STEPS_PER_REVOLUTION = 32;
Stepper myStepper(STEPS_PER_REVOLUTION, 8, 9, 10, 11);

// Define registers
volatile unsigned char* myUCSR0A = (unsigned char*)0x00C0;
volatile unsigned char* myUCSR0B = (unsigned char*)0x00C1;
volatile unsigned char* myUCSR0C = (unsigned char*)0x00C2;
volatile unsigned int* myUBRR0 = (unsigned int*)0x00C4;
volatile unsigned char* myUDR0 = (unsigned char*)0x00C6;
volatile unsigned char* my_ADMUX = (unsigned char*)0x7C;
volatile unsigned char* my_ADCSRB = (unsigned char*)0x7B;
volatile unsigned char* my_ADCSRA = (unsigned char*)0x7A;
volatile unsigned int* my_ADC_DATA = (unsigned int*)0x78;

// Setup function
void setup() {
  // Set up serial communication and ADC
  U0init(9600);
  adc_init();

  // Set up pins
  pinMode(BUTTON_PIN, INPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(MOTOR_PIN, INPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // Set up stepper motor
  myStepper.setSpeed(200);

  // Set up LCD display
  lcd.begin(16, 2);
  lcd.setCursor(7, 1);
  lcd.print("---");

  // Set up DHT sensor
  dht.begin();

  // Set up green light and ADC
  DDRB |= (1 << 5);

  // Display initial text on LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.setCursor(0, 1);
  lcd.print("Humid:");
  delay(1500);
}

// Main loop function
void loop() {
  // Check if button is pressed
  if (digitalRead(BUTTON_PIN) == HIGH) {
    buttonPressed = true;
    delay(2000);
  }

  // While button is pressed
  while (buttonPressed) {
    // Turn on lights and motor
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(BLUE_PIN, HIGH);
    digitalWrite(RED_PIN, LOW);
    digitalWrite(FAN_PIN, LOW);
    motor_move();

    // Check if button is pressed again to stop
    if (digitalRead(BUTTON_PIN) == HIGH) {
      buttonPressed = false;
    }
  }

  // Set green light
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, LOW);
  digitalWrite(RED_PIN, LOW);

  // Read ADC and move motor if necessary
  unsigned
