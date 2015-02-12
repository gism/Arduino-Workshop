#include "Wire.h"                   //  Libreria para I2C de arduino
#define DS1307_ADDRESS B1101000     // Corresponde a 0x68, especificado en el datasheet del DS1307
#define REGISTER_ADDRESS 0x00       // Corresponde con la direccón del registro segundos

void setup(){
  
  Wire.begin();                     //  Inicializa el puerto I2C como master.
  
  Serial.begin(115200);             //  Start Serial comunication with the computer
  Serial.println("Una interficie simple entre arduino y un DS1307");

  // initDS1307();                //  Inicializamos la hora si es necesario
  
}

void loop(){
  printDateAndTime();
  delay(1000);
}

//  Codifica un valor decimal en BCD (Binario Decimal Codificado)
byte decToBcd(byte val){
  return ( (val/10*16) + (val%10) );
}

//  Decodifica un valor BCD a decimal
byte bcdToDec(byte val)  {
  return ( (val/16*10) + (val%16) );
}

//  Funcion par establecer la hora en el DS1307
void initDS1307(){
  byte second, minute, hour, weekDay, monthDay, month, year;
  
                              //  Cambia estos valores para establecerla hora.
  second = 00;           //  0-59
  minute = 02;           //  0-59
  hour = 20;             //  0-23
  weekDay = 3;           //  1-7
  monthDay = 11;         //  1-31
  month = 2;             //  1-12
  year  = 15;            //  0-99
  
  setDateAndTime(second, minute, hour, weekDay, monthDay, month, year);
}

void setDateAndTime(byte second,
    byte minute,
    byte hour,
    byte weekDay,
    byte monthDay,
    byte month,
    byte year){

  Wire.beginTransmission(DS1307_ADDRESS);      //  Indicamos al DS1307 que iniciamos comunicación
  Wire.write(REGISTER_ADDRESS);                //  Indicamos el registro que queremos escribir. (Fecha)

  Wire.write(decToBcd(second));
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));
  Wire.write(decToBcd(weekDay));
  Wire.write(decToBcd(monthDay));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));

  Wire.write(0x00);                             // Control register: OUT  |  0  |  0  |  SQWE  |  0  |  0  |  RS1  |  RS0
  Wire.endTransmission();
}

void getDateAndTime(byte *second,
    byte *minute,
    byte *hour,
    byte *weekDay,
    byte *monthDay,
    byte *month,
    byte *year){
      
  // Especificamos la direccion/registro que queremos leer paraque se guarde en el register pointer.
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(REGISTER_ADDRESS);                    //  En este caso queremos leer el registro con la fecha y hora.
  Wire.endTransmission();
  
  // Lectura fecha y hora:
  Wire.requestFrom(DS1307_ADDRESS, 7);             //  Leemos 7 bytes desde la dirección del register pointer
  *second = bcdToDec(Wire.read() & 0b01111111);    //  El primer bit corresponde al Clock Hold (CH)
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0b111111);        //  Para configuración 24h
  *weekDay = bcdToDec(Wire.read());
  *monthDay = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

void printDateAndTime(){
  byte second, minute, hour, weekDay, monthDay, month, year;
  getDateAndTime(&second, &minute, &hour, &weekDay, &monthDay, &month, &year);

  //  Queremos printar algo como: DD/MM/YYYY HH:MM:SS
  //  Este codigo hay que cambiarlo para el año 2100
  if(monthDay<10) Serial.print("0");
  Serial.print(monthDay);
  Serial.print("/");
  if(month<10) Serial.print("0");
  Serial.print(month);
  Serial.print("/");
  Serial.print("20");
  Serial.print(year);
  Serial.print(" ");
  if(hour<10) Serial.print("0");
  Serial.print(hour);
  Serial.print(":");
  if(minute<10) Serial.print("0");
  Serial.print(minute);
  Serial.print(":");
  if(second<10) Serial.print("0");
  Serial.print(second);
  
  switch (weekDay){
    case 1:
      Serial.println(" Lunes"); 
      break;
    case 2:
      Serial.println(" Martes"); 
      break;
    case 3:
      Serial.println(" Miercoles"); 
      break;
    case 4:
      Serial.println(" Jueves"); 
      break;
    case 5:
      Serial.println(" Viernes"); 
      break;
    case 6:
      Serial.println(" Sabado"); 
      break;
    case 7:
      Serial.println(" Domingo"); 
      break;
    default:
      Serial.println(" Day error");
  }
  
}
