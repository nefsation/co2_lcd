/*
 * Carbon Dioxide Parts Per Million Meter
 * CO2PPM
 * 
 * learnelectronics
 * 26 MAR 2017
 * 
 * www.youtube.com/c/learnelectronics
 * arduino0169@gmail.com
 */

/*
 * Atmospheric CO2 Level..............400ppm
 * Average indoor co2.............350-450ppm
 * Maxiumum acceptable co2...........1000ppm
 * Dangerous co2 levels.............>2000ppm
 */






//---------------------------------------------------------------------------------------------------------------
//                                                  LIBRARIES
//---------------------------------------------------------------------------------------------------------------
#include <Wire.h>                             //I2C for OLED
#include <Adafruit_GFX.h>                     //grafix library for OLED
#include <Adafruit_SSD1306.h>                 //OLED Driver
#include <LiquidCrystal.h>                    //
int backLight = 13;    // pin 13 will control the backlight
#include <DHT.h> // load lib


//---------------------------------------------------------------------------------------------------------------
//                                                   DEFINES
//---------------------------------------------------------------------------------------------------------------
#define anInput     A0                        //analog feed from MQ135
#define digTrigger   2                        //digital feed from MQ135
#define co2Zero     55                        //calibrated CO2 0 level
#define led          9                        //led on pin 9
#define OLED_RESET   4                        //OLED reset on lin 4
#define dht_apin 6 
#define dht_type DHT11
DHT dht(dht_apin, dht_type); // sensor pin S to pin11

//---------------------------------------------------------------------------------------------------------------
//                                                LIBRARY CALL
//---------------------------------------------------------------------------------------------------------------
Adafruit_SSD1306 display(OLED_RESET);         //create instance of OLED called display
LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);

//---------------------------------------------------------------------------------------------------------------
//                                                  SETUP
//---------------------------------------------------------------------------------------------------------------
void setup() {
  
  pinMode(anInput,INPUT);                     //MQ135 analog feed set for input
  pinMode(digTrigger,INPUT);                  //MQ135 digital feed set for input
  //pinMode(led,OUTPUT);                        //led set for output
  Serial.begin(9600);                         //serial comms for debuging
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  //begin display @ hex addy 0x3C
  display.display();                          //show buffer
  display.clearDisplay();                     //clear buffer
  pinMode(backLight, OUTPUT); //backlightlcd
  lcd.begin(20,4); // to tell arduino there are 16 colunms and 2 rows
  
 
  
}
//---------------------------------------------------------------------------------------------------------------
//                                               MAIN LOOP
//---------------------------------------------------------------------------------------------------------------
void loop() {

  int backLight = 13;    // pin 13 will control the backlight
  int co2now[10];                               //int array for co2 readings
  int co2raw = 0;                               //int for raw value of co2
  int co2comp = 0;                              //int for compensated co2 
  int co2ppm = 0;                               //int for calculated ppm
  int zzz = 0;                                  //int for averaging
  int grafX = 0;                                //int for x value of graph


  digitalWrite(backLight, HIGH); // turn backlight on. Replace 'HIGH' with 'LOW' to turn it off.


  display.clearDisplay();                     //clear display @ beginning of each loop

  for (int x = 0;x<10;x++){                   //samplpe co2 10x over 2 seconds
    co2now[x]=analogRead(A0);
    delay(40);
  }

for (int x = 0;x<10;x++){                     //add samples together
    zzz=zzz + co2now[x];
    
  }
  co2raw = zzz/10;                            //divide samples by 10
  co2comp = co2raw - co2Zero;                 //get compensated value
  co2ppm = map(co2comp,0,1023,400,5000);      //map value for atmospheric levels



float h = dht.readHumidity();
float f = dht.readTemperature();
lcd.setCursor(0,0); // row 1
lcd.print(" Humidity = "); // print on the lcd Humidity = 
lcd.print(h); // to print the humidity
lcd.setCursor(16,0); //set cursor just after humidity integer (no decimals) 
lcd.print("% "); //print "percentage" symbol

lcd.setCursor(0,1);
lcd.print(" Temp = "); // print on the lcd temp = 
lcd.print(f); // print on the lcd temperture
lcd.setCursor(10,1); //take out deceimal places on temp
lcd.print((char)223); // print "degrees" symbol 
lcd.println("C"); // print "C" with spaces after to fill 16 character line
lcd.setCursor(0, 2);
lcd.print ("CO2 Level:");               //print title
lcd.print (co2ppm);
lcd.print(" ");




  display.setTextSize(2);                     //set text size
  display.setTextColor(WHITE);                //set text color
  display.setCursor(0,0);                     //set cursor
  display.println("CO2 Level:");               //print title
  //display.println("");                          //skip a line
  display.print(co2ppm);                      //print co2 ppm
  display.print(" PPM");                      //print units
  grafX = map(co2ppm,0,1000,0,127);           //map value to screen width
  display.fillRect(0, 52, grafX, 10, WHITE);  //print graph 400min 1000max
  display.display();                          //show the buffer
  Serial.println (co2ppm);                     //
  if(co2ppm>999){                             //if co2 ppm > 1000
    digitalWrite(led,HIGH);                   //turn on led
  }
  else{                                       //if not
    digitalWrite(led,LOW);                    //turn off led
  }
}
