/* Example for MADE Arduino workshop
Please visit: made-bcn.org/arduino

 Microphone
 --------------------------
  How to connect:
  VCC --> 5V
  GND --> GND
   A0 --> A0
 
 OLED screen
 --------------------------
 How to connect it:
 GND --> GND
 VCC --> VCC
 SCL --> A4
 SDA --> A5
 --------------------------
 
*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

static unsigned char PROGMEM flappybird_frame_1[] = { 0x03, 0xF0, 0x0C, 0x48, 0x10, 0x84, 0x78, 0x8A, 0x84, 0x8A, 0x82, 0x42, 0x82, 0x3E, 0x44, 0x41,0x38, 0xBE, 0x20, 0x41, 0x18, 0x3E, 0x07, 0xC0 };
static unsigned char PROGMEM flappybird_frame_2[] = { 0x03, 0xF0, 0x0C, 0x48, 0x10, 0x84, 0x20, 0x8A, 0x40, 0x8A, 0x40, 0x42, 0x7C, 0x3E, 0x82, 0x41, 0x84, 0xBE, 0x88, 0x41, 0x78, 0x3E, 0x07, 0xC0 };

static const int numReadings = 23;      // Number of filter samples
const int microPin = 0;      // Arduino pin number for micro

int valMicRef = 0;
int valMic = 0;

int x = 0;
int valMicPrev = SSD1306_LCDHEIGHT - 10;
int frameCount = 0;

void setup() {
  
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done

  // Clear the buffer. As we are using adafruit lib buffer contains it's logo
  display.clearDisplay();
  display.display();
  
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

  // Display a fancy text
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,3);
  display.println("AudioPulsometro");
  
  valMicRef = median;
  // valMicRef = average;
    
}

void loop() {
    
  valMic = analogRead(A0);              // read the micro input pin
  int valMicCor = valMicRef - valMic;   // Remove offset

  // Preparamos el display
  // display.fillRect(0, 14, 128, 50, BLACK);
 
  int y = map(valMicCor, 0, 100, SSD1306_LCDHEIGHT - 10, 14);
  display.drawLine(x, valMicPrev, x+1, y, WHITE);
  
  valMicPrev = y;
  x++;
    
  if (x > SSD1306_LCDWIDTH){
     // Preparamos el display
    display.fillRect(0, 14, 128, 50, BLACK);
    x = 0;
  }
  
  // Draw a nice bird
  display.fillRect(110, 0, 126, 12, BLACK);
  if (frameCount < 5) {
    display.drawBitmap(110, 0, flappybird_frame_1, 16, 12, WHITE);		
  } 
  else {
    display.drawBitmap(110, 0, flappybird_frame_2, 16, 12, WHITE);			
  }  
  // Move the bird
  frameCount++;
  if (frameCount > 10){
    frameCount =0;
  }
  
  display.display();
}


