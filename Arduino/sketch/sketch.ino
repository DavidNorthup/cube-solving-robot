#include <MyStepper.h>

#define STEPS_PER_QUARTER_TURN 50
#define STEPS_PER_SEPARATION 22

int identifier = 1; // 1 for LR, 2 for FB

int motor1_pins[] = {13,11,12,10};
int motor2_pins[] = {9,7,8,6};
int motor3_pins[] = {5,3,4,2};

StepperMotor motor1(motor1_pins);
StepperMotor motor2(motor2_pins);
StepperMotor motor3(motor3_pins);

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < PINS_PER_STEPPER; i++) {
    pinMode(motor1_pins[i], OUTPUT);
    pinMode(motor2_pins[i], OUTPUT);
    pinMode(motor3_pins[i], OUTPUT);  
  }
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    int num_read = Serial.read();
    if (num_read != 255) {
      // Parse the message
      int motor_index = num_read & 3;
      bool dir_clockwise = ((num_read & 4) >> 2) == 1;
      int quarter_turns = (num_read >> 3);

      if (motor_index == 0) {
        if (!dir_clockwise) {
          motor1.step(dir_clockwise, STEPS_PER_SEPARATION);
        } else {
          motor1.step(dir_clockwise, STEPS_PER_SEPARATION + 1);
        } 
      }
      if (motor_index == 1) {
        motor2.step(dir_clockwise, quarter_turns * STEPS_PER_QUARTER_TURN);
      }
      if (motor_index == 2) {
        motor3.step(dir_clockwise, quarter_turns * STEPS_PER_QUARTER_TURN);  
      }
      if (motor_index == 3) {
        if (dir_clockwise) {
          StepperMotor::step_two_motors(motor3, motor2, quarter_turns * 50);
        } else {
          StepperMotor::step_two_motors(motor2, motor3, quarter_turns * 50);
        }
      }
      Serial.print((char) num_read);
    } else {
      Serial.print((char)identifier);  
    }  
  }
}
