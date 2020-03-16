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
  /*PRIVATE VARS*/
  //User codes
  char user1[4];
  char user2[4];
  char user3[4];
  char user4[4];
  
  //Admin code, this is hard coded and cannot be changed
  //during run time for security
  char admin[4] = {1,2,3,4};

  /*Public*/
  //Class variables
  public:
    char code[4];
    int point;

    //Return the length of the code
    int len() {
      return point;
    }

    //Set up the variables within the class
    void set () {
      code[0,3] = NULL;
      point = 0;
    }

    //Enter a new digit
    void enterDigit (char key) {
      if (point < 3) {
        code[point] = key;
        point += 1;
      }
    }

    //Clear the last digit entered
    void clearLast () {
      if (point != 0) {
        point -= 1;
        code[point] = NULL;
      }
    }

    //Check admin
    bool checkAdmin() {
      if (code == admin) {
        return 1;
      } else {
        return 0;
      }
    }

    //Check the code entered by the user
    bool checkCode() {
      if (code == user1 || code == user2 || code == user3 || code == user4) {
        return 1;  
      } else {
        return 0;
      }
    }

    //Set user code 
    void setUserCode(int user, char code) {
      if (user == 1) {
        user1 = code;
      } else if (user == 2) {
        user2 = code;
      } else if (user == 3) {
        user3 = code;
      } else if (user == 4) {
        user4 = code;
      }
    }
    
    //Clear user code 
    void clearUserCode(int user) {
      if (user == 1) {
        user1[0,3] = NULL;
      } else if (user == 2) {
        user2[0,3] = NULL;
      } else if (user == 3) {
        user3[0,3] = NULL;
      } else if (user == 4) {
        user4[0,3] = NULL;
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
  
  //Clear screen and set cursor to base second line
  //Display 'clear' message
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Press # to clear!");

  //Set cursor the top line and display message to user
  lcd.setCursor(0,0);
  lcd.print("Enter code: ");

  //Get user input
  char key = keypad.getKey();

  //If the # key is press, clear the current user input
  if (key == "#") {
    buzzer_tap();
    code.set();
  } else if (key) {
    //If a key is pressed

    //Add the entered key to the code object
    code.enterDigit(key);

    //Use the multipleChar function to display a * for each character entered
    lcd.print(multipleChar("*", code.pointer + 1));
    buzzer_tap();

    //If a full code is entered, check to see if it's the admin code
    if (code.len() == 3) {
      if (code.checkAdmin) {
        buzzer_success();
        
      } else if (code.checkCode()) {
        // Check to see if code entered is a user code
        // If valid, make usccess tone and display
        // welcome message to user
        buzzer_success();
        code.set();
        lcd.clear();
        lcd.print("### Welcome! ###");
        delay(2000);
      } else {
        // If code is invalid, play fail tone and display
        // invalid code message to user
        buzzer_fail();
        code.set();
        lcd.clear();
        lcd.print("# Invalid Code #");
        delay(2000);
      }
    }
  }
}

// Buzzer tone for success
void buzzer_success() {
  tone(BUZZER, 2500);
  delay(200);
  noTone(BUZZER);
}

// Buzzer tone for failure (I hear this tone a lot)
void buzzer_fail() {
  tone(BUZZER, 450);
  delay(200);
  noTone(BUZZER);
}

// Buzzer tone for a key press
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
