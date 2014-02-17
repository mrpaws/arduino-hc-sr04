/* 
   HC-SR04 Ultrasonic Range Sensor Stoplight Application
   Matt Pawelski
   02/17/2014
   
   Adapated from PING))) Arduino example code for
   relatively inexpensive HC-SR04.    

   PING))) Tutorial:
     http://www.arduino.cc/en/Tutorial/Ping
   HC-SR04 Datasheet:
     http://users.ece.utexas.edu/~valvano/Datasheets/HCSR04b.pdf
   
   HC-SR04 Specs:
     Working Voltage DC 5 V
     Working Current 15mA
     Working Frequency 40Hz
     Max Range 4m
     Min Range 2cm
     MeasuringAngle 15 degree
     Trigger Input Signal 10uS TTL pulse
     Echo Output Signal Input TTL lever signal and the range in proportion
     Dimension 45*20*15mm 
     
   Arduino connections to circuit:
        * Red LED connection attached to pin 3
        * Yellow LED connection attached to pin 4
        * Green LED connection attached to pin 5
   	* VCC connection of the HC-SR04 attached to +5V
	* GND connection of the HC-SR04 attached to ground
	* TRIG connection of the HC-SR04 attached to pin 6
	* ECHO connection of the HC-SR04 attached to pin 7

   Distance Formulas:
     uS / 58 = centimeter
     uS / 148 = inch
     range = high level time * velocity (340M/S) / 2;
     
     *MFR. suggests 60ms measurement cycle minimum to avoid mixing signals.
 
    ToDo/Notes: 
      - (Performance) Don't bother converting distance unless console output is requested, just
        set thresholds based on uS response
      - (Usability) Manage error condition better:  If sensor cannot reliably determine distance
        then flash the last known light rapidly.
      - (Oops)Merge this back to the mainline due to improvements in sensor handling code.
      - (OOP)Modularize and abstract from procedural approach. Or, leave like this for efficiency
        since this is hardware?
      - (Usability)Add parameters to allow for unit system specification.
      - (Cross-Compatability)Is the error length value microcontroller dependent or range sensor dependent?
      - (Policy Rant)Is it necessarily an error if they don't return?  
        May need to add a counter and accumulate them to judge whether there is just nothing in
        front of the sensor or if it is an error. The harm is that if we flash green on error, 
        if you're close but can manage to trick the sensor, you could be 3 inches away and flashing green.
        The logic would fail if being used to prevent a moving object from proceeding into an obstacle.
        Apply the above thought in reverse though, and we can simply make switching to green more strict.
        I think the weakness in a single data stream is revealed here.  Another sensor could provide
        reliability by confirming the primary data source.
*/

/* Preprocessing */

/* Adjust these to suit your needs.  Be sure to observe max ranges */
#define DISTANCE_UNIT inches
#define RED_LENGTH 24
#define YELLOW_LENGTH 48
#define GREEN_LENGTH 48
#define ERROR_LENGTH 1600
#define PRINT_VALUES false
#define PRINT_DEBUG false

/* Declarations */

/* Trigger, echo pulse pins and LED pins */
const int redPin = 3;
const int yellowPin = 4;
const int greenPin = 5;
const int trigPin = 6;
const int echoPin = 7;

/* States */
bool is_red = false;
bool is_yellow = false;
bool is_green = false;

/* Functions */
void stateCheck(int led) ;
void switchLED(void (*LEDAction)()) ;
void clearLEDs(void);
void setGreen(void);
void setYellow(void);
void setRed(void);
void unsetGreen(void) ;
void unsetYellow(void) ;
void unsetRed(void);
long uSToInches(long microseconds);
long uSToCentimeters(long microseconds);

/* Procedures */

void setup() {
  /* initialize serial communication for testing */
  if ( (PRINT_VALUES == true) || (PRINT_DEBUG == true ) ) {
    Serial.begin(9600);
  }
  /* set our echo and trig pin modes here to save processing time */
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
}

void loop()
{
  /* main Main MAIN */

  /* duration (in microseconds(uS)) */
  long duration, inches, cm;

  /* Fire the trigger pulse (10uS) */
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  /* capture the response pulse from the echo pin */
  duration = pulseIn(echoPin, HIGH);

  /* convert the time into a distance */ 
  inches = uSToInches(duration);

  /* compare most recent sensory data to current state */ 
  if ( (inches <=  YELLOW_LENGTH) && ( inches  > RED_LENGTH ) ) { 
    stateCheck(yellowPin);
  }
  else if ( inches <= RED_LENGTH ) { 
    stateCheck(redPin);
  }
  else if ( inches >= ERROR_LENGTH ) {
    stateCheck(0);
  }
  else if ( inches > GREEN_LENGTH ) {
    stateCheck(greenPin);
  }

  /* print values to the console if requested */
  if ( PRINT_VALUES == true ) {
    printValues(inches);
  }

  /* Datasheet recommends 60ms test intervals */
  delay(60);
}

void printValues(long distance) {
  /* print out values to console */
  Serial.print(distance);
  Serial.print("in");
  Serial.println();
}

void stateCheck(int led) {
  /* examine the state of all LEDs, update if necessary */
  if ( led == greenPin ) {
    if ( is_green == false ) {
      switchLED(&setGreen);
      if ( PRINT_DEBUG == true ) {
        Serial.print("State change: RED");
        Serial.println();
      }
    }
  }
  else if ( led == redPin ) { 
    if ( is_red == false ) {
      switchLED(&setRed);
      if ( PRINT_DEBUG == true ) { 
        Serial.print("State change: RED");
        Serial.println();
      }
    } 
  }
  else if ( led == yellowPin ) {
    if ( is_yellow == false ) { 
      switchLED(&setYellow);
      if ( PRINT_DEBUG == true ) {
        Serial.print("State change: YELLOW");
        Serial.println();
      }
    }
  }
}

void switchLED(void (*LEDAction)()) {
  /* switches to given LED */
  clearLEDs();
  (*LEDAction)();
}

void clearLEDs(void) { 
  /* clear all Pins */
  unsetRed();
  unsetYellow();
  unsetGreen();
}

void setGreen(void) {
  /* set green */ 
  digitalWrite(greenPin, HIGH);
  is_green = true;
}

void setYellow(void) {
  /* set yellow */
  digitalWrite(yellowPin, HIGH);
  is_yellow = true;
}

void setRed(void) {
  /* set green */
  digitalWrite(redPin, HIGH);
  is_red = true;
}

void unsetGreen(void) {
  /* unset green */
  digitalWrite(greenPin, LOW);
  is_green = false;
}

void unsetYellow(void) {
  /* unset yellow */
  digitalWrite(yellowPin, LOW);
  is_yellow = false;
}

void unsetRed(void) {
  /* unset green */
  digitalWrite(redPin, LOW);
  is_red = false;
}

long uSToInches(long microseconds)
{
  /* 
     convert the response pulse duration from microseconds
     to Inches, using the formulas provided by device 
     datasheet
  */
  return microseconds / 148 ;
}

long uSToCentimeters(long microseconds)
{
  /* 
     convert the response pulse duration from microseconds
     to Centimeters, using the formulas provided by device 
     datasheet
  */
  return microseconds / 58 ;
}
