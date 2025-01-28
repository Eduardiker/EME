#include "ABlocks_DHT.h"
#include <Wire.h>
#include "ABlocks_DS3231rtc.h"
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>

double Pluja;
double LLum;
double Temperatura;
double Humitat;
double Aire;
double Port;
double Blue;
DHT dhtA3(A3,DHT11);
DS3231rtc rtc;
unsigned long task_time_ms=0;

File sd_file;
SoftwareSerial bt_serial(3,2);

String rtc_date_text(){
	char rtcbuffer[12];
	sprintf(rtcbuffer,"%02d/%02d/%04d", rtc.getDay(), rtc.getMonth(), (2000+rtc.getYear()) );
	return String(rtcbuffer);
}

String rtc_time_text(){
	char rtcbuffer[10];
	sprintf(rtcbuffer,"%02d:%02d:%02d", rtc.getHour(), rtc.getMinute(), rtc.getSecond() );
	return String(rtcbuffer);
}

void fnc_sd_print(String _f, String _t, boolean _nl){
	sd_file = SD.open(_f, FILE_WRITE);
	if(sd_file){
		sd_file.seek(sd_file.size());
		if(_nl)sd_file.println(_t);
		else sd_file.print(_t);
		sd_file.close();
	}
}

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
  	pinMode(A1, INPUT);
	pinMode(A3, INPUT);
	pinMode(A0, INPUT);
	pinMode(A2, INPUT);

	dhtA3.begin();
	Serial.begin(9600);
	Serial.flush();
	while(Serial.available()>0)Serial.read();

	rtc.begin();
bt_serial.begin(9600);

	fnc_bt_serial_namepin(String("EME"),String("1234"));
	SD.begin(10);
	rtc.setTime(0,0,16,20,9,21);
	Pluja = map(analogRead(A1),0,1023,0,100);
	Temperatura = dhtA3.readTemperature();
	Humitat = dhtA3.readHumidity();
	LLum = map(analogRead(A0),0,1023,0,100);
	Aire = map(analogRead(A2),0,1023,0,100);

}


void loop()
{

  	Pluja = map(analogRead(A1),0,1023,0,100);
  	Temperatura = dhtA3.readTemperature();
  	Humitat = dhtA3.readHumidity();
  	LLum = map(analogRead(A0),0,1023,0,100);
  	Aire = map(analogRead(A2),0,1023,0,100);
  	Port = 0;
  	Blue = 0;
  	if((millis()-task_time_ms)>=30000){
  		task_time_ms=millis();
  		Serial.println(String("Info Clima"));
  		Serial.println(String(""));
  		if ((Pluja > 0)) {
  			Serial.println(String(rtc_date_text())+String(",")+String(rtc_time_text())+String(",")+String("Percentage de pluja =")+String(Pluja)+String("%"));
  		}

  		Serial.println(String(rtc_date_text())+String(",")+String(rtc_time_text())+String(",")+String("Temperatura =")+String(Temperatura));
  		Serial.println(String(rtc_date_text())+String(",")+String(rtc_time_text())+String(",")+String("Humitat")+String(Humitat));
  		Serial.println(String(rtc_date_text())+String(",")+String(rtc_time_text())+String(",")+String("Nivell de llum =")+String((map(LLum, 0,1023,0,100))));
  		Serial.println(String(rtc_date_text())+String(",")+String(rtc_time_text())+String(",")+String("Qualitat de l\'aire =")+String(Aire));
  		Serial.println(String("-----------------------------------------------"));
  	}
  	delay(30000);
  	fnc_sd_print(String("Temp.txt"),String(String(rtc_date_text())+String(",")+String(rtc_time_text())+String(",")+String("Temperatura =")+String(Temperatura)),true);
  	fnc_sd_print(String("Hum.txt"),String(String(rtc_date_text())+String(",")+String(rtc_time_text())+String(",")+String("Humitat")+String(Humitat)),true);
  	fnc_sd_print(String("Llum.txt"),String(String(rtc_date_text())+String(",")+String(rtc_time_text())+String(",")+String("Nivell de llum =")+String((map(LLum, 0,1023,0,100)))),true);
  	fnc_sd_print(String("Aire.txt"),String(String(rtc_date_text())+String(",")+String(rtc_time_text())+String(",")+String("Qualitat de l\'aire =")+String(Aire)),true);
  	if ((Pluja > 0)) {
  		for (int count = 0; count < 5; count++) {
  			fnc_sd_print(String("Pluja.txt"),String(String(rtc_date_text())+String(",")+String(rtc_time_text())+String(",")+String("Percentage de pluja =")+String(Pluja)+String("%")),true);
  		}
  	}

  	if ((bt_serial.available()>0)) {
  		Blue = atof((bt_serial.readStringUntil('\n')).c_str());
  	}

  	if ((Blue == 1)) {
  		bt_serial.println(String("Info Clima"));
  		bt_serial.println(String(""));
  		bt_serial.println(String(rtc_date_text())+String(",")+String(rtc_time_text())+String(",")+String("Temperatura =")+String(Temperatura));
  		bt_serial.println(String(rtc_date_text())+String(",")+String(rtc_time_text())+String(",")+String("Humitat =")+String(Humitat));
  		bt_serial.println(String(rtc_date_text())+String(",")+String(rtc_time_text())+String(",")+String("Nivell de llum =")+String((map(LLum, 0,1023,0,100))));
  		bt_serial.println(String(rtc_date_text())+String(",")+String(rtc_time_text())+String(",")+String("Qualitat de l\'aire =")+String(Aire));
  		if ((Pluja > 0)) {
  			bt_serial.println(String(rtc_date_text())+String(",")+String(rtc_time_text())+String(",")+String("Percentage de pluja =")+String(Pluja)+String("%"));
  		}

  		bt_serial.println(String("-----------------------------------------------"));
  		Blue = 0;
  	}

}
