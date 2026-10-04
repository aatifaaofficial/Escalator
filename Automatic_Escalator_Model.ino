#include <Arduino.h>

// Automatic Escalator Model - Arduino UNO
// IR modules are assumed to output LOW when they detect an object.
// Change SENSOR_ACTIVE_STATE to HIGH if your modules work the other way.

const byte ENTRY_SENSOR_PIN = 2;
const byte EXIT_SENSOR_PIN = 3;
const byte EMERGENCY_BUTTON_PIN = 4;
const byte MOTOR_IN1_PIN = 5;
const byte MOTOR_IN2_PIN = 6;
const byte MOTOR_ENA_PIN = 9;
const byte GREEN_LED_PIN = 10;
const byte RED_LED_PIN = 11;
const byte YELLOW_LED_PIN = 12;
const byte BUZZER_PIN = 13;

const byte SENSOR_ACTIVE_STATE = LOW;
const unsigned long SENSOR_DEBOUNCE_MS = 40;
const unsigned long EMERGENCY_RELEASE_MS = 50;
const unsigned long STOP_DELAY = 5000;
const unsigned long STATUS_INTERVAL = 1000;

int MOTOR_SPEED = 180;  // PWM range: 0-255

bool entryDetected = false;
bool exitDetected = false;
bool entryEvent = false;
bool exitEvent = false;
bool lastEntryReading = false;
bool lastExitReading = false;
bool emergencyActive = false;
bool emergencyReleaseTiming = false;
bool motorRunning = false;
bool stopPending = false;

unsigned long entryDebounceTime = 0;
unsigned long exitDebounceTime = 0;
unsigned long emergencyReleaseTime = 0;
unsigned long stopStartTime = 0;
unsigned long lastStatusTime = 0;

bool updateSensorState(byte pin, bool &stableState, bool &lastReading,
                       unsigned long &lastChangeTime, unsigned long now);
void setupPins();
void readSensors();
void checkEmergency();
void handleEscalator();
void startEscalator();
void stopEscalator();
void emergencyStop();
void updateLEDs();
void updateBuzzer();
void printStatus();

void setup() {
  Serial.begin(9600);
  setupPins();
  stopEscalator();
  updateLEDs();
  updateBuzzer();
  Serial.println("Automatic Escalator Model ready.");
}

void loop() {
  checkEmergency();
  readSensors();
  handleEscalator();
  updateLEDs();
  updateBuzzer();

  const unsigned long now = millis();
  if (now - lastStatusTime >= STATUS_INTERVAL) {
    lastStatusTime = now;
    printStatus();
  }
}

void setupPins() {
  pinMode(ENTRY_SENSOR_PIN, INPUT_PULLUP);
  pinMode(EXIT_SENSOR_PIN, INPUT_PULLUP);
  pinMode(EMERGENCY_BUTTON_PIN, INPUT_PULLUP);

  pinMode(MOTOR_IN1_PIN, OUTPUT);
  pinMode(MOTOR_IN2_PIN, OUTPUT);
  pinMode(MOTOR_ENA_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(MOTOR_IN1_PIN, LOW);
  digitalWrite(MOTOR_IN2_PIN, LOW);
  analogWrite(MOTOR_ENA_PIN, 0);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
}

void readSensors() {
  const unsigned long now = millis();
  const bool entryChanged = updateSensorState(
      ENTRY_SENSOR_PIN, entryDetected, lastEntryReading, entryDebounceTime, now);
  const bool exitChanged = updateSensorState(
      EXIT_SENSOR_PIN, exitDetected, lastExitReading, exitDebounceTime, now);

  entryEvent = entryChanged && entryDetected;
  exitEvent = exitChanged && exitDetected;

  if (entryEvent) {
    Serial.println("Person detected at entrance");
  }
  if (exitEvent) {
    Serial.println("Person detected at exit");
  }
}

bool updateSensorState(byte pin, bool &stableState, bool &lastReading,
                       unsigned long &lastChangeTime, unsigned long now) {
  const bool reading = digitalRead(pin) == SENSOR_ACTIVE_STATE;
  if (reading != lastReading) {
    lastReading = reading;
    lastChangeTime = now;
  }

  if (now - lastChangeTime >= SENSOR_DEBOUNCE_MS && stableState != reading) {
    stableState = reading;
    return true;
  }
  return false;
}

void checkEmergency() {
  if (digitalRead(EMERGENCY_BUTTON_PIN) == LOW) {
    emergencyReleaseTiming = false;
    if (!emergencyActive) {
      emergencyActive = true;
      stopPending = false;
      emergencyStop();
      Serial.println("!!! EMERGENCY STOP !!!");
      Serial.println("Motor: OFF");
      Serial.println("Buzzer: ON");
    }
    return;
  }

  if (emergencyActive) {
    const unsigned long now = millis();
    if (!emergencyReleaseTiming) {
      emergencyReleaseTiming = true;
      emergencyReleaseTime = now;
    } else if (now - emergencyReleaseTime >= EMERGENCY_RELEASE_MS) {
      emergencyActive = false;
      emergencyReleaseTiming = false;
      Serial.println("Emergency released. System: STANDBY");
    }
  }
}

void handleEscalator() {
  if (emergencyActive) {
    return;
  }

  if (entryEvent) {
    stopPending = false;
    startEscalator();
  }

  if (exitEvent && motorRunning) {
    stopPending = true;
    stopStartTime = millis();
    Serial.println("Stopping escalator in 5 seconds...");
  }

  if (stopPending && millis() - stopStartTime >= STOP_DELAY) {
    Serial.println("Stopping escalator...");
    stopEscalator();
    stopPending = false;
    Serial.println("Motor: OFF");
    Serial.println("System: STANDBY");
  }
}

void startEscalator() {
  if (emergencyActive || motorRunning) {
    return;
  }

  stopPending = false;
  digitalWrite(MOTOR_IN1_PIN, HIGH);
  digitalWrite(MOTOR_IN2_PIN, LOW);
  analogWrite(MOTOR_ENA_PIN, constrain(MOTOR_SPEED, 0, 255));
  motorRunning = true;
  Serial.println("Escalator STARTED");
  Serial.println("Motor: ON");
}

void stopEscalator() {
  analogWrite(MOTOR_ENA_PIN, 0);
  digitalWrite(MOTOR_IN1_PIN, LOW);
  digitalWrite(MOTOR_IN2_PIN, LOW);
  motorRunning = false;
}

void emergencyStop() {
  stopPending = false;
  stopEscalator();
}

void updateLEDs() {
  digitalWrite(RED_LED_PIN, emergencyActive ? HIGH : LOW);
  digitalWrite(GREEN_LED_PIN, emergencyActive ? LOW : HIGH);
  digitalWrite(YELLOW_LED_PIN,
               (!emergencyActive && (entryDetected || exitDetected || motorRunning || stopPending))
                   ? HIGH
                   : LOW);
}

void updateBuzzer() {
  digitalWrite(BUZZER_PIN, emergencyActive ? HIGH : LOW);
}

void printStatus() {
  Serial.println("================================");
  Serial.println("   AUTOMATIC ESCALATOR MODEL");
  Serial.println("================================");
  Serial.print("Entry Sensor : ");
  Serial.println(entryDetected ? "DETECTED" : "CLEAR");
  Serial.print("Exit Sensor  : ");
  Serial.println(exitDetected ? "DETECTED" : "CLEAR");
  Serial.print("Emergency    : ");
  Serial.println(emergencyActive ? "YES" : "NO");
  Serial.print("Motor        : ");
  Serial.println(motorRunning ? "ON" : "OFF");
  Serial.print("Speed        : ");
  Serial.println(MOTOR_SPEED);
  Serial.print("System       : ");
  if (emergencyActive) {
    Serial.println("EMERGENCY STOP");
  } else if (motorRunning && stopPending) {
    Serial.println("WAITING TO STOP");
  } else if (motorRunning) {
    Serial.println("RUNNING");
  } else {
    Serial.println("STANDBY");
  }
  Serial.println("================================");
}
