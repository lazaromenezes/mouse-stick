#include "math.h"

#define UP 23
#define LEFT 22
#define DOWN 21
#define RIGHT 19

#define X_AXIS 39
#define Y_AXIS 34

const int MAX_VALUE = 4095;
const int STEP_COUNT = 9;

const int LED_COUNT = 4;
const int pins[] = {RIGHT, DOWN, LEFT, UP};
const int STEP_AMOUNT = 63; // 255 / 3

int midStep = 0;

void setup() {
  for(int i = 0; i < LED_COUNT; i++)
    pinMode(pins[i], OUTPUT);

  pinMode(X_AXIS, INPUT);
  pinMode(Y_AXIS, INPUT);

  midStep = ceil(STEP_COUNT / 2.0);
}

void loop() {
  Serial.begin(115200);

  int x_read = analogRead(X_AXIS);
  int y_read = analogRead(Y_AXIS);
  int x_step = stepFromValue(x_read);
  int y_step = stepFromValue(y_read);

  Serial.printf("X read: %d step: %d | Y read: %d step: %d\n", x_read, x_step, y_read, y_step);

  if(x_step < 0)
    analogWrite(LEFT, x_step * -1 * STEP_AMOUNT);
  else if(x_step > 0)
    analogWrite(RIGHT, x_step * STEP_AMOUNT);
  else {
    analogWrite(LEFT, LOW);
    analogWrite(RIGHT, LOW);
  }

  if(y_step < 0)
    analogWrite(DOWN, y_step * -1 * STEP_AMOUNT);
  else if(y_step > 0)
    analogWrite(UP, y_step * STEP_AMOUNT);
  else {
    analogWrite(UP, LOW);
    analogWrite(DOWN, LOW);
  }
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
