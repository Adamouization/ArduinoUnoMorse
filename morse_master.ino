#include <Wire.h> //Library imported to allow the master to send information to the slave via wires

const int buttonPin1 = 3;     // Right button assigned to pin #3
const int buttonPin2 = 2;     // Left button assigned to pin #2
//Integers sent to the slave arduino depending on which button is pressed
const int x1 = 2;
const int x2 = 3;

// Variable for reading the pushbutton status
// Defines them both as 2 for starting values
int buttonState1 = 0;
int buttonState2 = 0;

// Defines two booleans as false
// They are volatile so that they can be changed
volatile boolean isPressed1 = false;
volatile boolean isPressed2 = false;

/**
 * SETUP 
 * Contains code that runs before anything else on system start/restart
 * The code is used to initiliaze connections between arduinos and their components
 */
void setup() {
  // Initializing the pushbutton pins as an input:
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  // AttachInterrupts to the two button pins
  // When the button state changes it calls the corresponding press methods
  attachInterrupt(digitalPinToInterrupt(3), press1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(2), press2, CHANGE);
  Wire.begin(); // Start the I2C Bus as Master
  Serial.begin(9600); //Initialize baud rate at 9600
}


// Loops constantly forever
void loop() {

  // Reads the state of the pushbutton value:
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);

  /*
   * If right button is pressed, the buttons state changes
   * This calls the press1 method
   * The method changes a corresponding boolean to true
   * Wire begins transmission and sends the defined x1 value
   * Changes the volatile boolean 'isPressed1' back to false so the if statement doesnt constantly loop
   */
  if (isPressed1 == true) {
  Wire.beginTransmission(9);  // Transmits to device #9 (slave)
  Wire.write(x1); // Sends x1 to the slave via wires
  Wire.endTransmission(); // Ends transmission
  isPressed1 = false;
  }

   /*
   * If left button is pressed, the buttons state changes
   * This calls the press2 method
   * The method changes a corresponding boolean to true
   * Wire begins transmission and sends the defined x2 value
   * Changes the volatile boolean 'isPressed2' back to false so the if statement doesnt constantly loop
   */
  if (isPressed2 == true) {
  Wire.beginTransmission(9);  // Transmits to device #9
  Wire.write(x2); // Sends x2 to the slave via wires
  Wire.endTransmission(); // Ends transmission
  isPressed2 = false;
  }

  delay(500); //Delay between each transmission
}

/*
 * Method called upon right button state change
 * Changes the volatile boolean 'isPressed1' to true
 */
void press1(){
  isPressed1 = true;
}

/*
 * Method called upon left button state change
 * Changes the volatile boolean 'isPresed2' to true
 */
void press2(){
  isPressed2 = true;
}

