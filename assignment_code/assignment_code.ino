/*==== Header files ====*/
#include <Keypad.h> // Keypad header file
#include <Servo.h> // Servo motor header file
#include <SPI.h> // SPI to all for talking to the lcd using a shift register
#include <LiquidCrystal.h> // LCD library header file

/*==== Servo Setup ====*/
// Set servo pin to be digital pin 2
int servoPin = 2;

// Servo object
Servo servo;

// Declare the two servo functions used
void open_door();
void close_door();

/*==== Keypad setup code ====*/
// The keypad dimensions
const byte ROWS = 4;
const byte COLS = 3;

// Keypad array of buttons
char keymap[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}};

// The digital pins used by the keypad
byte rowPins[ROWS] = {12,8,7,6};
byte colPins[COLS] = {5,4,3};

// Create keypad object
Keypad keypad = Keypad(makeKeymap(keymap), rowPins, colPins, ROWS, COLS);

/*==== LCD ====*/
// LCD object, attack the data pin to pin 9
LiquidCrystal lcd(9);

/*==== Buzzer tone function declaration ===*/
// Buzzer pin
const int BUZZER = 10;

// Declare the three functions used by the buzzer
void buzzer_success();
void buzzer_fail();
void buzzer_tap();

/*==== Pin code storage ====*/
// An array of pointers to the pin codes
char** pinCodeArray = new char*[10];

// Array used to store user input
char* input = new char[4];

// Value to keep track of which digit has been entered into the "input" array
int pointer = 0;

/*==== Pin code function declaration ====*/
// Clear the user input code
void clear_code();

// Check to see if user input is a number
bool is_num(char key);

// Hide the user input with *'s
void hide_input();

// Check to see if the code is valid
bool check_code(int code);

// The administrator menu function
int admin_menu();

/*==== Main arduino functions ====*/
// Arduino setup function, runs when the arduino starts
void setup() {
  // Allow for listening to the serial bus
  Serial.begin(9600);
  
  // Set LCD Screen dimensions
  lcd.begin(16,2);

  // Set the buzzer pin to output
  pinMode(BUZZER, OUTPUT);

  // Give a pointer to an empty array to each of the elements
  // in the pincode array
  for (int i=0; i<10; i++) {
    pinCodeArray[i] = new char[4];
  }

  // Set each array element to be NULL
  for(int i=0; i<10; i++) {
    for (int j=0; j<4; j++) {
      pinCodeArray[i][j] = NULL;
    }
  }

  // Setup admin code, always stored in the 0 element of the
  // pin code array
  for (int i=0; i<4; i++){
    char int_str [1];

    // itoa converts an int (i in base 10) to a string and stores 
    // it in int_str
    itoa(i, int_str, 10);
    pinCodeArray[0][i] = int_str[0];
  }
}

void loop() {
  delay(200);

  // Clear the LCD and display the menu text
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("# to clear!");
  lcd.setCursor(0,0);
  lcd.print("Enter code: ");

  // Hide the users input
  hide_input();

  // If the user has entered 4 digits
  if (pointer == 4) {
    pointer = 0;

    // Check the users code against each of the codes in
    // the pin code array
    for (int i=0; i<10; i++) {

      // If the code is valid...
      if (check_code(i)) {
        
        // The 0 code is the admin code
        if (i == 0) {
          
          // Make success tone and display success message
          buzzer_success();
          login_success();

          // Open the admin menu
          admin_menu();
          break;

        // If is user code
        } else {

          // Make success tone and display success message
          buzzer_success();
          login_success();

          // Open the door with the servo
          open_door();
          delay(2000);

          // Clse the door with the servo
          close_door();
          delay(2000);
        }
      }
    }
  }

  // Get key pressed by the user
  char key = keypad.getKey();

  // If the # key is pressed
  if (key == '#') {
    
    // Make fail tone
    buzzer_fail();

    // Clear the code entered by the user
    clear_code();

  // If a key is pressed
  } else if (key) {
    
    // Make tap sounds
    buzzer_tap();

    // If the key pressed is a numeric key
    if (is_num(key)) {
      
      // Add the key to the input array
      input[pointer] = key;

      // increment the pointer
      pointer++;
    }
  }
}

/*==== Pin code function definition ====*/
// Clear the code entered by the user
void clear_code() {
  for (int i=0; i<4; i++) {
    input[i] = "";
  }
  pointer = 0;
}

// Check to see if key pressed is a number
bool is_num(char key) {
  
  // loop through numbers 0-9
  for (int i=0; i<10; i++) {
    
    // int_str is used to store the int converted to str
    char int_str [1];

    // Convert int to string
    itoa(i, int_str, 10);

    // If valid, return true
    if (key == int_str[0]){
      return 1;
    }
  }

  // If the for loop ends, then key isn't a number
  // return false
  return 0;
}

// Print a * to the lcd for ever key entered into 
// the user code
void hide_input() {
  if (pointer != 0) {
    for (int i=0; i<pointer; i++) {
      lcd.print("*");
    }
  }
}

// Check to see if the code entered by the user is valid
// parameter is the code to check the user code against
bool check_code (int code) {
  
  // In this function the code is valid until it is proven
  // to be false
  int valid = 1;
  for (int i=0; i<4 ; i++) {
    
    // If the digits don't match, set valid to false
    if (pinCodeArray[code][i] != input[i]) {
      valid = 0;
    }
  }
  return valid;
}

// Function gives feedback to the user to show a
// successful login
void login_success () {

  // Clear the lcd and write message to screen
  lcd.clear();
  lcd.print("### Success ###");
  delay(1000);
}

// Function gives feedback to the user to show a
// failed login
void login_fail () {

  // Clear the lcd and write message to screen
  lcd.clear();
  lcd.print("### Failed ###");
  delay(1000);
}

// The administrator menu function is where the user
// can edit codes used by the system
int admin_menu () {
  
  // Valid is used to check the new code entered in the admin menu
  // is valid
  int valid = 0;
  
  // First while loop for the admin menu system 
  while (1) {
    
    // Clear the LCD and display the admin menu ui
    lcd.clear();
    lcd.print("press 1:EDIT");
    lcd.setCursor(0,1);
    lcd.print("2: EXIT MENU");

    // Listen for user key input
    int key = keypad.getKey();

    // Exit the admin menu
    if (key == '2'){
      buzzer_tap();
      break;

    // If key is one open menu to allow for code editing
    } else if (key == '1') {
      buzzer_tap();

      // While loop for entering new code to edit
      while (1) {
        
        // Clear lcd and ask for which code to edit
        lcd.clear();
        lcd.print("Enter user code");
        lcd.setCursor(0,1);
        lcd.print("number to edit: ");

        // Listen for user input
        char key = keypad.getKey();

        // If the user input is a number
        if (is_num(key)) {
          buzzer_tap();

          // Convert the key pressed into an int
          int edit_code = (int)key - '0';
          Serial.print("edit code: ");
          Serial.print(edit_code);
          Serial.print("\n");

          // New code array and pointer
          char* new_code = new char[4];
          int new_code_pointer = 0;

          // New code repeat and pointer
          char* repeat_code = new char[4];
          int repeat_code_pointer = 0;

          // loop to enter the new code and new code repeat
          while (1) {
            
            // If both codes have been entered
            if (new_code_pointer == 4 and repeat_code_pointer == 4) {

              // Check to see if both codes are the same
              valid = 1;
              for (int i=0; i<4; i++) {
                if (new_code[i] != repeat_code[i]) {
                  valid = 0;
                }
              } 

              // If both codes are the same, save the new code to the pin
              // code array
              if (valid) {
                Serial.print("i: ");
                for(int i=0; i<4; i++) {
                  Serial.print(i);
                  pinCodeArray[edit_code][i] = new_code[i];
                }

                // Clear the memory of the new code and new code repeat arrays
                delete[] new_code;
                delete[] repeat_code;

                // CHECK
                Serial.print("\n");
                for (int i=0; i<10; i++) {
                  Serial.print(i);
                  Serial.print(": ");
                  for (int j=0;j<4; j++) {
                    Serial.print(pinCodeArray[i][j]);
                  }
                  Serial.print("\n");
                }
                Serial.print("Exit");
                return 1;
              }

              // If the code wasn't valid, clear the screen and display message
              lcd.clear();
              lcd.print("Invalid code!");
              delay(1000);
              break;
            }
            
            lcd.clear();

            // New code entry and hidden user entry
            lcd.print("new code: ");
            for (int i=0; i<new_code_pointer; i++) {
              lcd.print("*");
            }

            // Repeat code entry and hidden user entry
            lcd.setCursor(0,1);
            lcd.print("repeat  : ");
            Serial.print(repeat_code_pointer);
            for (int i=0; i<repeat_code_pointer; i++) {
              lcd.print("*");
            }

            // Get user input
            char key = keypad.getKey();

            // If the input is a number
            if (is_num(key)) {
              buzzer_tap();

              // If 4 digits haven't been entered into the new code
              if (new_code_pointer < 4) {
                
                // Store the user input into the new code and increment the new pointer
                new_code[new_code_pointer] = key;
                new_code_pointer++;

              // If 4 digits haven't been entered into the repeat code
              } else if (repeat_code_pointer < 4) {
                
                // Store the user input into the repeat code and increment the repeat pointer
                repeat_code[repeat_code_pointer] = key;
                repeat_code_pointer++;
              }
            }
            delay(200);
          }
        }
        delay(200);
      }
    }
    delay(200);
  }
}

/*==== Servo Function definition ====*/
// Function used to control servo to open the door
void open_door() {
  
  // Attach the servo to the servo pin to allow
  // for data to be sent to the servo
  servo.attach(servoPin);

  // Clear the lcd and write to it
  lcd.clear();
  lcd.print("Unlocked");

  // Set the servo to the open position
  servo.write(0);

  // Wait and then detach the servo
  delay(1000);
  servo.detach();
}

// Function used to control servo to close the door
void close_door() {
  
  // Attach the servo to the servo pin to allow
  // for data to be sent to the servo
  servo.attach(servoPin);

  // Set the servo to the closed postion
  servo.write(180);

  // Wait and detach the servo
  delay(1000);
  servo.detach();

  // Clear the lcd and write to it
  lcd.clear();
  lcd.print("Locked");
  delay(1000);
}

/*==== Buzzer tone function definition ====*/
// Function to make the success tone with the buzzer
void buzzer_success() {
  tone(BUZZER, 2500);
  delay(200);
  noTone(BUZZER);
}

// Function to make the fail tone with the buzzer
void buzzer_fail() {
  tone(BUZZER, 450);
  delay(200);
  noTone(BUZZER);
}

// Function to make the tapping sounds of button pressing
// with the buzzer
void buzzer_tap() {
  tone(BUZZER, 2500);
  delay(10);
  noTone(BUZZER);
}
