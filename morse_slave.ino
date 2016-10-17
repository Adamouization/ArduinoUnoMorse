#include <Servo.h> // Library imported to allow the servo to work properly
#include <Wire.h> // Library imported to allow the slave to receive information from the master via wires

/* Initializing all the variables that will be needed */
int distancePin = 0;  // Distance detector assigned to pin #0
int SensorValue;  // Variable used to calculate the intensity of infrared light sent back to the distance detector
int speakerPin = 9; // Speaker assigned to pint #9
Servo myservo;  // Create servo object to control the servo
int pos = 0;  // Initializes the position of the servo to 0
const int pos1 = 0; // Position the servo goes to when the right button is pressed on the master arduino
const int pos2 = 90;  // Position the servo goes to when the middle button is pressed on the master arduino
const int pos3 = 180; // Position the servo goes to when the left button is pressed on the master arduino
int x;  // Integer used to store temporarily the integer received by the master
int x1 = 0; // Initializes the integer x1 received by the right button when it is pressed on the master arduino
int x2 = 0; // Initializes the integer x3 received by the middle button when it is pressed on the master arduino

/**
 * SETUP 
 * Contains code that runs before anything else on system start/restart
 * The code is used to initiliaze connections between arduinos and their components
 */
void setup()
{
  pinMode(speakerPin, OUTPUT); // Define the speaker pin 9 as an output
  myservo.attach(2); // Attach the servo at pin 2
  Serial.begin(9600); // Initialize baud rate at 9600
  Wire.begin(9);  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent); // Calls the 'receiveEvent' method when data is received
  myservo.write(pos2); // Move the servo to 90 degrees
}

// Method called when information is received from the master via the wires
void receiveEvent(int byte){
  x = Wire.read();  // Read one character from the I2C 
}

// Loops constantly forever
void loop(){
   SensorValue = analogRead(distancePin); // Sets SensorValue as the distance value from the IR distance calculator
   delay(1000); // Delay by 1 second  

    /*
     * DOT - Right Button Press
     * If the value received is equal to 2
     * Move servo to position 1/0 degrees
     * Delay by 1 second, this is adequate time for the distance to be read once and ONLY once
     * Moves the servo back to neutral position
     * Prints a dot to serial monitor
     * Then sets the x value to one thats not in use. this prevents the if statement from looping until the next value is received via transmission
     */
    if (x == 2){
      myservo.write(pos1);
      delay(1000);
      myservo.write(pos2);
      Serial.println(".");
      x=10;   
    }

    /*
     * DASH - Left Button Press
     * If the value received is equal to 3
     * Move servo to position 3/180 degrees
     * Delay by 1 second, this is adequate time for the distance to be read once and ONLY once
     * Moves the servo back to neutral position
     * Prints a dash to serial monitor
     * Then sets the x value to one thats not in use. this prevents the if statement from looping until the next value is received via transmission
     */
    if (x == 3){
      myservo.write(pos3);
      delay(1000);
      myservo.write(pos2);
      Serial.println("-");
      x=10;
    }

    /*
     * DOT - Lower Value
     * If the SensorValue is read to be above 280 
     * Then it prints the note c for 200 milliseconds     * 
     */
    if(SensorValue>280){ //dot
      playNote ('c', 200);
    }

     /*
     * DASH - Higher Value
     * If the SensorValue is read to be above 510 
     * Then it prints the note c for 800 milliseconds     * 
     */
    if(SensorValue>510){  //dash
      playNote ( 'c', 800);
    }

    /* 
     *  If neither the dot or dash zone is over the IR sensor
     *  The value returned will be very small
     *  As such neither of the two if statements above will be satisfied
     *  in turn no note will be played
     */
}

/*
 * Function called by the playNote function which creates the playable notes. 
 * This function is used for the tone and the duration of the sound.
 */
void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}

// Function in which the 8 playable sounds by the piezo elements are defined
void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
  
  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}
