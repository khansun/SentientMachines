#include <Servo.h>

const int echoPin = 5; // Echo Pin of Ultrasonic Sensor
const int pingPin = 6; // Trigger Pin of Ultrasonic Sensor
const int echoPin2 = 9; // Echo Pin of Ultrasonic Sensor
const int pingPin2 = 10; // Trigger Pin of Ultrasonic Sensor
const int gasPin=0; // MQ4 with Analog channel 0 pin (A0) of Arduino
const int servoPin = 3; // PWM pin for SERVO motor
const int greenLED = 2; // Normal indicator
const int orangeLED = 4; // Lid action indicator
const int redLED = 7; // Gas Hazard indicator



Servo lidServo; // Servo motor
int pos = 0; // initial Servo rotation
int dialLimit = 1; // Send SMS only once
int sentCount = 0; // Counter variable to send SMS only once
char phoneNumber[] = "01784190980";  //phone number to alert
int gasSaturation = 0; // Analogue output of the MQ4 sensor
int trashDistance = 50;
int remainingCapacity = 50;

void setup() 
{
  lidServo.attach(servoPin);
  closeLid();
  Serial.begin(115200);
  delay(1000);
  Serial.println("AT+CMGF=1"); //set GSM to text mode
  delay(200);
  pinMode(greenLED, OUTPUT);
  pinMode(orangeLED, OUTPUT);
  pinMode(redLED, OUTPUT); 
}

void openLid()
{
  for(pos = 0; pos<= 170; pos+=8)
  {
    lidServo.write(pos);
    delay(30); 
  }
}

void closeLid()
{
  for(pos; pos >= 0; pos-=8)
  {
    lidServo.write(pos);
    delay(20);
  }

}

long microsecondsToInches(long microseconds) 
{
  return microseconds / 74 / 2;
}


void collectTrash()
{
  long duration;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  trashDistance = microsecondsToInches(duration);
  
  if(trashDistance < 7)
  {
    digitalWrite(orangeLED, HIGH);
    openLid();
    delay(3000); 
  }
  else
  {
    digitalWrite(orangeLED, LOW);   
    closeLid();
  }   
}


void capacityAlert()
{
  long duration;
  pinMode(pingPin2, OUTPUT);
  digitalWrite(pingPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin2, LOW);
  pinMode(echoPin2, INPUT);
  duration = pulseIn(echoPin2, HIGH);
  remainingCapacity = microsecondsToInches(duration);
  
  if(remainingCapacity <= 3)
  {
    openLid();
    digitalWrite(orangeLED, HIGH);
    digitalWrite(greenLED, LOW);
    delay(3000);     
  }
  else
  {
    digitalWrite(greenLED, HIGH);    
    digitalWrite(orangeLED, LOW);  
    closeLid();
  }
}  


void gasAlert()
{
  gasSaturation = analogRead(gasPin);
  if (gasSaturation > 250)
  {
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, HIGH);
    while(sentCount<dialLimit)
    {
      delay(1500);
      Serial.print("ATD");; //AT Dial phone number
      Serial.print(phoneNumber);
      Serial.println(";"); // Append Semicolon to execute command
      delay(15000);
      
      Serial.println("ATH"); // Hang up in a few seconds
      delay(1000);
      
      Serial.print("AT+CMGS=\"");
      Serial.print(phoneNumber);
      Serial.println("\"");

      delay(1000);  // Wait for '>'

      Serial.print("DANGER!!! Hazardous GAS detected near your SmartDustbin!");  // Alert SMS
      Serial.write(0x1A);  // HEX code for ctrl+z send the SMS
      delay(5000);
      
      sentCount++;
    }
  }
  else
  {
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);
  }  
}

void loop() 
{ 
  digitalWrite(greenLED, HIGH);
  collectTrash();
  capacityAlert();
  gasAlert();
}
