#ifndef StepperMotor_h
#define StepperMotor_h

#define PINS_PER_STEPPER 4 

class StepperMotor
{
    public:
        StepperMotor(int pins_clockwise_order[]);
        void step(bool direction_clockwise, int number_steps);
        static void step_two_motors(StepperMotor clockwise_motor, StepperMotor counter_motor, int number_steps);
        void hold();
        void shutoff();
    private:
        int pins_clockwise[PINS_PER_STEPPER];
        int pins_cclockwise[PINS_PER_STEPPER];
        int i;
        void one_step(bool clockwise);
};

#endif
