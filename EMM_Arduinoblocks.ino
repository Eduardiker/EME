#include <SoftwareSerial.h>

#include <Wire.h>
#include "ABlocks_LiquidCrystal_I2C.h"
#include "ABlocks_DHT.h"

double Aire;
double Temperatura;
double Humitat;
double Luz;
double Blue;

SoftwareSerial bt_serial(7,8);
LiquidCrystal_I2C lcd(0x27,16,2);
DHT dht4(4,DHT11);
unsigned long task_time_ms=0;

void fnc_bt_serial_namepin(String _name,String _pin){
	delay(2000);
	bt_serial.print(String("AT+NAME")+_name);
	delay(1000);
	bt_serial.print(String("AT+PIN")+_pin);
	delay(1000);
	while(bt_serial.available()>0)bt_serial.read();
	bt_serial.flush();
}

void setup()
{
  Serial.begin(9600); Serial.flush(); while(Serial.available()>0)Serial.read();

bt_serial.begin(9600);

lcd.begin();lcd.noCursor();lcd.backlight();
pinMode(A0, INPUT);
pinMode(4, INPUT);
dht4.begin();
pinMode(A3, INPUT);
  fnc_bt_serial_namepin(String("EMM"),String("1234"));
  Aire = map(analogRead(A0),0,1023,0,100);
  Temperatura = dht4.readTemperature();
  Humitat = dht4.readHumidity();
  Aire = map(analogRead(A3),0,1023,0,100);

}


void loop()
{

    Aire = map(analogRead(A0),0,1023,0,100);
    Temperatura = dht4.readTemperature();
    Humitat = dht4.readHumidity();
    Aire = map(analogRead(A3),0,1023,0,100);
    Serial.println(String(Temperatura)+String("Grados"));
    Serial.println(String(Humitat)+String("Hu"));
    Serial.println(String(Aire)+String("Air"));
    Serial.println(String(Luz)+String("Luz"));
    Serial.println(String("------------------------------------"));
    if ((bt_serial.available()>0)) {
      Blue = atof((bt_serial.readStringUntil('\n')).c_str());

    }
    if ((Blue == 0)) {
      bt_serial.println(String("Conectat correctament"));

    }
    delay(500);
    if ((Blue == 1)) {
      bt_serial.println(String(Luz)+String("Luz"));

    }
    delay(500);
    if ((Blue == 2)) {
      bt_serial.println(String(Temperatura)+String("Graus"));

    }
    delay(500);
    if ((Blue == 3)) {
      bt_serial.println(String(Humitat)+String("Humitat"));

    }
    delay(500);
    if ((Blue == 4)) {
      bt_serial.println(String(Aire)+String("Aire"));

    }
    delay(500);
    if ((Blue == 5)) {
      bt_serial.println(String(Temperatura)+String("Graus"));
      bt_serial.println(String(Humitat)+String("Humitat"));
      bt_serial.println(String(Aire)+String("Aire"));

    }
    bt_serial.println(String("------------------------------------"));
    lcd.setCursor(0, 0);
    lcd.print(String("Air ="));
    lcd.setCursor(6, 0);
    lcd.print((String(Aire,0)));
    lcd.setCursor(10, 0);
    lcd.print(String("Hum="));
    lcd.setCursor(14, 0);
    lcd.print((String(Humitat,0)));
    lcd.setCursor(0, 1);
    lcd.print(String("Temp="));
    lcd.setCursor(6, 1);
    lcd.print((String(Temperatura,0)));
    lcd.setCursor(8, 1);
    lcd.print(String("C"));
    if((millis()-task_time_ms)>=5000){
    	task_time_ms=millis();
      lcd.clear();
    }

}
