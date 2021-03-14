// Set a template to easy print operations
template <typename T>
Print& operator<<(Print& printer, T value) {
  printer.print(value);
  return printer;
}

// initialize libraries
#include <Servo.h>
#include <Keypad.h>
#include <SPI.h>

#include <DHT.h>
#define dhtpin A1
#define dhttype DHT22

DHT dht(dhtpin, dhttype);

// Define the keypad size
const byte rows = 4;
const byte cols = 4;

// Pin Config
const byte soundpin = 13;
const byte movesensor = 12;
const byte ledpin = 11;

// Variable Config
int pin = 0;
int len;
int movestatus = 0;
String nameinput;
int decision;
String oldpassword;
int repasswordlogin = 0;

float temperature;
float humidity;

String password = "1221"; // Change the debug veriable to the password you want
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

// function for changing the current password
void onchangepassword() {
  Serial << "\nEnter the old password\n";
  
  while (!Serial.available()); // Waits until something has been entered 

  oldpassword = Serial.readStringUntil('\n');

  if (oldpassword == password) {
    Serial << "Great! Now please enter your new password\n";

    while (!Serial.available()); // Waits until something has been entered
    
    password = Serial.readStringUntil('\n');

    Serial << "The password was set to " << password << '\n';
    
  } else {
    Serial << "Please enter the password again\n";
    return;
  }
  
}

void setup() {

  //Pin Config
  pinMode(soundpin, OUTPUT);
  pinMode(movesensor, INPUT);
  pinMode(ledpin, OUTPUT);
  
  servo.attach(10); // attaches the servo on GPIO2 to the servo object
  
  playSound(); // Give a soundoutput for starting

  dht.begin(); // Start DHT
  
  Serial.begin(9600); // Start Serial Monitor on port 9600
  Serial.flush(); // Waits for the transmission of outgoing serial data to complete

  password_input.reserve(8); // Maximum input characters 8
  
  Serial << '\n' << "Smarthome is running now" << '\n';
  Serial << "What is your name?" << '\n';
  
  while (Serial.available() == 0);
  
  nameinput = Serial.readStringUntil('\n');
  
  Serial << "Well, " << nameinput << ". Welcome to your Smarthome" << '\n';

  Serial << "The default password of your smarthome is " << password <<".\nPlease log in with this password before changing the default password." << '\n';

  Serial << "Password: ";
}

void loop() {
    char key = keypad.getKey(); 
    
    if (key) {
      
      if (key == '*') { // Reset the password input
        password_input = ""; // Reset the password input
        Serial << '\n' << "Reset the password input" << '\n';
      } else if (key == 'D') { // Submit the password input
        if (password == password_input) {
          Serial << '\n' << "Password is correct. Access granted" << '\n';

          temperature = dht.readTemperature(); // Passes the temperature variable the temperature of the environment
          humidity = dht.readHumidity(); // Passes the humidityature variable the humidity of the environment

          if (isnan(temperature) || isnan(humidity)) { // Checks that the result is not a number
            Serial << "Hey " << nameinput << "\nSomething went wrong with the readout!";
            // I want to use return statement, but the compiler didn't want :(
          } else {
            Serial << "Hey " << nameinput << "\nIt has " << temperature << " degrees Celsius. The humidity is " << humidity;
          }

          
          if(repasswordlogin == 0) { // Checks if the password is changed and therefore the password must be entered again
            // Opens the door
            servo.write(0);
            delay(7000); // 7 seconds delay
            servo.write(90);
          }

          // As soon as the motion sensor detects a movement, the light in the house goes on
          while(movestatus == HIGH) {
            digitalWrite(ledpin, HIGH);
          }

          Serial << '\n' << "(1) - Change Password (2) - Leave Home";
          
          while (!Serial.available());

          decision = Serial.parseInt();
          Serial.read();

          if (decision == 1) {
              delay(1000);
              onchangepassword();
              repasswordlogin = 1; // Sets the variable to 1 so that the door does not open when the password is entered again.
              Serial << "You have been logged out. Please enter the new password to log in again\n";
              Serial << "Password: ";
          } else if (decision == 2) {
              digitalWrite(ledpin, LOW); // Turns the light off again
              Serial << "\nSee you next time\n";

              // Opens the door to leave the house
              servo.write(0);
              delay(7000); // 7 seconds delay
              servo.write(90);

              Serial << "Password: ";
          } else {
            return;
          }
          

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
