/* 
   HC-SR04 Ultrasonic Range Sensor Serial Outputter 
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
      - (OOP)Modularize and abstract from procedural approach. Or, leave like this for efficiency
        since this is hardware?
      - (Cruft)Add debug mode.  Not likely to happen here.
*/

/* Preprocessing */

/* Adjust these to suit your needs.  
*/
#define DISTANCE_UNIT "centimeters"  // inches | centimeters
#define PRINT_DEBUG false     // true | false

/* Declarations */

/* Trigger, echo pulse pins */
const int trigPin = 6;
const int echoPin = 7;

/* Functions */
long uSToIn(long uS);
long uSToCm(long uS);

/* Procedures */

void setup() {
  /* initialize serial communication */
  Serial.begin(9600);
  /* set our echo and trig pin modes here to save processing time */
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
}

void loop()
{
  /* main Main MAIN */

  /* duration (in uS( microseconds )) */
  long uS, distance; 

  /* Fire the trigger pulse (10uS) */
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  /* capture the response pulse from the echo pin */
  uS = pulseIn(echoPin, HIGH);

  /* convert the time into a distance */ 
  distance = convDistance(uS);

  /* print to console */
  printValues(distance);

  /* Datasheet recommends 60ms test intervals */
  delay(60);
}

long convDistance(long uS) {
  /* convert the duration (350m/s -> defined unit) */
  if ( DISTANCE_UNIT == "inches" ) {
    return uSToIn(uS);
  }
  else {
    return uSToCm(uS);
  }
}

void printValues(long distance) {
  /* print out values to console */
  Serial.print(distance);
  if ( DISTANCE_UNIT == "inches" ) { 
    Serial.print(" in");
  }
  else { 
    Serial.print(" cm");
  }
  Serial.println();
}

long uSToIn(long uS) {
  /* 
     convert the response pulse duration from uS
     to In, using the formulas provided by device 
     datasheet
  */
  return uS / 148 ;
}

long uSToCm(long uS) {
  /* 
     convert the response pulse duration from uS
     to Cm, using the formulas provided by device 
     datasheet
  */
  return uS / 58 ;
}
