//Code by Leo Marek 2020 www.gocapit.tech -- Inspiration from roboticadiy.com

//US Library
#include <NewPing.h>

//LCD Library
#include <LiquidCrystal.h>

//Pins for First US Sensor
const int trig1Pin = 10;
const int echo1Pin = 11;

//Pins for Second US Sensor
const int trig2Pin = 12;
const int echo2Pin = 13;

//LED Output Pins
const int gPin = 9;
const int rPin = 8;

//Room Capacity
const int capacity = 4;

//Distance Sensitivity Adjustment
const int sensitivity = 100;

//Input pins for LCD
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

#define max_cm_distance 1000

int currentPeople = 0;

class USSensor {
//US Sensor setup
public:
  USSensor(int triggerPin, int echoPin, String aName) {
    trig = triggerPin;
    echo = echoPin;
    sonar = new NewPing(triggerPin, echoPin, max_cm_distance);
    isActivated = false;
    name = aName;
    time = 0;
  }

  
  void update() {
    updateSensor();
    if (value < sensitivity) {
      setActive(true);
    } else {
      if (millis() - time > 500) {
        setActive(false);
      } 
    }
  }


  
  void reset() {
    setActive(false);
    time = 0;
  }
  
  void setActive(boolean state) {
    if (!isActivated && state) {
      time = millis();
    }
    if (isActivated != state) {
      isActivated = state;
      Serial.print(name + ": ");
      Serial.println(state);
    }
  }


  
  boolean wasActive() {
    return (time != 0) && !isActivated;
  }


  
  int lastActive() {
    return millis() - time;
  }


  
private:
  int trig;
  int echo;
  int led;
  String name;
  boolean isActivated;
  unsigned long time;
  float value;
  NewPing *sonar;
  
  void updateSensor() {
    
    delay(25);
    unsigned int uS = sonar->ping();
    value = uS / US_ROUNDTRIP_CM;

  }
};

\
USSensor *A;
USSensor *B;

void setup() {
  Serial.begin(9600);
  
  A = new USSensor(trig1Pin, echo1Pin, "A");
  B = new USSensor(trig2Pin, echo2Pin, "B");

  pinMode(gPin, OUTPUT);
  pinMode (rPin, OUTPUT);

  lcd.begin(16,2);
  lcd.print("Cap-It!");
  delay(1000);

}



void loop() {
  
    A->update();
    B->update();
    
    if (A->wasActive() && B->wasActive()) {
      int a_time = A->lastActive();
      int b_time = B->lastActive();
      if (a_time < 5000 && b_time < 5000) {
        if (a_time > b_time) {
          currentPeople++;
        } else {
          currentPeople--;
        }
      }
      A->reset();
      B->reset();
    }

    if (currentPeople < 0) {
      currentPeople = 0;
    }
    
    if (currentPeople >= capacity) {
      digitalWrite(gPin, HIGH);
      digitalWrite(rPin, LOW);
      
    } else {
      digitalWrite(gPin, LOW);
      digitalWrite(rPin, HIGH);
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Capacity:");
    lcd.setCursor(0,1);
    lcd.print(currentPeople);
    lcd.print("/");
    lcd.print(capacity);
    
}
