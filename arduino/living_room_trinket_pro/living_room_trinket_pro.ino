/*
   This script is for the trinket in the living room.
*/
#include <Bounce2.h>

#define DOOR_BUTTON 1
#define SIDE_BUTTON 2
#define LAMPS_BUTTON 3
#define ALL_BUTTON 4
#define DOOR_PWM 5
#define SIDE_PWM 6
#define LAMPS_PWM 9
#define DEBOUNCE_INTERVAL 50

const String ON = "ON";
const String OFF = "OFF";
const String DOOR = "DOOR";
const String SIDE = "SIDE";
const String LAMPS = "LAMPS";
const String ALL = "ALL";
const char delimiter = ' ';

Bounce doorBtn = Bounce();
Bounce sideBtn = Bounce();
Bounce lampsBtn = Bounce();
Bounce allBtn = Bounce();

bool updateBtns() {
  return doorBtn.update() ||
         sideBtn.update() ||
         lampsBtn.update() ||
         allBtn.update();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(DOOR_BUTTON, INPUT_PULLUP);
  pinMode(SIDE_BUTTON, INPUT_PULLUP);
  pinMode(LAMPS_BUTTON, INPUT_PULLUP);
  pinMode(ALL_BUTTON, INPUT_PULLUP);
  pinMode(DOOR_PWM, OUTPUT);
  pinMode(SIDE_PWM, OUTPUT);
  pinMode(LAMPS_PWM, OUTPUT);

  doorBtn.attach(DOOR_BUTTON);
  sideBtn.attach(SIDE_BUTTON);
  lampsBtn.attach(LAMPS_BUTTON);
  allBtn.attach(ALL_BUTTON);

  doorBtn.interval(DEBOUNCE_INTERVAL);
  sideBtn.interval(DEBOUNCE_INTERVAL);
  lampsBtn.interval(DEBOUNCE_INTERVAL);
  allBtn.interval(DEBOUNCE_INTERVAL);
}

void loop() {
  if (Serial.available()) {
    
  }
  else if (updateBtns()) {
    if (doorBtn.fell()) {
      
    }
    else if (sideBtn.fell()) {
      
    }
    else if (lampsBtn.fell()) {
      
    }
    else if (all.fell()) {
      
    }
  }
}
