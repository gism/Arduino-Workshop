/* Example for MADE Arduino workshop
Please visit: made-bcn.org/arduino

How to connect:
VCC --> 5V
GND --> GND
 A0 --> A0
 
*/


const int microPin = 0;
int valMic = 0;
int valMicRef = 0;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  pinMode(A0,INPUT);
  valMicRef = analogRead(A0);
}

void loop()
{
  valMic = analogRead(A0);    // read the micro input pin
  Serial.print("Mic: ");
  Serial.println(valMicRef - valMic); 
}

