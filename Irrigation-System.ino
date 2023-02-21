/*
 * Created by 
 * Ark Bramwell, July 2010
 * Modified by keen 
 * Date: 05/05/2017
 * Function: this program will test the LCD panel and the buttons
 */ 
//Sample using LiquidCrystal library
#include <LiquidCrystal.h>
#include <DHT.h>
#include <Wire.h>
int DHTPIN = A3;
#define RELAY1 2
#define RELAY2 3


#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);

float tempValue = 29;
int umiditate1 = A1;
int sensorPin = A2;
int umiditate2 = A4;

int thresholdValue = 800;
int lightValue = 0;
int lightOnThreshold = 300;

int p1=0;
int p2=0;
float t;
 
// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
 
// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5


// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1500) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 500)  return btnLEFT; 
 if (adc_key_in < 800)  return btnSELECT;   
 return btnNONE;  // when all others fail, return this...
}
 
void setup()
{
 pinMode(sensorPin, INPUT);
 pinMode(umiditate1, INPUT);
 pinMode(RELAY1, OUTPUT);
 pinMode(RELAY2, OUTPUT);
 Serial.begin(9600);
 dht.begin();
 Wire.begin();
 lcd.begin(16, 2);              
 
}



void loop()
{
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  
  lcd.setCursor(0,0);
  lcd.print("H1:");
  lcd.setCursor(7,0);
  lcd.print("Sun:");
  lcd.setCursor(0,1);
  lcd.print("H2:");
  lcd.setCursor(7,1);
  lcd.print("T:");

  //temperatura
  t = dht.readTemperature();
  Serial.println(t);
  lcd.setCursor(9,1);
  lcd.print(t);
  lcd.print("C");
  
  //lumina
  lightValue = analogRead(sensorPin);
  lcd.setCursor(11,0);
  lcd.print("    ");
  
  lcd.setCursor(11,0);
  if( lightValue < lightOnThreshold){
      lcd.print("up");
    } 
    else {
      lcd.print("down");
    }

  //umiditate
  float sensorValue1 = analogRead(umiditate1);
  sensorValue1 = sensorValue1 + 100 / 664;
  p1=100-((sensorValue1/1023)*100);
  lcd.setCursor(3,0);
  lcd.print("  ");
  
  lcd.setCursor(3,0);
  lcd.print(p1);
  
  lcd.setCursor(5,0);
  lcd.print("%");

  float sensorValue2 = analogRead(umiditate2);
  sensorValue2 = sensorValue2 + 100 / 664;
  p2=100-((sensorValue2/1023)*100);

  lcd.setCursor(3,1);
  lcd.print("  ");
  
  lcd.setCursor(3,1);
  lcd.print(p2);
  
  lcd.setCursor(5,1);
  lcd.print("%");

  
  delay(5000);

  lcd.clear();               

  delay(1000);

  lcd.setCursor(3, 0);
  lcd.print("Zone 1:OFF");
  lcd.setCursor(3, 1);
  lcd.print("Zone 2:OFF");

  if(sensorValue1 > thresholdValue && t < tempValue && lightValue > lightOnThreshold){
    lcd.setCursor(3, 0);
    lcd.print("          ");
    lcd.setCursor(3, 0);
    lcd.print("Zone 1:ON");
    digitalWrite(RELAY1, LOW);
    delay(5000);
    lcd.setCursor(3, 0);
    lcd.print("Zone 1:OFF");
    digitalWrite(RELAY1, HIGH);

    
    }
  else{
    digitalWrite(RELAY1, HIGH);
    lcd.setCursor(3, 0);
    lcd.print("Zone 1:OFF");
  }
  
    delay(2000);
  
  if(sensorValue2 > thresholdValue && t < tempValue && lightValue > lightOnThreshold){
    lcd.setCursor(3, 1);
    lcd.print("          ");
    lcd.setCursor(3, 1);
    lcd.print("Zone 2:ON");
    digitalWrite(RELAY2, LOW);
    delay(5000);
    lcd.setCursor(3, 1);
    lcd.print("Zone 2:OFF");
    digitalWrite(RELAY2, HIGH);
  }else {
    delay(500);
    lcd.setCursor(3, 1);
    lcd.print("Zone 2:OFF");
    digitalWrite(RELAY2, HIGH);
  }

  delay(2000);
  lcd.clear();               

  delay(1000);
}
