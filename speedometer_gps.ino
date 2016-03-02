/*
GPS Speedometer Firmware
Taylor Daniska
*/

#include <SoftwareSerial.h>
#include <Stepper.h>
#include <TinyGPS.h>

TinyGPS gps;
SoftwareSerial ss(10, 11);

static void smartdelay(unsigned long ms);
static void print_float(float val, float invalid, int len, int prec);
static void print_int(unsigned long val, unsigned long invalid, int len);
static void print_date(TinyGPS &gps);
static void print_str(const char *str, int len);
const int stepsPerRevolution = 200; 
const int buttonPin = 2;

Stepper myStepper(stepsPerRevolution, 5,6,8,7);            

int currentStep = 0;
int speedValue = 0;
int buttonState = 0; 
boolean startup = true;
int incomingByte;
int stepValue = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(buttonPin, INPUT); 
  ss.begin(9600);
}

void loop()
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
    gps.encode(ss.read());
  } while (millis() - start < 10);          //change 10 to adjust delay for gps
  if (startup == true) {
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {
      for (int i=0; i <= 30; i++) {
      myStepper.step(-1);
      delay(10);
      1}
    }
  Serial.print("Startup");
  for (int i=0; i <= 68; i++) { 
    Serial.print("for");
    myStepper.step(1);
    delay(10);
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {     
      startup = false;
      for (int i=0; i <= 44; i++) {
      Serial.print("return");
      myStepper.step(-1);
      delay(10);
      }
      break;  
    } 
  }
  if(startup == true) {
    for (int i=0; i <= 68; i++) {
      Serial.print("for");
      myStepper.step(-1);
      delay(10);
      buttonState = digitalRead(buttonPin);
      if (buttonState == HIGH) {     
        startup = false;
        for (int i=0; i <= 53; i++) {
          Serial.print("return");
          myStepper.step(-1);
          delay(10);
        }
        break;  
      } 
    }
  }
  startup = false;
  }
  Serial.print("Run");
  speedValue = (gps.f_speed_kmph()*0.621371);
  delay(10);
  stepValue = map(speedValue, 0, 80, 0, 63);
  if (stepValue > currentStep) {
    myStepper.step(1);
    currentStep = currentStep + 1;
  }
  else if (stepValue < currentStep) {
    myStepper.step(-1);
    currentStep = currentStep - 1;
  }
}


