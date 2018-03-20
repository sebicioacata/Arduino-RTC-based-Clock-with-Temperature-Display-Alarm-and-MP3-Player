#include "SoftwareSerial.h"
SoftwareSerial mySerial(14, 15);

//#include <TimeLib.h>
# define Start_Byte 0x7E
# define Version_Byte 0xFF
# define Command_Length 0x06
# define End_Byte 0xEF
# define Acknowledge 0x00 //Returns info with command 0x41 [0x01: info, 0x00: no info]


#include <Wire.h>
#include <SPI.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_PCD8544.h>
#include <RTClib.h>
#include <dht.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 9,8,7,6);
RTC_DS1307 RTC;
#define DHT11_PIN 5
dht DHT;
int temperatura=0;
volatile boolean isPlaying = false;
static const unsigned char PROGMEM clock[] =
{
B00000000,B11111100,B00000000,
B00000011,B00000011,B00000000,
B00001100,B00000000,B11000000,
B00010000,B00000000,B00100000,
B00100000,B00000001,B00100000,
B00100000,B00000010,B00010000,
B01000000,B00000010,B00001000,
B01000000,B00000100,B00001000,
B10001100,B00001000,B00000100,
B10000011,B00001000,B00000100,
B10000000,B11010000,B00000100,
B10000000,B00110000,B00000100,
B10000000,B00000000,B00000100,
B10000000,B00000000,B00000100,
B01000000,B00000000,B00001000,
B01000000,B00000000,B00001000,
B00100000,B00000000,B00010000,
B00100000,B00000000,B00010000,
B00011000,B00000000,B00100000,
B00000100,B00000000,B11000000,
B00000011,B00000011,B00000000,
B00000000,B11111100,B00000000, };


int temp;
int setDateTime = 0;
int setAlarm = 0;
int format = 0;
int zile_luna [] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; 
int day, month, year, hour, minute, second;
int alarmHour = 0; 
int alarmMinute = 0;
int formattedHour;
int volume = 0;
int buzzerPin = 10;
int alarmActive = 0;


int readFirstButton(){
  delay(50);
  return digitalRead(2);
}

int readSecondButton(){
  delay(50);
  return digitalRead(3);
}

int readThirdButton(){
  delay(50);
  return digitalRead(4);
}


void setup() {

digitalWrite(14,HIGH);

digitalWrite(15,HIGH);
analogWrite(13, 20);

    lcd.begin(16,2);
   Wire.begin();
   RTC.begin();
   //RTC.adjust(DateTime(__DATE__,__TIME__));
   pinMode(2,INPUT_PULLUP);
   pinMode(3,INPUT_PULLUP);
   pinMode(4, INPUT_PULLUP);
   pinMode(buzzerPin,OUTPUT);
   Serial.begin(9600);
   mySerial.begin(9600);
}



void loop() {   
  //lcd.clear();
 int chk = DHT.read11(DHT11_PIN);
  lcd.setCursor(11,0); 
temperatura=DHT.temperature;
if(temperatura >0 & temperatura <50){
  lcd.print(temperatura);
  lcd.print((char)223);
  lcd.print("C ");
}

  DateTime now = RTC.now();
  Serial.println(setAlarm);
    
  day = now.day();
  month = now.month();
  year = now.year();
  hour = now.hour();
  minute = now.minute();
  second = now.second();

  if(format == 0) formattedHour = hour;
  else{
    if(hour == 0 || hour == 12) formattedHour = 12;
    else formattedHour = hour % 12;
  }

  if(hour == alarmHour && minute == alarmMinute && alarmActive == 1){    //see if it is time to ring the alarm
tone(10,500);
delay(1000);
  }
//  if(readSecondButton() == 0 && alarmActive == 1) alarmActive = 0;

 // display.drawBitmap(60, 12, clock, 22, 22, 1);

  
  lcd.setCursor(0,1);
  if(formattedHour < 10) lcd.print("0");
  lcd.print(formattedHour, DEC);
  lcd.print(":");
  if(minute < 10) lcd.print("0");
  lcd.print(minute, DEC);
  lcd.print(":");
  if(second < 10) lcd.print("0");
  lcd.print(second, DEC);
  if(format == 1 && hour >= 12) if(setDateTime==0&&setAlarm==0){lcd.print(" PM     ");}
  if(format == 1 && hour < 12)  if(setDateTime==0&&setAlarm){lcd.print(" AM   ");}
  
  //lcd.setTextSize(1);
  lcd.setCursor(0,0);
  lcd.print(day, DEC);
  lcd.print("/");
  lcd.print(month, DEC);
  lcd.print("/");
  lcd.print(year, DEC);
  lcd.print("  ");

  
 

  if(readFirstButton() == 0 && setAlarm == 0){
    if(setDateTime < 8) setDateTime++;
    else setDateTime = 0;
  }

  if(readThirdButton() == 0 && setDateTime == 0){
    if(setAlarm < 4) setAlarm ++;
    else setAlarm = 0;
  }
/*lcd.setCursor(0,0);
     lcd.print("  Set Alarm  ");
    delay(2000);
    defualt();
    time();
    delay(1000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("  Alarm time ");
    lcd.setCursor(0,1);
    lcd.print(" has been set  ");
    delay(2000);
  */

  if(setDateTime == 0){
    if(setAlarm != 0){
      
    
      switch(setAlarm){
        case 1:
        lcd.setCursor(9,1);
        lcd.print(" Alarm ");
        delay(100);
        break;
    
        case 2:
        lcd.setCursor(9,1);
          lcd.print("ORA: ");
          if(readFirstButton() == 0){
            alarmHour ++;
            if(alarmHour > 23) alarmHour = 0;
          }
          if(readSecondButton() == 0){
            alarmHour --;
            if(alarmHour < 0) alarmHour = 23;
          }
         
          lcd.setCursor(13,1);
          lcd.print(alarmHour, DEC);
            lcd.print("        ");
                
          break;

         case 3:
         lcd.setCursor(9,1);
          lcd.print("MIN:");
          if(readFirstButton() == 0){
            alarmMinute ++;
            if(alarmMinute > 59) alarmMinute = 0;
          }
          if(readSecondButton() == 0){
            alarmMinute --;
            if(alarmMinute < 0) alarmMinute = 59;
          }
          lcd.setCursor(13,1);
          //if(alarmMinute < 10) lcd.print("0");
          lcd.print(alarmMinute,DEC);
           lcd.print("    ");
          
          break;

          case 4:
          lcd.setCursor(9,1);
          lcd.print("Y/N?     ");
            if(readFirstButton() == 0){
              alarmActive = 1;
              
            }
            if(readSecondButton() == 0){
              alarmActive = 0;
            }
            setAlarm ++;
            delay(1000);
            lcd.clear();
              lcd.setCursor(6,1);
              lcd.print("Alarm set");
            delay(1000);
            lcd.clear();
            break;
            
            
      }
    }
  }

  if(setAlarm == 0){
    if(setDateTime != 0){
      lcd.setCursor(9,1);
      
      switch (setDateTime){
        case 1:
          lcd.print("SET Z  ");
          if(readSecondButton() == 0){
            day ++;
            if(day > zile_luna[now.month()-1]) day = 1;
            RTC.adjust(DateTime(year, month, day, hour, minute, second));
          }
          if(readThirdButton() == 0){
            day --;
            if(day == 0) day = zile_luna[now.month()-1];
            RTC.adjust(DateTime(year, month, day, hour, minute, second));
          }
          break;
        case 2:
          lcd.print("SET L  ");
          if(readSecondButton() == 0){
            month ++;
            if(month > 12) month = 1;
            RTC.adjust(DateTime(year, month, day, hour, minute, second));
          }
          if(readThirdButton() == 0){
            month --;
            if(month == 0) month = 12;
            RTC.adjust(DateTime(year, month, day, hour, minute, second));
          }
          break;
        case 3:
          lcd.print("SET A  ");
          if(readSecondButton() == 0){
            year++;
            RTC.adjust(DateTime(year, month, day, hour, minute, second));
          }
          if(readThirdButton() == 0){
            year--;
            RTC.adjust(DateTime(year, month, day, hour, minute, second));
          }
          break;
        case 4:
          lcd.print("SET O  ");
          if(readSecondButton() == 0){
            hour ++;
            if(hour > 23) hour = 0;
            RTC.adjust(DateTime(year, month, day, hour, minute, second));
          }
          if(readThirdButton() == 0){
            hour --;
            if(hour == -1) hour = 23;
            RTC.adjust(DateTime(year, month, day, hour, minute, second));
          }
          break;
        case 5:
          lcd.print("SET M  ");
          if(readSecondButton() == 0){
            minute ++;
            if(minute > 59) minute = 0;
            RTC.adjust(DateTime(year, month, day, hour, minute, second));
          }
          if(readThirdButton() == 0){
            minute --;
            if(minute == -1) minute = 59;
            RTC.adjust(DateTime(year, month, day, hour, minute, second));
          }
          break;
        case 6:
          lcd.print("SET S  ");
          if(readSecondButton() == 0){
            second ++;
            if(second > 59) second = 0;
            RTC.adjust(DateTime(year, month, day, hour, minute, second));
          }
          if(readThirdButton() == 0){
            second --;
            if(second == -1) second = 59;
            RTC.adjust(DateTime(year, month, day, hour, minute, second));
          }
          break;
        case 7:
          lcd.print("12/24");
          if(readSecondButton() == 0){
            if(format == 0) format = 1;
            else format = 0;
          }
         
          
          break;
          case 8:
          setDateTime = 0;
           delay(500);
            lcd.setCursor(6,1);
              lcd.print("          ");
              break;
      }
    }
  }
  
   lcd.display();


if(readSecondButton() == 0){
            if(isPlaying)
    {
      pause();
      isPlaying = false;
    }else
    {
      isPlaying = true;
      play();
    }
          }
}

void pause()
{
  execute_CMD(0x0E,0,0);
  delay(500);
}

void play()
{
  execute_CMD(0x0D,0,1); 
  delay(500);
}

void playNext()
{
  execute_CMD(0x01,0,1);
  delay(500);
}

void playPrevious()
{
  execute_CMD(0x02,0,1);
  delay(500);
}

void setVolume(int volume)
{
  execute_CMD(0x06, 0, volume); // Set the volume (0x00~0x30)
  delay(2000);
}

void execute_CMD(byte CMD, byte Par1, byte Par2)
// Excecute the command and parameters
{
// Calculate the checksum (2 bytes)
word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
// Build the command line
byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};
//Send the command line to the module
for (byte k=0; k<10; k++)
{
mySerial.write( Command_line[k]);
}
}

