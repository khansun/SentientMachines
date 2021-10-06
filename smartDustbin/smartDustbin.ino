#include <Servo.h>
const int echoPin = 6; // Echo Pin of Ultrasonic Sensor
const int pingPin = 7; // Trigger Pin of Ultrasonic Sensor
const int echoPin2 = 9; // Echo Pin of Ultrasonic Sensor
const int pingPin2 = 8; // Trigger Pin of Ultrasonic Sensor
const int AO_Pin=0; // Connect AO of MQ4 with Analog channel 0 pin (A0) of Arduino
int AO_Out; // stores analog output of MQ4 sensor


Servo motor;
int pointer = 0;
void setup() {
  motor.attach(2, 2000, 2400);
  ServoClose();
  Serial.begin(9600);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT); 
}
void ServoClose()
{
  for (pointer = 420; pointer >= 0; pointer -= 10) { 
    motor.write(pointer);
  }
}

void ServoOpen()
{
  for (pointer = 0; pointer <= 420; pointer += 10) {
    motor.write(pointer);  
  }
}

long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}


void lidAction(){
   long duration, inches;
   pinMode(pingPin, OUTPUT);
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   pinMode(echoPin, INPUT);
   duration = pulseIn(echoPin, HIGH);
   inches = microsecondsToInches(duration);
   Serial.print("Distance: ");
   Serial.print(inches);
   Serial.print(" in");
   Serial.println();
   if(inches < 8){
    ServoOpen();
  digitalWrite(11, HIGH);   
  delay(2500); 
   }
   else{
  digitalWrite(11, LOW);   
  ServoClose();
    }   
  }


void capacityAlert(){
   long duration, inches;
   pinMode(pingPin2, OUTPUT);
   digitalWrite(pingPin2, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin2, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin2, LOW);
   pinMode(echoPin2, INPUT);
   duration = pulseIn(echoPin2, HIGH);
   inches = microsecondsToInches(duration);
   Serial.print("Remaining Distance: ");
   Serial.print(inches);
   Serial.print(" in");
   Serial.println();
   if(inches < 3){
  ServoOpen();
  digitalWrite(11, HIGH);
  digitalWrite(12, LOW);
  delay(4000);     
   }
   else{
  digitalWrite(11, LOW); 
  digitalWrite(12, HIGH); 
  ServoClose();
    }
      
  }  

void gasAlert(){
  AO_Out= analogRead(AO_Pin);
  Serial.print("Methane Conentration: ");
  Serial.println(AO_Out);
  if (AO_Out >  300){
  digitalWrite(13, HIGH);
  digitalWrite(12, LOW);
  delay(200);
 }
  else{
  digitalWrite(13, LOW);
  digitalWrite(12, HIGH);
} 
}

void loop() {
  digitalWrite(12, HIGH);
  capacityAlert();
  lidAction();
  gasAlert();
   
}
