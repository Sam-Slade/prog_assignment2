// Include header files
#include <Keypad.h> //4x4 keypad header file
#include <Servo.h> //Servo motor header file

//Servo motor setup
const int SERVO_PIN = 9;
Servo Servo1;

//Keypad setup code
const int ROWS = 4;
const int COLS = 4;

char keymap[ROWS][COLS] = {{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}};

byte rowPins[ROWS] = {7,6,5,4};
byte colPins[COLS] = {3,2,1,0};

Keypad keypad = Keypad(makeKeymap(keymap), rowPins, colPins, ROWS, COLS);

//Buzzer Setup code
void buzzer_success(); // Login Success buzzer tone
void buzzer_fail(); // Login Fail buzzer tone
const int BUZZER = 10; // Set buzzer pin


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
  //Set buzzer pin to output
  pinMode(BUZZER, OUTPUT);

  //Set servo pin to output
  Servo1.attach(SERVO_PIN);

  //Declare struct for user code
  ::userCode code;
  code.set();
    
  //Add event listener to the keypad
  //to catch user input
  keypad.addEventListener(userInput);
}


void loop() {
}

void buzzer_success() {
  tone(BUZZER, 2500);
  delay(500);
  noTone(BUZZER);
}

void buzzer_fail() {
  tone(BUZZER, 450);
  delay(500);
  noTone(BUZZER);
}



//Listen for user input and store it
void userInput(KeypadEvent key, userCode code){
  switch (keypad.getState()) {
  case PRESSED:
    if (key == 'c') {
      code.set();
    } else {
      code.enterDigit(key);
    }
  }
}
