/*
  Serial Event example

  When new serial data arrives, this sketch adds it to a String.
  When a newline is received, the loop prints the string and clears it.

  A good test for this is to try it with a GPS receiver that sends out
  NMEA 0183 sentences.

  NOTE: The serialEvent() feature is not available on the Leonardo, Micro, or
  other ATmega32U4 based boards.

  created 9 May 2011
  by Tom Igoe

  This example code is in the public domain.

  https://docs.arduino.cc/built-in-examples/communication/SerialEvent/
*/

String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
int delayAmount = 0;
//pwm - 2-13  - enable
//regular 22-53 for in1/in2
const int LEFT_IN_1 = 22;
const int LEFT_IN_2 = 23;
const int LEFT_ENABLE = 2; // speed

const int RIGHT_IN_1 = 32;
const int RIGHT_IN_2 = 33;
const int RIGHT_ENABLE = 3; // speed

const int FRONT_IN_1 = 42;
const int FRONT_IN_2 = 43;
const int FRONT_ENABLE = 4; // speed

const int BACK_IN_1 = 52;
const int BACK_IN_2 = 53;
const int BACK_ENABLE = 5; // speed

void setup() {
  // initialize serial:
  Serial.begin(9600);
  Serial1.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);   // change state of the LED by setting the pin to the LOW voltage level
  
}

void loop() {

  if (stringComplete) {
     Serial.println(inputString); //this should stay as Serial because it prints to debug console
    if(inputString=="B,CW\n"){
      Serial.println("Clockwise");
          digitalWrite(ledPin, HIGH);
           analogWrite(thisPin, brightness);
    }
    else if(inputString=="B,CCW\n"){
      Serial.println("CounterClockwise");
    }
    else{
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
    
   
  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent1() {
  while (Serial1.available()) {
    // get the new byte:
    char inChar = (char)Serial1.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
