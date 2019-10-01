/*
This file implements all of the specific information that is going to be needed in order
to control the robot implementation. It is designed to interface the rest of the application
to the CSR robot itself.

*/

#include "SerialCommunication.h"
#include <stdlib.h>

// This number represents the minimum number of microseconds to pause to allow the arduino's 
// to reset after connecting to them.
#define PAUSE_DURATION 3*1000*1000

// These two macros are the expected values when we ping 
// the SerialManagers with message value "IDENTIFICATION_PACKET", and are
// used to determine which arduino is which, regardless of what ports they are plugged into.
#define LR_IDENTIFIER 1
#define FB_IDENTIFIER 2

// The following macros are values that can be added together in order to form a command 
// that fits within one byte.

// The two least significant bits index the motor within the controller [1-3]
#define MOTOR_1 0
#define MOTOR_2 1
#define MOTOR_3 2

// The next least significant bit identifies the direction of travel
#define DIRECTION_CLOCKWISE 0
#define DIRECTION_COUNTERCLOCKWISE (1 << 2)

// The remaining bits store the number of quarter turns desired, or in the case
// of zero, the special value used by the separator motors for separation purposes
// in the slower (ruku style) configuration. 
#define ONE_QUARTER_TURN (1 << 3)
#define TWO_QUARTER_TURNS (2 << 3)
#define THREE_QUARTER_TURNS (3 << 3)
#define FOUR_QUARTER_TURNS (4 << 3)

// Definitions for all of the valid cube moves
#define MOVE_R MOTOR_3 + DIRECTION_CLOCKWISE + ONE_QUARTER_TURN, LR_IDENTIFIER
#define MOVE_R_PRIME MOTOR_3 + DIRECTION_COUNTERCLOCKWISE + ONE_QUARTER_TURN, LR_IDENTIFIER
#define MOVE_R2 MOTOR_3 + DIRECTION_CLOCKWISE + TWO_QUARTER_TURNS, LR_IDENTIFIER

#define MOVE_L MOTOR_2 + DIRECTION_CLOCKWISE + ONE_QUARTER_TURN, LR_IDENTIFIER
#define MOVE_L_PRIME MOTOR_2 + DIRECTION_COUNTERCLOCKWISE + ONE_QUARTER_TURN, LR_IDENTIFIER
#define MOVE_L2 MOTOR_2 + DIRECTION_CLOCKWISE + TWO_QUARTER_TURNS, LR_IDENTIFIER

#define MOVE_U MOTOR_1 + DIRECTION_CLOCKWISE + ONE_QUARTER_TURN, LR_IDENTIFIER
#define MOVE_U_PRIME MOTOR_1 + DIRECTION_COUNTERCLOCKWISE + ONE_QUARTER_TURN, LR_IDENTIFIER
#define MOVE_U2 MOTOR_1 + DIRECTION_CLOCKWISE + TWO_QUARTER_TURNS, LR_IDENTIFIER

#define MOVE_F MOTOR_3 + DIRECTION_CLOCKWISE + ONE_QUARTER_TURN, FB_IDENTIFIER
#define MOVE_F_PRIME MOTOR_3 + DIRECTION_COUNTERCLOCKWISE + ONE_QUARTER_TURN, FB_IDENTIFIER
#define MOVE_F2 MOTOR_3 + DIRECTION_CLOCKWISE + TWO_QUARTER_TURNS, FB_IDENTIFIER

#define MOVE_B MOTOR_2 + DIRECTION_COUNTERCLOCKWISE + ONE_QUARTER_TURN, FB_IDENTIFIER
#define MOVE_B_PRIME MOTOR_2 + DIRECTION_CLOCKWISE + ONE_QUARTER_TURN, FB_IDENTIFIER
#define MOVE_B2 MOTOR_2 + DIRECTION_COUNTERCLOCKWISE + TWO_QUARTER_TURNS, FB_IDENTIFIER

#define MOVE_D MOTOR_1 + DIRECTION_CLOCKWISE + ONE_QUARTER_TURN, FB_IDENTIFIER
#define MOVE_D_PRIME MOTOR_1 + DIRECTION_COUNTERCLOCKWISE + ONE_QUARTER_TURN, FB_IDENTIFIER
#define MOVE_D2 MOTOR_1 + DIRECTION_CLOCKWISE + TWO_QUARTER_TURNS, FB_IDENTIFIER 

#define MOVES {"R", "R'", "R2", "L", "L'", "L2", "F", "F'", "F2", "B", "B'", "B2", "U", "U'", "U2", "D", "D'", "D2"}

#define SCRAMBLE_LENGTH 20

// So for example if we wanted to perform a move on motor 2, clockwise, with 3 quarter turns,
// we would pass the following into the sendCommand function: MOTOR_2 + DIRECTION_CLOCKWISE + THREE_QUARTER_TURNS

namespace csr{

class Robot {
    public:
    Robot(std::string port1, std::string port2);
    ~Robot();
    bool sendCommand(uint16_t command, uint16_t smIdentifier);
    void performMove(std::string move);
    std::string reverseMove(std::string move);
    std::string performMoves(std::string moves);
    std::string scramble();

    private:
    communication::SerialManager * lr_manager;
    communication::SerialManager * fb_manager;
};

}