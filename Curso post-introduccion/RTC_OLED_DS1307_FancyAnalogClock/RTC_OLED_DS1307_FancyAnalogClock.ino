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
  
    int secOutRadius = 32;
    int secInRadius = 25;
    
    int minOutRadius = 24;
    int minInRadius = 15;
    
    int hourOutRadius = 14;
    int hourInRadius = 5;
   
    display.clearDisplay();
    
    DateTime now = RTC.now();
    
    static float secTan;
    int sec = now.second();
    secTan = (((sec * 360) / 60) + 270) * (PI / 180);
    secTan = tan(secTan);    
   
    static float minTan;
    int currentMin = now.minute();
    minTan = (((currentMin * 360) / 60) + 270) * (PI / 180);
    minTan = tan(minTan);
    
    static float houTan;
    int currentHour = now.hour();
    houTan = (((currentHour * 360) / 12) + 270) * (PI / 180);
    houTan = tan(houTan);
    
    Serial.println(currentHour);
    
    for(int x=30; x<SSD1306_LCDWIDTH-30; x++){
      for(int y=0; y<SSD1306_LCDHEIGHT; y++){
        
        float distX = x - centerX;
        float distY = y - centerY;
        float dist = distX * distX + distY * distY;
        float XYtan = distY/distX;
        
        
        // DRAW Seconds ARC
        if((dist < secOutRadius * secOutRadius)&&(dist > secInRadius * secInRadius)){
          if (sec>30){
            if(distX>=0){
               display.drawPixel((int)x, (int)y, WHITE);
            }else{     
              if (XYtan<secTan){       
                display.drawPixel((int)x, (int)y, WHITE);
              }
            }
          }
          if(sec<30){ // menor de 30
            if(distX>0){
              if (XYtan<secTan){       
                display.drawPixel((int)x, (int)y, WHITE);
              }
            }
          }
          if((sec==30)&&(distX>=0)){
            display.drawPixel((int)x, (int)y, WHITE);
          }
        }
        
        // DRAW Minutes ARC
        if((dist < minOutRadius * minOutRadius)&&(dist > minInRadius * minInRadius)){
          if (currentMin>30){
            if(distX>=0){
               display.drawPixel((int)x, (int)y, WHITE);
            }else{     
              if (XYtan<minTan){       
                display.drawPixel((int)x, (int)y, WHITE);
              }
            }
          }
          if(currentMin<30){ // menor de 30
            if(distX>0){
              if (XYtan<minTan){       
                display.drawPixel((int)x, (int)y, WHITE);
              }
            }
          }
          if((currentMin==30)&&(distX>=0)){
            display.drawPixel((int)x, (int)y, WHITE);
          }
        }
        
        // DRAW Hour ARC
        if((dist < hourOutRadius * hourOutRadius)&&(dist > hourInRadius * hourInRadius)){
          if (currentHour>12){
            if(distX>=0){
               display.drawPixel((int)x, (int)y, WHITE);
            }else{     
              if (XYtan<houTan){       
                display.drawPixel((int)x, (int)y, WHITE);
              }
            }
          }
          if(currentHour<12){ // menor de 30
            if(distX>0){
              if (XYtan<houTan){       
                display.drawPixel((int)x, (int)y, WHITE);
              }
            }
          }
          if((currentHour==12)&&(distX>=0)){
            display.drawPixel((int)x, (int)y, WHITE);
          }
        }        
        
        
        
      }      
    }
    
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println(currentHour);
    display.println(currentMin);
    display.println(sec);
    
    display.display();
}

