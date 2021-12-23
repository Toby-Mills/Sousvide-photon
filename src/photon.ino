//--------------------------------------------------------------
// Included libraries
//--------------------------------------------------------------
#include "config.h" //add your own config.h with: char const blynkToken[] = "your-blynk-token-here"
#include "OneWire.h"
#include "spark-dallas-temperature/spark-dallas-temperature.h"
#include "LiquidCrystal_I2C_Spark.h"
#include <math.h>
#include <string.h>
#include <Encoder.h>
#include <blynk.h>
#include "MQTT.h"

//--------------------------------------------------------------
// Variables
//--------------------------------------------------------------
SYSTEM_THREAD(ENABLED); //enables system functions to happen in a separate thread from the application setup and loop
//this includes connecting to the network and the cloud

char *sourceCode = "https://github.com/Toby-Mills/Sousvide-photon";

int defaultTargetTemperature = 60;
double targetTemperature;
double currentTemperature = 0; //current temperature

char *messageCurrentTemp = "Current";
char *messageTargetTemp = "Target";
char *messageSetTemp = "Set Target";

unsigned long lastSensorReading = 0; //time of last sensor reading
unsigned long sensorReadingDelay = 1000; //minimum milliseconds between processing successive temperature readings

unsigned long lastRelayCommand = 0; //time of last command to relay
unsigned long relayCommandDelay = 5000; //minimum milliseconds between succesive commands to relay

unsigned long lastScreenRedraw = 0; //time of last screen redraw
unsigned long screenRedrawDelay = 600000; //minimum milliseconds between successive redraws of screen

long encoderNewPosition = -999;
long encoderOldPosition  = -999;

int buttonReading = 0;
bool buttonClick = false;

bool setTempMode = false;
int newTargetTemp =  0;

bool timerStarted = false;
bool timerEnded = false;
unsigned long timerLength = 0;
unsigned long timerStartTime = 0;
unsigned long lastTimerUpdate = 0; //time of last timer update
unsigned long timerUpdateDelay = 1000; //minimum milliseconds between succesive timer updates

unsigned long lastStatusPublish = 0; //time of last time the status was published
unsigned long StatusPublishDelay = 10000; //minimum milliseconds between publishing status updates
//--------------------------------------------------------------
//Pin declarations
//--------------------------------------------------------------
int ONE_WIRE_BUS = A5;
int relayPin = D4;
int encoderPin1 = D3;
int encoderPin2 = D2;
int ledPowerPin = D7;
int ledRelayPin = D6;
int buttonPin = A0;

//--------------------------------------------------------------
// Blynk Pins
//--------------------------------------------------------------
int blynkPin_TargetTemp = 0;
int blynkPin_CurrentTemp = 1;
int blynkPin_RelayState = 2;
int blynkPin_TimeRemaining = 3;
int blynkPin_TimerLength = 4;

WidgetLED blynkLED_relay(V2);

//--------------------------------------------------------------
// MQTT
//--------------------------------------------------------------
bool mqttConnected = false;
byte mqttServer[] = {192, 168, 68, 135};
MQTT mqttClient(mqttServer, 1883, mqttCallback);
char *mqttTopic_cmnd_set_target = "cmnd/sous_vide/target";
char *mqttTopic_stat = "stat/sous_vide";
// char *mqttTopic_cmnd_start_timer = "cmnd/sous_vide/timer";
// char *mqttTopic_cmnd_stop = "cmnd/sous_vide/stop";

//--------------------------------------------------------------
// Initialize libraries
//--------------------------------------------------------------
LiquidCrystal_I2C *lcd;
 // Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
DeviceAddress thermometer1 = { 0x28, 0x94, 0xE2, 0xDF, 0x02, 0x00, 0x00, 0xFE };
Encoder encoder(encoderPin1, encoderPin2);

//--------------------------------------------------------------
// Setup
//--------------------------------------------------------------

// setup() runs once, when the device is first turned on.
void setup() {
  Particle.variable("sourceCode", sourceCode, STRING);
  Particle.variable("currentTemp", currentTemperature);
  Particle.variable("targetTemp", targetTemperature);
  Particle.function("setTemp", setDesiredTemperature_Cloud);
  Particle.variable("buttonRead", buttonReading);
  Particle.variable("buttonClick", buttonClick);
  Particle.function("setTimer", setTimer_Cloud);
  Particle.variable("timerLength", timerLength);

  // Set pin modes
  pinMode(ONE_WIRE_BUS, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(ledPowerPin, OUTPUT);
  pinMode(ledRelayPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLDOWN);

  digitalWrite(ledPowerPin, HIGH);

  loadDesiredTemperature();

  //initialize the temperature sensor
  sensors.begin();
  // set the resolution to 10 bit (good enough?)
  sensors.setResolution(thermometer1, 12);

  // initialize the lcd
  initializeDisplay();

  //initialize rotary encoder
  encoderOldPosition = encoder.read();

  //Create connection to Blynk Cloud
  Blynk.begin(blynkToken); 
  //update Blynk variables
  updateBlynkPins(); 
}

//--------------------------------------------------------------
// Loop
//--------------------------------------------------------------
void loop() {

  if (!mqttClient.isConnected())
  {
    mqttConnected = false;
    mqttClient.connect("sousVide");
  }
  else
  {
    if (!mqttConnected)
    {
      mqttConnected = true;
      mqttClient.publish("stat/sousVide", "connected");
      Particle.publish("Log Event", "mqtt connected");
      mqttClient.subscribe(mqttTopic_cmnd_set_target);
      // mqttClient.subscribe(mqttTopic_cmnd_start);
      // mqttClient.subscribe(mqttTopic_cmnd_start_timer);
      // mqttClient.subscribe(mqttTopic_cmnd_stop);
    }
    mqttClient.loop();
  }

  Blynk.run(); //read / write Blynk pins

  if (buttonClicked()){
    if(setTempMode == false){
        newTargetTemp = targetTemperature;
        setTempMode = true;
        redrawScreen();
    }else{
      setDesiredTemperature(newTargetTemp);
      setTempMode = false;
      redrawScreen();
    }
  }

  if(setTempMode==true){
    int encoderDirection = readEncoderDirection();
    newTargetTemp = newTargetTemp + encoderDirection;
    writeScreenBottomValue(newTargetTemp);
  }

  //process a sensor reading if the minimum time has elapsed
  if ( millis() - lastSensorReading >= sensorReadingDelay) {
    float sensorTemperature;
    sensorTemperature = getSensorTemperature();
    if(sensorTemperature > -127){
      currentTemperature = sensorTemperature;
      Blynk.virtualWrite(blynkPin_CurrentTemp, currentTemperature);
    }

    //record time of last sensor reading
    lastSensorReading = millis();

    //write the current temperature to the screen
    writeScreenTopValue(currentTemperature);
  }

  //publish a status update if needed
  if ( millis() - lastStatusPublish >= StatusPublishDelay) {
      char msg[50];
      sprintf(msg, "{\"currentTemp\": %0.2f, \"targetTemp\": %0.2f}", currentTemperature, targetTemperature);
      // Particle.publish("stat/sousvide",msg);
      mqttClient.publish(mqttTopic_stat, msg);
      lastStatusPublish = millis();
  }

  switchRelay();
  updateTimer();

  //redraw the screen if the minimum time has elapsed
  if (( millis()  - lastScreenRedraw >= screenRedrawDelay) ||(lastScreenRedraw == 0)){
    redrawScreen();
    //record time of latest screen redraw
    lastScreenRedraw = millis();
  }
//end of loop
}

//---------------------------------------------------------------
// Functions
//---------------------------------------------------------------

void publishMessage(String name, String message, int value)
{
  char msg[50];
  sprintf(msg, message.c_str(), value);
  Particle.publish(name, msg, 600, PRIVATE);
}

void updateTimer(){
  unsigned long timeRemaining;

  if (( millis() - lastTimerUpdate >= timerUpdateDelay) || (lastTimerUpdate == 0)) {
    if(timerLength > 0){
      if(timerStarted == false){
        if(currentTemperature > targetTemperature){
          timerStarted = true;
          timerStartTime = millis();
          Blynk.virtualWrite(blynkPin_TimeRemaining, "timer started");
        }else{
          Blynk.virtualWrite(blynkPin_TimeRemaining, "heating to target...");
        }
      }else{
        Blynk.virtualWrite(blynkPin_TimeRemaining, "calculating time remaining...");
        if(millis() - timerStartTime > timerLength){
          timerEnded = true;
          Blynk.virtualWrite(blynkPin_TimeRemaining, "timer finished");
        }else{
          timeRemaining = calculateTimeRemaining();
          Blynk.virtualWrite(blynkPin_TimeRemaining, formatTime(calculateTimeRemaining()));
        }
      }
    }
    //record time of latest timer update
    lastTimerUpdate = millis();
  }
  
}

unsigned long calculateTimeRemaining(){
  unsigned long millisRemaining;

  millisRemaining = timerLength- (millis() - timerStartTime);
  return millisRemaining;
}

String formatTime(unsigned long millis){
  char returnValue[8];
  unsigned long workingMillis;
  int hours;
  int minutes;
  int seconds;

  workingMillis = millis;
  hours = workingMillis / (1000*60*60);
  workingMillis = workingMillis - (hours*1000*60*60);
  minutes = workingMillis / (1000*60);
  workingMillis = workingMillis - (minutes*1000*60);
  seconds = workingMillis / 1000;

  sprintf(returnValue,"%02d:%02d:%02d", hours, minutes, seconds);
  return returnValue;
}

//Method to switch the relay on or off as needed
void switchRelay(){
  //set the relay if the minimum time has elapsed
  if (( millis() - lastRelayCommand >= relayCommandDelay) || (lastRelayCommand == 0)) {
    //if  more than the buffer amount above the desired temperature, switch the relay off
    if ((currentTemperature +0.2 < targetTemperature) && timerEnded == false) {
      digitalWrite(relayPin, LOW);
      digitalWrite(ledRelayPin, HIGH);
      blynkLED_relay.on();
    } else {
      digitalWrite(relayPin, HIGH);
      digitalWrite(ledRelayPin, LOW);
      blynkLED_relay.off();
    }
    //record time of latest command to relay
    lastRelayCommand  = millis();
  }
}

//Redraw all screen elements
void redrawScreen(){
  //clear the screen and rewrite the various text elements
  lcd->clear();
  writeScreenTopMessage(messageCurrentTemp);
  writeScreenTopValue(currentTemperature);
  if(setTempMode==false){
    writeScreenBottomMessage(messageTargetTemp);
    writeScreenBottomValue(targetTemperature);
  }else{
    writeScreenBottomMessage(messageSetTemp);
    writeScreenBottomValue(newTargetTemp);
  }

}
//Method to indicate user click of button
bool buttonClicked() {

    buttonReading = analogRead(A0);
    if (buttonReading  > 4000){
      if( buttonClick == false){
        buttonClick = true;
        return(true);
      }
    }else{
      buttonClick = false;
    }
    return(false);
}

// Method to set the desired temperature
int setDesiredTemperature_Cloud(String temp) {
  setDesiredTemperature(temp.toFloat());
  return targetTemperature;
}

void setDesiredTemperature(int temperature){
  targetTemperature = temperature;
  EEPROM.put(10, targetTemperature);
  writeScreenBottomValue(targetTemperature);
  Blynk.virtualWrite(blynkPin_TargetTemp, targetTemperature);
}

int setTimer_Cloud(String timerMinutes){
  setTimer(timerMinutes.toInt()*60*1000);
  return timerMinutes.toInt();
}

void setTimer(unsigned long timerMillis){
  timerStarted = false;
  timerEnded = false;
  timerStartTime = 0;
  timerLength = timerMillis;
  Blynk.virtualWrite(blynkPin_TimerLength, formatTime(timerLength));
}

void loadDesiredTemperature(){
  // Load the desired temp from storage if available, otherwise use the default
  EEPROM.get(10,targetTemperature);
  if (targetTemperature == 0xFFFF) {
    // Stored memory is empty, so set it to default temp
    targetTemperature = defaultTargetTemperature;
    // write the desired temp to stored memory for next time
    EEPROM.put(10, defaultTargetTemperature);
  }
}

//method to initialize the lcd
void initializeDisplay(){
  //initialize the display
  lcd = new LiquidCrystal_I2C(0x27, 16 , 2);
  lcd->init();
  lcd->backlight();
  lcd->clear();
}

// Method to read temperature
float getSensorTemperature(){
    sensors.requestTemperatures();
    float tempC = sensors.getTempCByIndex(0);
    return tempC;
}

// Method to write to top line of lcd
void writeScreenTopMessage(char* message){
  lcd->setCursor(0,0);
  lcd->print(message);
}

// Method to write current temperature to the lcd
void writeScreenTopValue(double value){
  lcd->setCursor(11,0);
  lcd->print(value);
}

//Method to write to bottom line of lcd
void writeScreenBottomMessage(char* message){
  lcd->setCursor(0,1);
  lcd->print(message);
}

//Method to write the desired temperature to the lcd
void writeScreenBottomValue(double  value){
  lcd->setCursor(11,1);
  lcd->print(value);
}

int readEncoderDirection(){
  encoderNewPosition = encoder.read();
  float returnValue = 0.25 * (encoderNewPosition - encoderOldPosition);
  encoderOldPosition = encoderNewPosition;
  return round(returnValue);
}

BLYNK_WRITE(V0)
{
  int pinValue = param.asInt();
  setDesiredTemperature(pinValue);
}

void updateBlynkPins(){
  Blynk.virtualWrite(blynkPin_TargetTemp, targetTemperature);
  Blynk.virtualWrite(blynkPin_CurrentTemp, currentTemperature);
  Blynk.virtualWrite(blynkPin_TimeRemaining, "not started");
}

// recieve mqtt message
void mqttCallback(char *topic, byte *payload, unsigned int length)
{
  publishMessage("Log Event", "mqtt message received", 0);
  publishMessage("Log Event", topic, 0);

  char value[length + 1];
  memcpy(value, payload, length);
  value[length] = NULL;

  if (strcmp(topic, mqttTopic_cmnd_set_target) == 0)
  {
    setDesiredTemperature_Cloud(value);
  }

}