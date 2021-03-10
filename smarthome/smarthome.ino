// Set a template to easy print operations
template <typename T>
Print& operator<<(Print& printer, T value) {
  printer.print(value);
  return printer;
}

// initialize libraries
#include <Servo.h>
#include <Keypad.h>
//#include <SPI.h>
//#include <Ethernet.h>

const byte rows = 4;
const byte cols = 4;

// Pin Config
const byte soundpin = 13;
const byte ultraechopin = 6;
const byte ultratriggerpin = 5;

// Variable Config
int pin = 0;
int len;

const String password = "1742"; // Change the debug veriable to the password you want
String password_input;
 int password_trys = 0;


//define the cymbols on the buttons of the keypads
char hexaKeys[rows][cols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[rows] = {9, 8, 7, 6};
byte colPins[cols] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, rows, cols); //initialize an instance of class NewKeypad

Servo servo; // initialize servo object

// function for playing a system starting sound
void playSound() {
    tone(soundpin, 330, 250);
    delay(500);
    noTone(soundpin);
    delay(30);
    tone(soundpin, 500, 250);
    delay(300);
    noTone(soundpin);
    delay(500);
}

void setup() {

  //Pin Config
  pinMode(soundpin, OUTPUT);
  servo.attach(10);
  
  playSound(); // Give a soundoutput for starting
  
  Serial.begin(9600); // Start Serial Monitor on port 9600
  Serial.flush(); // Waits for the transmission of outgoing serial data to complete

  password_input.reserve(8); // Maximum input characters 8
  
  Serial << '\n' << "Smarthome is running now" << '\n';
  Serial << "Set the house password with four numbers" << '\n';

  servo.attach(2); // attaches the servo on GPIO2 to the servo object
}

void loop() {
    char key = keypad.getKey();

    if (key) {
      

      if (key == '*') {
        password_input = ""; // Reset the password input
        Serial << '\n' << "Reset the password input" << '\n';
      } else if (key == 'D') {
        if (password == password_input) {
          Serial << '\n' << "Password is correct. Access granted" << '\n';

          servo.write(90);
          
          // After Checking password
        } else if (password_trys == 5) {
          Serial << '\n' << "You have entered the wrong password too many times. You can try again in 5 minutes." << '\n';
          delay(5 * 60000); // 5 minutes delay
        } else {
          Serial << '\n' << "Password is incorrect, try again" << '\n';
          password_trys++;
        }

        password_input = ""; // Clear the password input
      } else {
        Serial << "*";
        password_input += key; // Adds a new character to the input
      }
    }
}
