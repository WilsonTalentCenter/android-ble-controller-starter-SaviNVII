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
  pinMode(LEFT_IN_1, OUTPUT);
  pinMode(LEFT_IN_2, OUTPUT);
  pinMode(LEFT_ENABLE, OUTPUT);

  pinMode(RIGHT_IN_1, OUTPUT);
  pinMode(RIGHT_IN_2, OUTPUT);
  pinMode(RIGHT_ENABLE, OUTPUT);

  pinMode(FRONT_IN_1, OUTPUT);
  pinMode(FRONT_IN_2, OUTPUT);
  pinMode(FRONT_ENABLE, OUTPUT);

  pinMode(BACK_IN_1, OUTPUT);
  pinMode(BACK_IN_2, OUTPUT);
  pinMode(BACK_ENABLE, OUTPUT);

  pinMode(51, OUTPUT);
  
  digitalWrite(51, HIGH);

  digitalWrite(LEFT_IN_1, LOW);  
  digitalWrite(LEFT_IN_2, LOW);  
  analogWrite(LEFT_ENABLE, 0); //speed is 0-255  

  digitalWrite(RIGHT_IN_1, LOW);  
  digitalWrite(RIGHT_IN_2, LOW);  
  analogWrite(RIGHT_ENABLE, 0); //speed is 0-255

  digitalWrite(FRONT_IN_1, LOW);  
  digitalWrite(FRONT_IN_2, LOW);  
  analogWrite(FRONT_ENABLE, 0); //speed is 0-255

  digitalWrite(BACK_IN_1, LOW);  
  digitalWrite(BACK_IN_2, LOW);  
  analogWrite(BACK_ENABLE, 0); //speed is 0-255   
  
}

void loop() {

  if (stringComplete) {
     Serial.println(inputString); //this should stay as Serial because it prints to debug console
    if(inputString=="B,CW\n"){
      Serial.println("Clockwise");
    }
    else if(inputString=="B,CCW\n"){
      Serial.println("CounterClockwise");
    }
    else if(input.startsWIth("J")) {
      String data = input.substring(2);
      
      // Find the comma separating bx and by
      int commaIndex = data.indexOf(',');
      
      if (commaIndex != -1) {
        String bxStr = data.substring(0, commaIndex);
        String byStr = data.substring(commaIndex + 1);
        
        int bx = bxStr.toInt();
        int by = byStr.toInt();
        
        if(bx>0){
          digitalWrite(FRONT_IN_1,HIGH);
          digitalWrite(FRONT_IN_2,LOW);
          analogWrite(FRONT_ENABLE, abs(bx)*2-1);

          digitalWrite(BACK_IN_1,HIGH);
          digitalWrite(BACK_IN_2,LOW);
          analogWrite(BACK_ENABLE, abs(bx)*2-1);
        }else if(bx<0){
          digitalWrite(FRONT_IN_1,LOW);
          digitalWrite(FRONT_IN_2,HIGH);
          analogWrite(FRONT_ENABLE, abs(bx)*2-1);

          digitalWrite(BACK_IN_1,LOW);
          digitalWrite(BACK_IN_2,HIGH);
          analogWrite(BACK_ENABLE, abs(bx)*2-1);
        }

        if(by>0){
          digitalWrite(LEFT_IN_1,HIGH);
          digitalWrite(LEFT_IN_2,LOW);
          analogWrite(LEFT_ENABLE, abs(by)*2-1);

          digitalWrite(RIGHT_IN_1,HIGH);
          digitalWrite(RIGHT_IN_2,LOW);
          analogWrite(RIGHT_ENABLE, abs(by)*2-1);
        }else if(by<0){
          digitalWrite(LEFT_IN_1,LOW);
          digitalWrite(LEFT_IN_2,HIGH);
          analogWrite(LEFT_ENABLE, abs(by)*2-1);

          digitalWrite(RIGHT_IN_1,LOW);
          digitalWrite(RIGHT_IN_2,HIGH);
          analogWrite(RIGHT_ENABLE, abs(by)*2-1);
        }

        Serial.print("bx: ");
        Serial.print(bx);
        Serial.print(", by: ");
        Serial.println(by);
      }

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
