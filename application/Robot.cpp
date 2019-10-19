#include "Robot.h"


namespace csr {

    Robot::Robot(std::string port1, std::string port2) {

        communication::SerialManager * sm1 = new communication::SerialManager(port1);
        communication::SerialManager * sm2 = new communication::SerialManager(port2);


        std::cout << "Pasuing for " << PAUSE_DURATION / (1000*1000) << " seconds to allow  the" 
                                    << " arduino to reset before continuing." << std::endl;
        usleep(PAUSE_DURATION);

        std::cout << "Pinging first device." << std::endl;
        uint16_t response1 = sm1->sendMessage(IDENTIFICATION_PACKET);
        std::cout << "Pinging second device." << std::endl;
        uint16_t response2 = sm2->sendMessage(IDENTIFICATION_PACKET);

        std::cout << "Identifying devices" << std::endl;

        if (response1 == LR_IDENTIFIER || response2 == FB_IDENTIFIER) {
            lr_manager = sm1;
            fb_manager = sm2;
        } else if (response1 == FB_IDENTIFIER || response2 == LR_IDENTIFIER) {
            lr_manager = sm2;
            fb_manager = sm1;
        } else {
            std::cout << "Failure identifying robot, results not guarenteed." << std::endl;
            lr_manager = sm1;
            fb_manager = sm2;
        }
    }    

    Robot::~Robot() {
        std::cout << "Closing robot communication down" << std::endl;
    }

    bool Robot::sendCommand(uint16_t command, uint16_t smIdentifier) {
        uint16_t response;
        if (smIdentifier == LR_IDENTIFIER) {
            response = lr_manager->sendMessage(command);
        } else {
            response = fb_manager->sendMessage(command);
        }

        return response == command; 
    }

    void Robot::performMove(std::string move) {
        std::cout << "Performing move: " << move << std::endl;

        if (move == "R") {
            sendCommand(MOVE_R);
        } else if (move == "R'") {
            sendCommand(MOVE_R_PRIME);
        } else if (move == "R2") {
            sendCommand(MOVE_R2);
        } else if (move == "L") {
            sendCommand(MOVE_L);
        } else if (move == "L'") {
            sendCommand(MOVE_L_PRIME);
        } else if (move == "L2") {
            sendCommand(MOVE_L2);
        } else if (move == "U") {
            sendCommand(MOVE_U);
        } else if (move == "U'") {
            sendCommand(MOVE_U_PRIME);
        } else if (move == "U2") {
            sendCommand(MOVE_U2);
        } else if (move == "D") {
            sendCommand(MOVE_D);
        } else if (move == "D'") {
            sendCommand(MOVE_D_PRIME);
        } else if (move == "D2") {
            sendCommand(MOVE_D2);
        } else if (move == "F") {
            sendCommand(MOVE_F);
        } else if (move == "F'") {
            sendCommand(MOVE_F_PRIME);
        } else if (move == "F2") {
            sendCommand(MOVE_F2);
        } else if (move == "B") {
            sendCommand(MOVE_B);
        } else if (move == "B'") {
            sendCommand(MOVE_B_PRIME);
        } else if (move == "B2") {
            sendCommand(MOVE_B2);
        } else {
            std::cout << "Unrecognized Move" << std::endl;
        }
    }

    std::string Robot::reverseMove(std::string move) {
        if (move.find('\'') != std::string::npos) {
            return move.substr(0, 1);
        } else if (move.find('2') != std::string::npos) {
            return move;
        } else {
            return move + "'";
        }
    }

    std::string Robot::performMoves(std::string moves) {
        std::cout << "Performing: "  << moves << std::endl;
        std::string reverse = "";
        size_t last_space = -1;
        size_t len = 0;
        for (size_t i = 0; i < moves.length(); i++) {
            if (moves[i] == ' ') { 
                std::string move = moves.substr(last_space + 1, len);
                len = 0;
                performMove(move);
                // usleep(10000);
                reverse = reverseMove(move) + " " + reverse;
                last_space = i;
            } else {
                len ++;
            }
            
        }
        std::string move = moves.substr(last_space + 1, len);
        reverse = reverseMove(move) + " " + reverse;
        performMove(move);
        std::cout << "Reverse: " << reverse << std::endl;
        return reverse.substr(0, reverse.length() - 1);
    }

    std::string Robot::scramble(int len) {
        srand(time(NULL));
        std::string moves = "";
        std::string possible_moves[18] = MOVES; 

        for (int i = 0; i < len; i++) {
            int r = rand() % 18;
            std::cout << r << " " << possible_moves[r] << std::endl; 
            moves = possible_moves[r] + " " + moves;
        }

        std::cout << "Generated scramble: " << moves << std::endl;

        return performMoves(moves.substr(0, moves.length() - 1));
    }
}