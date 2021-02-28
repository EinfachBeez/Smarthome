// initialize libraries
#include <Servo.h>

template <typename T>
Print& operator<<(Print& printer, T value) {
  printer.print(value);
  return printer;
}

// Pin Config
const int soundpin = 7;
const int ultraechopin = 6;
const int ultratriggerpin = 5;

// Variable Condig
int pin = 0;
int len;


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
  pinMode(soundpin, OUTPUT);
  playSound();
  
  Serial.begin(9600);
  Serial.flush();
  Serial << '\n' << "Smarthome is running now" << '\n';
  Serial << "Set the house password with four numbers" << '\n';
  while(Serial.available() == 0) {
    pin = Serial.read();
    pin = Serial.parseInt();
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
  
}
