// Include header files
#include <Keypad.h> //4x4 keypad header file
#include <Servo.h> //Servo motor header file
#include <SPI.h>
#include <LiquidCrystal.h>

//Hidden code input setup
char multiplyChar(char character, int multiplier);

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
    char code[4];
    int point;

  private:
    //User codes
    char user1[4];
    char user2[4];
    char user3[4];
    char user4[4];
    
    //Admin code
    char admin[4];

  //Return the length of the code
  public int len() {
    return point;
  }

  //Set up the variables within the class
  public void set () {
    code[0,3] = NULL;
    point = 0;
  }

  //Enter a new digit
  public void enterDigit (char key) {
    if (point < 3) {
      code[point] = key;
      point += 1;
    }
  }

  //Clear the last digit entered
  public void clearLast () {
    if (point != 0) {
      point -= 1;
      code[point] = NULL;
    }
  }

  //Check admin
  public bool checkAdmin() {
    if (code == admin) {
      return 1;
    } else {
      return 0;
    }
  }

  //Check the code entered by the user
  public bool checkCode() {
    if (code == user1 || code == user2 || code == user3 || code == user4) {
      return 1;  
    } else {
      return 0;
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
  lcd.setCursor(0,1);
  lcd.print("Press # to clear!");

  char key = keypad.getKey();

  if (key == "#") {
    code.set();
    lcd.clear();

  } else if (key) {
    lcd.setCursor(0,0);
    lcd.print("Enter code: ");
    code.enterDigit(key);
    lcd.print(multipleChar("*", code.pointer + 1));
    buzzer_tap();
    if (code.len() == 3) {
      if (code.checkCode()) {
        buzzer_success();
        code.set();
      } else {
        buzzer_fail();
        code.set();
      }
    }
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

// Return multiple of the same character
char multiplyChar(char character, int multiplier) {
  char outputString[multiplier];

  for (int i=0; i<multiplier; i++) {
    outputString[i] = character;
  }
  return outputString;
}
