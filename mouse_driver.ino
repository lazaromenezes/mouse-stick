#include "math.h"
#include "Mouse.h"

#define X_AXIS A0
#define Y_AXIS A1

#define MOUSE_GRAB_LIGHT 5
#define MOUSE_GRAB_TOGGLE 4

const int MAX_VALUE = 1023;
const int STEP_COUNT = 7;
const int MOUSE_RESPONSE_DELAY = 2;

int midStep = 0;

bool mouseControlEnabled = false;
bool previousToggleState = false;

void setup() {
  pinMode(X_AXIS, INPUT);
  pinMode(Y_AXIS, INPUT);
  pinMode(MOUSE_GRAB_TOGGLE, INPUT_PULLUP);

  pinMode(MOUSE_GRAB_LIGHT, OUTPUT);

  midStep = ceil(STEP_COUNT / 2.0);

  Mouse.begin();
}

void loop() {
  Serial.begin(115200);

  bool control_toggle = digitalRead(MOUSE_GRAB_TOGGLE);

  if(control_toggle != previousToggleState){
    if(control_toggle == LOW){
      mouseControlEnabled = !mouseControlEnabled;
      digitalWrite(MOUSE_GRAB_LIGHT, mouseControlEnabled ? HIGH : LOW);
    }
  }

  previousToggleState = control_toggle;

  if(mouseControlEnabled){
    int x_read = analogRead(X_AXIS);
    int y_read = analogRead(Y_AXIS);
    int x_step = stepFromValue(x_read);
    int y_step = stepFromValue(y_read);
    _serial_print_readings(x_read, y_read, x_step, y_step);
    Mouse.move(x_step, y_step);
  }

  delay(MOUSE_RESPONSE_DELAY);
}

int stepFromValue(int value) {
    if(value <= 0)
        return STEP_COUNT / -2;

    if(value >= MAX_VALUE)
        return STEP_COUNT / 2;

    double proportion = value / (double) MAX_VALUE;

    int step = ceil(STEP_COUNT * proportion);

    return step - midStep;
}

void _serial_print_readings(int xr, int yr, int xs, int ys){
  Serial.print("X read: ");
  Serial.print(xr);
  Serial.print(" Y read: ");
  Serial.print(yr);
  Serial.print(" X step: ");
  Serial.print(xs);
  Serial.print(" Y step: ");
  Serial.print(ys);
  Serial.println("");
}