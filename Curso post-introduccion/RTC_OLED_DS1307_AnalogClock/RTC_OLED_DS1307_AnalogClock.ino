// Original code:
// https://codebender.cc/sketch:45433

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "RTClib.h"

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

//Instantiate objects from class definitions
RTC_DS1307 RTC;

int centerX = 0;
int centerY = 0;

void setup () {
    Wire.begin();
    RTC.begin();
     
    if (! RTC.isrunning()) {
        //The following line sets the RTC to the date & time this sketch was compiled
        RTC.adjust(DateTime(__DATE__, __TIME__));
    }
    
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
    display.clearDisplay();
    
    Serial.begin(57600);
    
    centerX = SSD1306_LCDWIDTH/2;
    centerY = SSD1306_LCDHEIGHT/2; 
}
 
void loop() {
    int clocksize = 31;
    
    static uint8_t x0,y0,x1,y1;
    static float radianshour,radiansmin,radianssec,hourx,houry,minx,miny,secx,secy;
    static boolean drawnFirst=false;
    
    display.clearDisplay();
    
    display.drawCircle(centerX, centerY, clocksize, WHITE);
    
    // get the time from the clock chip
    DateTime now = RTC.now();
    	
    //convert hms into radians
    radianshour = (((now.hour() * 360) / 12) + 270) * (PI / 180);
    radiansmin = (((now.minute() * 360) / 60) + 270) * (PI / 180);
    radianssec = (((now.second() * 360) / 60) + 270) * (PI / 180);
    		
    // calculate clock hand endpoints		
    hourx = cos(radianshour) * (clocksize / 2.5);
    houry = sin(radianshour) * (clocksize / 2.5);
    
    minx = cos(radiansmin) * (clocksize / 1.4);
    miny = sin(radiansmin) * (clocksize / 1.4);
    
    secx = cos(radianssec) * (clocksize / 1.1);
    secy = sin(radianssec) * (clocksize / 1.1);

    display.drawLine(centerX,centerY,centerX+hourx,centerY+houry,WHITE);
    display.drawLine(centerX,centerY,centerX+minx,centerY+miny,WHITE);
    display.drawLine(centerX,centerY,centerX+secx,centerY+secy,WHITE);
    display.display();
}

