
#include "wiring_private.h"
#include "pins_arduino.h"

float analogRead_vcc();

long time1,time2;
unsigned int adcv,adcv2;
float adcvcc,adctmp,amps,pw_pulse,pw_mosfets,interval;
#define mosfets 2 
#define vcc A0
#define vcap A1
#define vout A2
//#define spike A3
#define mosfets_on  digitalWrite(mosfets,1)
#define mosfets_off  digitalWrite(mosfets,0)
#define mosfetspwmax 20  //max energy in joules to not damage the mosfets
#define pulse       40  //pulse energy aprox
#define mosfets_resistance float(0.0006) 
#define A_min       100
#define A_max      1200   
#define vcc_min    float(3.7) 
#define vcap_min    float(1.9) 

#define pin_leds 6 
#define pin_red 5
#define pin_green 7
#define pin_blue 8
#define red  digitalWrite(pin_red,0);    
#define yellow {red; green;}             
//#define violeta
#define blue digitalWrite(pin_blue,0);    
#define green digitalWrite(pin_green,0); 
#define white  {red;green;blue;}  //welding
#define black {digitalWrite(pin_red,1);digitalWrite(pin_green,1);digitalWrite(pin_blue,1); }

void setup() {
pinMode(mosfets,OUTPUT); 
mosfets_off;
pinMode(vcc,OUTPUT); 
digitalWrite(vcc,1);

pinMode(vcap,INPUT); 
pinMode(vout,INPUT);
pinMode(pin_leds,OUTPUT);
pinMode(pin_red,OUTPUT);
pinMode(pin_green,OUTPUT);
pinMode(pin_blue,OUTPUT);
black;
analogWrite(pin_leds,25);
Serial.begin(9600);
 adcvcc=analogRead_vcc();   //when power up ,indicate battery charge
 if(adcvcc>4) blue
 else if(adcvcc>3.8 ) green
 else if(adcvcc>3.6 ) yellow
 else red
 delay(1000);
 black
 delay(200);
 Serial.println(adcvcc);
  adctmp=(float (analogRead(vcap))*adcvcc)/1024;   //capacitors charge
  Serial.println(adctmp);
 if(adctmp>2.4) blue
 else if(adctmp>2.2) green
 else if(adctmp>2) yellow
 else red
 delay(1000);
 black
 delay(200);
}

void loop() {
  int count;
  adcvcc=analogRead_vcc();
  adctmp=(float (analogRead(vcap))*adcvcc)/1024;
  if(adcvcc<vcc_min) red                            //battery low
  else if(adctmp<vcap_min) yellow                   //capacitors low
  else  {
    if(analogRead(vout)>300)
    {
    delay(1000);
    adcv=analogRead(vout);
    if(adcv>300)                                   //if there is contact for 1 second, go weldding  
    {
      white
      mosfets_on;
      delay(4);
      adcv=analogRead(vcap);
      adcv2=analogRead(vout);
      mosfets_off;
      black
      delay(10);
      adcv-=adcv2;
      amps=(float(adcv2)*adcvcc/1024)/mosfets_resistance;
      if(amps>A_max || amps<A_min  ||(adcv<adcv2)) blue     
      else                                         // if first short pulse was ok continue 
      {
      white  
      pw_pulse=0;
      pw_mosfets=0 ; 
      count=20;                                   //time limit is about 40ms  
      time1=millis();  
      mosfets_on;
      do{
      delay(2);
      adcv=analogRead(vcap);
      adcv2=analogRead(vout);
      time2=millis();
      interval=(time2-time1)*float(0.001); time1=time2;
      
      adctmp=float(adcv2)*adcvcc/1024 ;//voltage on mosfets;
      amps=adctmp/mosfets_resistance ;
      pw_mosfets+=adctmp*amps*interval;
      adctmp=float(adcv-adcv2)*adcvcc/1024 ;//welding voltage=capacitors voltage - mosfets voltage 
      pw_pulse+=adctmp*amps*interval;       //welding power
      if((pw_mosfets>mosfetspwmax)||(!--count)||amps>A_max || amps<A_min  ||(adcv<adcv2)) break;
      if(pw_pulse>pulse) break;
      }while(1);
      mosfets_off;
      black
       if(pw_pulse>pulse) green    //welding was ok
       else if(amps>A_max) red     // shortcut or mosfets broken
       else blue                   // other error,pobably bad contact
            
       
       Serial.println(count);
        Serial.println(pw_pulse);
         Serial.println(pw_mosfets);
          Serial.println(amps);
      
      delay(2000);       //leave some time for operator move and mosfets cool down 
   
      }}}}
      delay(150);
      black
      delay(250);
}

float analogRead_vcc()
{
  uint8_t low, high;
  ADMUX = 64+14; //1.1v bandgap
  delay(1);
  sbi(ADCSRA, ADSC);
  while (bit_is_set(ADCSRA, ADSC));
 
 low  = ADCL;
 high = ADCH; 

 return  float (1.1*1024)/ float((high << 8) | low);
}
