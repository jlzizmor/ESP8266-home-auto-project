/*
   This script is for the trinket in the living room.
*/
#include <Bounce2.h>
#include <EEPROM.h>

#define DOOR_BUTTON 1
#define SIDE_BUTTON 2
#define LAMPS_BUTTON 3
#define ALL_BUTTON 4
#define DOOR_PWM 5
#define SIDE_PWM 6
#define LAMPS_PWM 9
#define DOOR_EEPROM 0
#define SIDE_EEPROM 1
#define LAMPS_EEPROM 2
#define DEBOUNCE_INTERVAL 50
#define SWITCH_DELAY 500

#define DOOR_ON 10
#define DOOR_OFF 0
#define DOOR_NEUTRAL 5

#define SIDE_ON 10
#define SIDE_OFF 0
#define SIDE_NEUTRAL 5

#define LAMPS_ON 10
#define LAMPS_OFF 0
#define LAMPS_NEUTRAL 5

#define ON 1
#define OFF -1
#define NEUTRAL 0

#define DOOR_ID 10
#define SIDE_ID 11
#define LAMPS_ID 12
#define ALL_ID 13

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

String serialInput;
int input[2];

bool updateBtns() {
  return doorBtn.update() ||
         sideBtn.update() ||
         lampsBtn.update() ||
         allBtn.update();
}

void flipDoor() {
  if (EEPROM.read(DOOR_EEPROM) == ON) {
    turnDoor(OFF);
  }
  else {
    turnDoor(ON);
  }
}

void flipSide() {
  if (EEPROM.read(SIDE_EEPROM) == ON) {
    turnSide(OFF);
  }
  else {
    turnSide(ON);
  }
}

void flipLamps() {
  if (EEPROM.read(LAMPS_EEPROM) == ON) {
		turnLamps(OFF);
  }
  else {
		turnLamps(ON);
  }
}

void flipAll() {
  int onCount = 0;
  int offCount = 0;

  if (EEPROM.read(DOOR_EEPROM) == 1) {
    onCount++;
  }
  else {
    offCount++;
  }
  if (EEPROM.read(SIDE_EEPROM) == 1) {
    onCount++;
  }
  else {
    offCount++;
  }
  if (EEPROM.read(LAMPS_EEPROM) == 1) {
    onCount++;
  }
  else {
    offCount++;
  }

  if (onCount > offCount) {
    turnAll(on);
  }
  else {
    turnAll(off);
  }
}

void turnAll(char state) {
	turnDoor(state);
	turnSide(state);
	turnLamps(state);
}

void turnDoor(char state) {
	if (state == ON) {
		analogWrite(DOOR_PWM, DOOR_ON);
	}
	else if (state == OFF) {
		analogWrite(DOOR_PWM, DOOR_OFF);
	}
	if ((state == ON) || (state == OFF)) {
		EEPROM.write(DOOR_EEPROM, state);
		EEPROM.commit();
		delay(SWITCH_DELAY);
	}

	analogWrite(DOOR_PWM, DOOR_NEUTRAL);
	delay(SWITCH_DELAY);
}

void turnSide(char state) {
	if (state == ON) {
		analogWrite(SIDE_PWM, SIDE_ON);
	}
	else if (state == OFF) {
		analogWrite(SIDE_PWM, SIDE_OFF);
	}
	if ((state == ON) || (state == OFF)) {
		EEPROM.write(SIDE_EEPROM, state);
		EEPROM.commit();
		delay(SWITCH_DELAY);
	}

	analogWrite(SIDE_PWM, SIDE_NEUTRAL);
	delay(SWITCH_DELAY);
}

void turnLamps(char state) {
	if (state == ON) {
		analogWrite(LAMPS_PWM, LAMPS_ON);
	}
	else if (state == OFF) {
		analogWrite(LAMPS_PWM, LAMPS_OFF);
	}
	if ((state == ON) || (state == OFF)) {
		EEPROM.write(LAMPS_EEPROM, state);
		EEPROM.commit();
		delay(SWITCH_DELAY);
	}

	analogWrite(LAMPS_PWM, LAMPS_NEUTRAL);
	delay(SWITCH_DELAY);
}

void setup() {
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

	serialInput = "";
}

void analyse(int* array, String input) {
	String character = "";

	character = input.substring(0,1);

	if (character.equals(DOOR.substring(0,1)))
		array[0] = DOOR_ID;
	else if (character.equals(SIDE.substring(0,1)))
		array[0] = SIDE_ID;
	else if (character.equals(LAMPS.substring(0,1)))
		array[0] = LAMPS_ID;
	else
		array[0] = ALL_ID;

	character = input.substring(input.length()-1);

	if (character.equals(ON.substring(ON.length()-1)))
		array[1] = ON;
	else
		array[1] = OFF;
}

void loop() {
  if (Serial.available()) {
    serialInput = Serial.readString();

		analyse(input, serialInput);

		if (input[0] == DOOR_ID)
			turnDoor(input[1]);
		else if (input[0] == SIDE_ID)
			turnSide(input[1]);
		else if (input[0] == LAMPS_ID)
			turnLamps(input[1]);
		else
			turnAll(input[1]);
  }
  else if (updateBtns()) {
    if (doorBtn.fell()) {
      flipDoor();
    }
    else if (sideBtn.fell()) {
      flipSide();
    }
    else if (lampsBtn.fell()) {
      flipLamps();
    }
    else if (all.fell()) {
      flipAll();
    }
  }
}
