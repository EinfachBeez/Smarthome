// initialize libraries
#include <Servo.h>
#include <Keypad.h>
#include <SPI.h>
#include <Ethernet.h>

// Set a template to easy print operations
template <typename T>
Print& operator<<(Print& printer, T value) {
  printer.print(value);
  return printer;
}

const byte rows = 4;
const byte cols = 4;

//define the cymbols on the buttons of the keypads
char hexaKeys[rows][cols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Pin Config
const byte soundpin = 13;
const byte ultraechopin = 6;
const byte ultratriggerpin = 5;

byte rowPins[rows] = {9, 8, 7, 6};
byte colPins[cols] = {5, 4, 3, 2};


// Variable Config
int pin = 0;
int len;

//initialize an instance of class NewKeypad
Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, rows, cols);

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
  
  playSound(); // Give a soundoutput for starting
  
  Serial.begin(9600); // Start Serial Monitor on port 9600
  Serial.flush(); // Waits for the transmission of outgoing serial data to complete
  
  Serial << '\n' << "Smarthome is running now" << '\n';
  Serial << "Set the house password with four numbers" << '\n';
  while(Serial.available() == 0) {
    pin = Serial.read();
    pin = Serial.parseStr();
  }
  len = sizeof(pin);
  if(len == 4) {
    Serial << "The house password was set to " << pin << '\n';
  } else {
    Serial << "Return please";
    return;
  }
  
}

void loop() {
  char key = keypad.getKey();

  if(key) {
    Serial << key;
  }


}
