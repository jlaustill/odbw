#include <Arduino.h>

int minAppsOffIdle = 1000;
int maxApps = 0;
int appsValue = 0;
int idleValue = 0;
int appsThrottlePercentage = 0;

void setup() {
  pinMode(4, INPUT_PULLUP);
}

void loop() {
  appsValue = analogRead(A0);
  idleValue = digitalRead(4);

  if (appsValue < minAppsOffIdle && !idleValue) {
    minAppsOffIdle = appsValue;
  }

  if (appsValue > maxApps) {
    maxApps = appsValue;
  }

  appsThrottlePercentage = map(appsValue, 157, 815, 0, 100);

  appsThrottlePercentage = appsThrottlePercentage < 0 ? 0 : appsThrottlePercentage;
  appsThrottlePercentage = appsThrottlePercentage > 100 ? 100 : appsThrottlePercentage;

  if (idleValue) {
    appsThrottlePercentage = 0;
  }
  Serial.println("APPS MIN: " + (String)minAppsOffIdle + " MAX: " + (String)maxApps + " Throttle Percentage: " + (String)appsThrottlePercentage + " Idle: " + (String)idleValue);
}