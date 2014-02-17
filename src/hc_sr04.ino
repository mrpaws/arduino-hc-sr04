/* 
   HC-SR04 Sensor Serial Outputter 
   Matt Pawelski
   
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
     
   Arduino connection to circuit:
   	* VCC connection of the HC-SR04 attached to +5V
	* GND connection of the HC-SR04 attached to ground
	* TRIG connection of the HC-SR04 attached to pin 6
	GH* ECHO connection of the HC-SR04 attached to pin 7

   Distance Formulas:
     uS / 58 = centimeter
     uS / 148 = inch
     range = high level time * velocity (340M/S) / 2;
     
     *MFR. suggests 60ms measurement cycle minimum to avoid mixing signals.
 */

/* Declarations */


/* Set our trigger and echo pulse pins and LED pins */
const int redPin = 3;
const int yellowPin = 4;
const int greenPin = 5;
const int trigPin = 6;
const int echoPin = 7;


bool is_red = false;
bool is_yellow = false;
bool is_green = false;

void switchLED(void (*func)());

/* Procedures */

void setup() {
  /* initialize serial communication for testing */
  Serial.begin(9600);
  /* set our echo and trig pin modes here to save processing time */
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(redPin, OUTPUT);
}

void loop()
{
  /* duration (in microseconds(uS)) */
  long duration, inches, cm;

  /* 
      Fire LOW pulse beforehand to ensure a clean HIGH pulse: <-- this may be unnecessary with hc-sr04
      Fire the trigger pulse (10uS);
  */
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  /* capture the response pulse from the echo pin */
  //  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  /* convert the time into a distance */
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  if ( (inches <  60) && ( inches  > 24 ) ) { 
    /* turn yellow when less than 5 feet */
    stateCheck(yellowPin);
  }
  else if ( inches < 24 ) { 
    /* turn red when less than 2 feet */
    stateCheck(redPin);
  }
  else if ( inches > 1600 ) {
    stateCheck(0);
  }
  else if ( inches > 60 ) {
    stateCheck(greenPin);
  }

  /* print console info */
  Serial.print(inches);
  Serial.print("in");
  Serial.println();
  
  /* Datasheet recommends 60ms test intervals */
  delay(3000);
}

void stateCheck(int led) {
  /* examine the state of all LEDs, update if necessary */
  if ( led == redPin ) { 
    if ( is_red == false ) {
      switchLED(&setRed);
      Serial.print("State change: RED");
      Serial.println();

    } 
  }
  else if ( led == yellowPin ) {
    if ( is_yellow == false ) { 
      switchLED(&setYellow);
      Serial.print("State change: YELLOW");
      Serial.println();
    }
  }
  else if ( led == greenPin ) {
    if ( is_green == false ) {
      switchLED(&setGreen);
      Serial.print("State change: GREEN");
      Serial.println();
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

long microsecondsToInches(long microseconds)
{
  /* 
     convert the response pulse duration from microseconds
     to Inches, using the formulas provided by device 
     datasheet
  */
  return microseconds / 148 ;
}

long microsecondsToCentimeters(long microseconds)
{
  /* 
     convert the response pulse duration from microseconds
     to Centimeters, using the formulas provided by device 
     datasheet
  */
  return microseconds / 58 ;
}
