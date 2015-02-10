/* Example for MADE Arduino workshop
Please visit: made-bcn.org/arduino

How to connect:
VCC --> 5V
GND --> GND
 A0 --> A0
 
*/

// More about algorthms: Skiena.-.TheAlgorithmDesignManual.

static const int numReadings = 23;      // Number of filter samples
const int microPin = 0;      // Arduino pin number for micro

int valMicRef = 0;
int valMic = 0;

void setup() {
  
  Serial.begin(9600);
  
  int analogValues[numReadings];
  int readingNumber; 
  // Read numReadings and save the value on analogValues array
  for (readingNumber = 0; readingNumber < numReadings; readingNumber++) {
    //get the reading:
    analogValues[readingNumber] = analogRead(microPin);
    // increment the counter:
    readingNumber++;
  }
  
  // Sort analogValuee Array
  int out, in, swapper;
  for(out=0 ; out < numReadings; out++) {  // outer loop
    for(in=out; in<(numReadings-1); in++)  {  // inner loop
      if( analogValues[in] > analogValues[in+1] ) {   // out of order?
        // swap them:
        swapper = analogValues[in];
        analogValues [in] = analogValues[in+1];
        analogValues[in+1] = swapper;
      }
    }
  }
  int median = 0;
  median = analogValues[numReadings / 2]; 
  
  byte i = 0;
  int total = 0;
  int average = 0;
  for (i = 0; i< numReadings; i++) {
    total = total + analogValues[i];
  }
  
  average = total/numReadings;
   
  // print the results:
  // print the array, nicely ASCII-formatted:
  i = 0;
  Serial.print("Array: [");
  for (i = 0; i < numReadings; i++) {
    Serial.print(analogValues[i], DEC);
    Serial.print (", ");
  }
  Serial.println("]\r\n");
  // average the array:
  Serial.print(" Average = ");
  Serial.print(average, DEC);
  Serial.print("\tMedian = ");
  Serial.println(median, DEC);
  delay(2000);
  
  valMicRef = median;
  // valMicRef = average;
     
}

void loop() {
  
  valMic = analogRead(A0);    // read the micro input pin
  Serial.print("Mic: ");
  Serial.println(valMicRef - valMic); 
  delay(50);
  
}


