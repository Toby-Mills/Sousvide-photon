https://api.particle.io/v1/devices/380042001247343339383037/setTemp
HEADERS:
Authorization: Bearer 98f018d520a9877ca6939bfb49e0d1d6923f1ce7
Content-Type: application/json
BODY:
{"arg":"28"}

Next up:
Publish Variables on any change
Ability to Start & Stop cooker
Ability to set total time
/*
 * Project photon
 * Description:
 * Author:
 * Date:
 */


 // This #include statement was automatically added by the Particle IDE.
#include "OneWire.h"

#include "spark-dallas-temperature/spark-dallas-temperature.h"

#include "LiquidCrystal_I2C_Spark.h"

#include <math.h>
#include <string.h>
LiquidCrystal_I2C *lcd;


SYSTEM_THREAD(ENABLED);


int ONE_WIRE_BUS = A5;
 // Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);
 //OneWire ds = oneWire(ONE_WIRE_BUS);

 // Pass our oneWire reference to Dallas Temperature.
 DallasTemperature sensors(&oneWire);

 DeviceAddress thermometer1 = { 0x28, 0x94, 0xE2, 0xDF, 0x02, 0x00, 0x00, 0xFE };





 double desiredTemperature;
 char *messageTopLine = "Current";
 char *messageBottomLine = "Target";
  int activeTime = 0;
 char debug[64] = "none";


 //float getTemperature(DeviceAddress deviceAddress);


 unsigned long lastTempRequest = 0;
 unsigned long tempDelayInMillis = 1000;


 unsigned long lastUpdate = 0;
 unsigned long delayInMillis = 100;


 unsigned long lastRelayRequest = 0;
 unsigned long relayDelayInMillis = 5000;


 unsigned long screenRedrawRelayRequest = 0;
 unsigned long screenRedrawDelayInMillis = 600000;

 bool atTemp = false;

 float temperature = 0;
 bool makeChange = true;
 bool redrawScreen = true;
 int redrawScreeni = 1;


bool connectedOnce = false;

 // Data wire is plugged into pin 4 on the Arduino
//  #define ONE_WIRE_BUS 4
 // #define sousVidePower 3
 // int sousvidePower = D3;
 //int ledPin = D7;
 int temp = 50;
 int ctemp = 0;
 int ctempm = 0;
int storedTemp;
 long hour = 3600000; // 3600000 milliseconds in an hour
 long minute = 60000; // 60000 milliseconds in a minute
 long second =  1000; // 1000 milliseconds in a second
 long offset = 0;

 int uptime = 0;
 int atime = 0;

int relayPin = D4;

// setup() runs once, when the device is first turned on.
void setup() {
      // Put initialization like pinMode and begin functions here.
    //  Serial.begin(9600);


    //  Sets the initial temperature, first from flash, then from default
    //   Perfectly done medium steak done at 58
    //   Pork 12-hour Ribs done at 74
    Serial.begin(9600);
    Serial.print("Hello");

    Particle.function("setTemp", setCloudTemp);

    EEPROM.get(10,desiredTemperature);
    if (desiredTemperature == 0xFFFF) {
      // Stored memory is empty, so set it to default temp
      int defaultTemp = 26;
      desiredTemperature = defaultTemp;
      EEPROM.put(10, defaultTemp);
    }else{

    }
      pinMode(ONE_WIRE_BUS, INPUT);
      pinMode(relayPin, OUTPUT);
      pinMode(D7, OUTPUT);
      //swith the relay HIGH to prevent the Relay from powering on
      digitalWrite(relayPin, HIGH);
       digitalWrite(D7,HIGH);
      //initialize the temperature sensor
      sensors.begin();

      // set the resolution to 10 bit (good enough?)
      sensors.setResolution(thermometer1, 12);



       lcd = new LiquidCrystal_I2C(0x27, 16 , 2);
       lcd->init();
       lcd->backlight();
       lcd->clear();
       lcd->setCursor(11,0);



       temp = desiredTemperature;

}

int setCloudTemp(String temp) {
  desiredTemperature = temp.toFloat();
  EEPROM.put(10, desiredTemperature);
  lcd->setCursor(11,1);
  lcd->print(desiredTemperature);
  //debug("setCloudTemp", "%d", desiredTemperature);
  return desiredTemperature;
}

int getSavedTemp() {
int val;
  return EEPROM.get(10, val);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.



//experimental SYSTEM_THREAD(ENABLED) code to register cloud functions once the particle is connected
  if (connectedOnce == false) {
    if (Particle.connected()) {
      Particle.variable("temp", desiredTemperature);


      Particle.variable("ctemp", (double)temperature);

      //Particle.function("setTemp", setCloudTemp);
      //Particle.function("getSavedTemp", getSavedTemp);
      connectedOnce = true;
    }
  }


  if ( millis()  - lastUpdate >= delayInMillis) {
    float sensorTemperature;
    sensorTemperature = getSensorTemperature();
    if(sensorTemperature > -127){
      temperature = sensorTemperature;
    }


      //ctemp = floor(temperature);
      //ctempm = temperature * pow(10,2) - ctemp * pow(10,2);

      lastUpdate = millis();

      lcd->setCursor(11,0);
     lcd->print(temperature);

   //lcd->setCursor(0,1);
   //lcd->print(time());


   uptime = millis();

    if ((atTemp == false) && (temperature >= desiredTemperature)) {
        atTemp = true;
        offset = millis();
    }

    if (atTemp == true) {
        atime = (millis()-offset);
    }
  }

  if (( millis()  - lastRelayRequest >= relayDelayInMillis) || makeChange == true) {

Serial.print("here");
Serial.print(temperature);
Serial.print(desiredTemperature);
Serial.println("");
      if ((temperature +0.2) < desiredTemperature) {
          //switch on the relay
          digitalWrite(relayPin, LOW);
           digitalWrite(D7,LOW);
      }
      else {
          digitalWrite(relayPin, HIGH);
           digitalWrite(D7,HIGH);
      }

      lastRelayRequest  = millis();
      makeChange = false;
  }


  if (( millis()  - screenRedrawRelayRequest >= screenRedrawDelayInMillis) || (redrawScreeni == 1)){





      lcd->clear();
      lcd->setCursor(0,0);
      lcd->print(messageTopLine);
      lcd->setCursor(0,1);
      lcd->print(messageBottomLine);
      lcd->setCursor(11,1);
      lcd->print(desiredTemperature);
    //  lcd->setCursor(0,1);
   //   lcd->print("Uptime");


      screenRedrawRelayRequest = millis();
      redrawScreeni = 0;
  }



//  digitalWrite(relayPin, LOW);

}


float getSensorTemperature()
{

    if (millis() - lastTempRequest >= delayInMillis) {
        sensors.requestTemperatures();
        float tempC = sensors.getTempCByIndex(0);
        //float tempC = sensors.getTempC(deviceAddress);

        //Serial.print(DallasTemperature::toFahrenheit(tempC));
        lastTempRequest = millis();
        //Serial.print(tempC );
        return tempC;
  }

    return temperature;
}


int setTitle(String args) {
    // command.toCharArray(message,command.length());
    char charBuf[20];
    //lcd->clear();
    //lcd->setCursor(0,0);
    args.toCharArray(charBuf, 20);
    strcpy(debug,charBuf);
    //lcd->print(args);
    //lcd->setCursor(0,1);
   // lcd->print("Uptime");
     return 1;

}

String time(){
long timeNow = millis();

                                //number of days
int hours = timeNow / hour;                       //the remainder from days division (in milliseconds) divided by hours, this gives the full hours
int minutes = ((timeNow ) % hour) / minute ;         //and so on...
int seconds = (((timeNow ) % hour) % minute) / second;

 // digital clock display of current time
 String f = "";
 f += printDigits(hours);
 f += ":";
 f += printDigits(minutes);
 f += ":";
 f += printDigits(seconds);

return f;

}


String printDigits(byte digits){
 // utility function for digital clock display: prints colon and leading 0
 String t = "";

 if(digits < 10)
   t += '0';
  t += digits;
  return t;
}
