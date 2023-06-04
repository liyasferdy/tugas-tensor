#include <Arduino.h>
#include <DHT.h>
#include <Servo.h>
#include "MQ135.h"


#include <WiFi.h>
#include <WiFiClient.h>
#define DHT_SENSOR_PIN  21 // ESP32 pin GIOP21 connected to DHT22 sensor
#define DHT_SENSOR_TYPE DHT22
#define servoPin 26
#define PIN_MQ135 39
#define timeSeconds 10 //for PIR
#define trigPin 19
#define echoPin 18

#define BLYNK_TEMPLATE_ID "TMPL6gVRngNro"
#define BLYNK_TEMPLATE_NAME "myTensor"
#define BLYNK_AUTH_TOKEN "_cvaut9V3KQzTzBIFr-92Zumv6jtbZaq"


Servo myservo;
DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
int airQuality ;


int relayPin1 = 13 ; // Relay for AC, 13
int relayPin2 = 12;  // Relay for Lampu, 12
int relayPin3 = 14;  // Relay for Keran with Servo, 14
int relayPin4 = 27;  // Relay for Air Humadifier, 27
const int motionSensor = 33;  //PIN PIR Sensor to 33
const int led = 32;  //PIN LED for PIR to 32
MQ135 mq135_sensor = MQ135(PIN_MQ135);  //Pin MQ135 to 39

//Set point
float desiredTemp = 29.0; // suhu yang diinginkan dalam derajat Celsius
float desiredAirQuality = 100.0; // kualitas udara yang diinginkan (ppm)


// Timer: Auxiliary variables
unsigned long now = millis();
unsigned long lastTrigger = 0;
boolean startTimer = false;
boolean motion = false;

// Checks if motion was detected, sets LED HIGH and starts a timer
void IRAM_ATTR detectsMovement() {
  digitalWrite(led, HIGH);
  startTimer = true;
  lastTrigger = millis();
}


void setup() {

  // put your setup code here, to run once:
  Serial.begin(9600);
  dht_sensor.begin(); // initialize the DHT sensor
  myservo.attach(servoPin); 
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(relayPin1, OUTPUT); // atur pin relay sebagai output
  pinMode(relayPin2, OUTPUT); // atur pin relay sebagai output
  pinMode(relayPin3, OUTPUT); // atur pin relay sebagai output
  pinMode(relayPin4, OUTPUT); // atur pin relay sebagai output

  // Ultrasonic level
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  // PIR Motion Sensor mode INPUT_PULLUP
  pinMode(motionSensor, INPUT_PULLUP);
  // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);

  // Set LED to LOW
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  
  
}

void loop() {

  //Kontrol Relay w/ Voice
  
  // put your main code here, to run repeatedly:
  char a = Serial.read();

  //PIR Sensor to Relay and lamp control
 // Current time
  now = millis();
  if(((digitalRead(led) == HIGH) && (motion == false)) || (a == '3')) {
    Serial.println("MOTION DETECTED!!!");
    digitalWrite(relayPin2, HIGH);  // While detect a motion, Turn on lamp
    motion = true;
  // Turn off the LED after the number of seconds defined in the timeSeconds variable
  } else if((startTimer && (now - lastTrigger > (timeSeconds*1000))) || (a == '4')){
    Serial.println("Motion stopped...");
    digitalWrite(led, LOW);
    digitalWrite(relayPin2, LOW);  // While not detect a motion, Turn off lamp
    startTimer = false;
    motion = false;
  }
  

// Humadity and Temperature serial read
 // read humidity
  float humi  = dht_sensor.readHumidity();
  // read temperature in Celsius
  float tempC = dht_sensor.readTemperature();
  // read temperature in Fahrenheit
  float tempF = dht_sensor.readTemperature(true);


//Get parameter from MQ135 library
  float rzero = mq135_sensor.getRZero();
  float correctedRZero = mq135_sensor.getCorrectedRZero(tempC, humi);
  float resistance = mq135_sensor.getResistance();
  float ppm = mq135_sensor.getPPM();
  float correctedPPM = mq135_sensor.getCorrectedPPM(tempC, humi);

// Give condition to Control Air Quality in the room with Air Humadifier
// corected change ppm param
 if ((tempC >= desiredTemp && ppm >= desiredAirQuality) ){ // jika suhu dan kualitas udara mencapai batas yang diinginkan
    digitalWrite(relayPin4, HIGH); // hidupkan Air humadifier
  } else {
    digitalWrite(relayPin4, LOW); // matikan Air humadifier
  }

// Give condition to Control temperature in the room with AC
 if ((tempC >= desiredTemp) || (a == '1')){ // jika suhu dan kualitas udara mencapai batas yang diinginkan
    digitalWrite(relayPin1, HIGH); // hidupkan AC otomatis
  }else if((tempC <= desiredTemp) || (a == '2')){
    //digitalWrite(LED_BUILTIN, LOW); 
    digitalWrite(relayPin1, LOW); // matikan AC otomatis   
  } 


// Controlling water level with HC-SR04
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  //give condition
  if ((distance < 10) || (a == '5')) {   // batas 10cm
    myservo.write(90); // posisi servo untuk menutup keran
  } else if((distance > 10) || (a == '6')){ 
    myservo.write(0); // posisi servo untuk menutup keran;   // matikan keran     
  }


//Serial print
  // check whether the reading is successful or not
  if ( isnan(tempC) || isnan(tempF) || isnan(humi)) {
    Serial.println("Failed to read from DHT sensor!");
  } else if(isnan(rzero) || isnan(correctedPPM) || isnan(resistance) || isnan(ppm) || isnan(correctedPPM) ){
    Serial.println("Failed to read from MQ135 sensor!");
  }else {
    Serial.print("Humidity: ");
    Serial.print(humi);
    Serial.print("%");

    Serial.print("  |  ");

    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print(" C  ~  ");
    Serial.print(tempF);
    Serial.println(" F");

    Serial.print("  |  ");

    Serial.print("\t Air Quality: ");
    Serial.print(ppm);
    Serial.print("\t Corrected PPM: ");
    Serial.print(correctedPPM);
    Serial.println("ppm");

  }

  // wait a 2 seconds between readings
  delay(2000);

}