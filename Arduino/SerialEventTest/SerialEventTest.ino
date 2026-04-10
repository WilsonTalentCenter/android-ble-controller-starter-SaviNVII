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
     Serial.println("result:"+ inputString); //this should stay as Serial because it prints to debug console
    if(inputString=="B,CW\n"){
     delayAmount = 500;
    }
    else if(inputString=="B,CCW\n"){
     delayAmount = 100;
    }
    else{
      delayAmount = 3000;
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
    
   
  }
    digitalWrite(LED_BUILTIN, HIGH);  // change state of the LED by setting the pin to the HIGH voltage level
    delay(delayAmount);                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // change state of the LED by setting the pin to the LOW voltage level
    delay(1000); 
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {

  while (Serial1.available()) {
    // get the new byte:
    char inChar = (char)Serial1.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
