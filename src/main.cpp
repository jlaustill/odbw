#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac;

int minAppsOffIdle = 1000;
int maxApps = 0;
int appsValue = 0;
int isIdling = 0;
int appsThrottlePercentage = 0;
int appsPin = A0;
int idlePin = 4;
int relayPin = 5;

int dacp3 = 0;
int dac3p8 = 0;
int dacOutput = 0;

void setup() {
  pinMode(idlePin, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);
  dac.begin(0x60);

  dacp3 = map(.3, 0, 5, 0, 4095);
  dac3p8 = map(3.8, 0, 5, 0, 4095);
}

void loop() {
  appsValue = analogRead(appsPin);
  isIdling = digitalRead(idlePin);

  if (appsValue < minAppsOffIdle && !isIdling) {
    minAppsOffIdle = appsValue;
  }

  if (appsValue > maxApps) {
    maxApps = appsValue;
  }

  appsThrottlePercentage = map(appsValue, 157, 815, 0, 100);

  appsThrottlePercentage =
      appsThrottlePercentage < 0 ? 0 : appsThrottlePercentage;
  appsThrottlePercentage =
      appsThrottlePercentage > 100 ? 100 : appsThrottlePercentage;

  if (isIdling) {
    appsThrottlePercentage = 0;
    digitalWrite(relayPin, LOW);
    dac.setVoltage(dacp3, false);
  } else {
    digitalWrite(relayPin, HIGH);
    dacOutput = map(appsThrottlePercentage, 0, 100, dacp3, dac3p8);
    dac.setVoltage(dacOutput, false);
  }
  
  Serial.println("APPS MIN: " + (String)minAppsOffIdle +
                 " MAX: " + (String)maxApps + " Throttle Percentage: " +
                 (String)appsThrottlePercentage + " Idle: " + (String)isIdling);
}