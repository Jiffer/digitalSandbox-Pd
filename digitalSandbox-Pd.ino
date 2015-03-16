// use with   digital-sandbox2.pd

// number of bytes  sent by Pd in a "packet"
#define NUMBYTES 10

int incomingByte[NUMBYTES]; // for incoming serial data

int numDigitals;
int numAnalogs;
int numOutputs;
int numPWM;

int digitalPins[] = {2, 12}; // digital inputs: pins
int analogPins[] = {A0, A1, A2, A3}; // analog pins
int outputPins[] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 13}; // outputs
int pwmPins[] =    {1, 0, 0, 0, 0, 0, 1, 1,  1,  0}; // 1 = pwm output
                   // note that 5 and 6 are pwm pins but in this sketch I treat them as digital
                   // just add them to the list if you want to do analogWrite with pins 5, 6

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // actually 57600

  numDigitals = sizeof(digitalPins) / sizeof(int);
  numAnalogs = sizeof(analogPins) / sizeof(int);
  numOutputs = sizeof(outputPins) / sizeof(int);
  numPWM = sizeof(pwmPins) / sizeof(int);

  for (int i = 0; i < numDigitals; i++)
  {
    pinMode(digitalPins[i], INPUT);
  }

  for (int i = 0; i < numOutputs; i++)
  {
    pinMode(outputPins[i], OUTPUT);
  }

}

void loop() {
  /////////////////////
  // Send digital data
  /////////////////////
  // send a 'D' to indicate digital data packet start
  Serial.println('D');

  // loop through all the digital pins.
  for (int i = 0; i < numDigitals; i++)
  { // read the value and send it to the serial port
    Serial.println(digitalRead(digitalPins[i]));
  }
  // a small 'd' indicates end of digital packet
  Serial.println('d');

  /////////////////////
  // Send analog data
  /////////////////////
  // send a 'A' to indicate analog data packet start
  Serial.println('A');
  // loop through all the analog pins.
  for (int i = 0; i < numAnalogs; i++)
  { // read the value and send it to the serial port
    Serial.println(analogRead(analogPins[i]));
  }
  // a small 'a' indicates end of analog packet
  Serial.println('a');


  /////////////////////
  // read from Pd
  /////////////////////
  // Check if we've collected new data from Pd

  // look for 3 bytes of data and write them to the 3 output pins
  if (Serial.available() > NUMBYTES - 1) {
    for ( int i = 0; i < NUMBYTES; i++) {
      incomingByte[i] = Serial.read();
      if (pwmPins[i] == 0)
        digitalWrite(outputPins[i], incomingByte[i]);
      else
        analogWrite(outputPins[i], incomingByte[i]);
    }
  }

  // short break
  delay(10);

}
