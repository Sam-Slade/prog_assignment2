// Include header files
#include <Keypad.h> //4x4 keypad header file
#include <Servo.h> //Servo motor header file
#include <SPI.h>
#include <LiquidCrystal.h>

//Servo motor setup
const int SERVO_PIN = 2;
Servo Servo1;

//Keypad setup code
const byte ROWS = 4;
const byte COLS = 4;

char keymap[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}};

byte rowPins[ROWS] = {12,8,7,6};
byte colPins[COLS] = {5,4,3,2};

Keypad keypad = Keypad(makeKeymap(keymap), rowPins, colPins, ROWS, COLS);

//Buzzer Setup code
void buzzer_success(); // Login Success buzzer tone
void buzzer_fail(); // Login Fail buzzer tone
const int BUZZER = 10; // Set buzzer pin

// lcd screen
LiquidCrystal lcd(9);

//User input class
class userCode {
  
  //Class variables
  public:
    int code[4];
    int point;

  //Set up the variables within the class
  void set () {
    code[0,3] = NULL;
    point = 0;
  }

  //Enter a new digit
  void enterDigit (int key) {
    code[point] = key;
  }

  //Clear the last digit entered
  void clearLast () {
    if (point != 0) {
      point -= 1;
      code[point] = NULL;
    }
  }

};

void setup() {
  Serial.begin(9600);
  //Set buzzer pin to output
  pinMode(BUZZER, OUTPUT);

  //Set servo pin to output
  Servo1.attach(SERVO_PIN);
  Servo1.writeMicroseconds(180);

  //Declare struct for user code
  ::userCode code;
  code.set();
    
  //Add event listener to the keypad
  //to catch user input
  //keypad.addEventListener(keypadEvent);
}


void loop() {
  char key = keypad.getKey();

  if (key) {
    lcd.setCursor(0,0);
    lcd.print(key);
    buzzer_success();
  }
}

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
