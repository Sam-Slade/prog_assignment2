/*==== Header files ====*/
#include <Keypad.h>
#include <Servo.h>
#include <SPI.h>
#include <LiquidCrystal.h>

/*==== Keypad setup code ====*/
const byte ROWS = 4;
const byte COLS = 4;

char keymap[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}};

byte rowPins[ROWS] = {12,8,7,6};
byte colPins[COLS] = {5,4,3,2};

Keypad keypad = Keypad(makeKeymap(keymap), rowPins, colPins, ROWS, COLS);

/*==== Buzzer tone function declaration ===*/
const int BUZZER = 10; // Buzzer pin
void buzzer_success();
void buzzer_fail();
void buzzer_tap();

/*==== Pin code storage ====*/
char* user1 = new char[4];
char* user2 = new char[4];
char* user3 = new char[4];
char* user4 = new char[4];
char* admin = new char[4];

int pointer = 0;

/*==== Main arduino functions ====*/
void setup() {
  Serial.begin(9600);
}

void loop() {
  char key = keypad.getKey();
}

/*==== Buzzer tone function definition ====*/
void buzzer_success() {
  tone(BUZZER, 2500);
  delay(200);
  noTone(BUZZER);
}

void buzzer_fail() {
  tone(BUZZER, 450);
  delay(200);
  noTone(BUZZER);
}

void buzzer_tap() {
  tone(BUZZER, 2500);
  delay(10);
  noTone(BUZZER);
}
