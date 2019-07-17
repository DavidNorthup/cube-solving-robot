#include "Arduino.h"
#include "MyStepper.h"

#define STEPPER_DELAY 1000 // This is in microseconds


/*
This is the constructor for the stepper motor object. There is only one 
constructor and it takes an integer array as a parameter, which corresponds
to a ordered list of pin numbers connected to the arduino, which corresponds to
the order needed to proceed in to produce motion in the clockwise direction,
so for example if we have the input:

pins_clockwise: [1,2,3,4]

Then if we activate the pins by themselves in the order 1-2-3-4,
then we produce a clockwise rotation on the motor.
*/
StepperMotor::StepperMotor(int pins_clockwise[]) {
    for(int j = 0; j < PINS_PER_STEPPER; j++) {
        this->pins_clockwise[j] = pins_clockwise[j];
        this->pins_cclockwise[j] = pins_clockwise[PINS_PER_STEPPER - 1 - j];
    }
    
    i = 0;
    for (int j = 0; j < PINS_PER_STEPPER; j++) {
        pinMode(pins_clockwise[j], OUTPUT);
    }
}



/*
This function steps the specified motor clockwise if the boolean parameter is true,
or counter-clockwise if it is given as false. It steps the motor the specified number
of steps.
*/
void StepperMotor::step(bool clockwise, int num_steps) {    
    for (int j = 0; j < num_steps; j++) {
        one_step(clockwise);
        delayMicroseconds(STEPPER_DELAY);
    }
    shutoff();
}

/* 
This function steps two stepper motors at the exact same time. The first parameter 
represents the stepper motor that is going to be going clockwise, and the second motor 
is one that will go counterclockwise. The number of steps is the number of steps that each
motor will perform.
*/
static void StepperMotor::step_two_motors(StepperMotor clockwise_motor, StepperMotor counter_motor, int number_steps) {
    for (int j = 0; j < number_steps; j++) {
        clockwise_motor.one_step(true);
        counter_motor.one_step(false);
        delayMicroseconds(STEPPER_DELAY);
    }
    clockwise_motor.shutoff();
    counter_motor.shutoff();
}

/*
This function turns the motor off, by nullifying all of the output signals to the given
pins.
*/
void StepperMotor::shutoff() {
    for (int j = 0; j < PINS_PER_STEPPER; j++) {
        digitalWrite(pins_clockwise[j], LOW); 
    }  
}



/*
This function rotates the stepper motor one single step in the specified direction. 
*/
void StepperMotor::one_step(bool clockwise) {
    int step1 = i;
    int step2 = (i + 1) % PINS_PER_STEPPER;
    

    int * pins;
    if (clockwise) {
        pins = pins_clockwise;
    } else {
        pins = pins_cclockwise;
    }

    for (int j = 0; j < 4; j++) {
        if (j != step1 && j != step2) {
            digitalWrite(pins[j], LOW);  
        }   
    }

    digitalWrite(pins[step1], HIGH);
    digitalWrite(pins[step2], HIGH);
    i = step2;
}
