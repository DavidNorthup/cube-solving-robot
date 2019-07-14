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

        if (response1 == LR_IDENTIFIER && response2 == FB_IDENTIFIER) {
            lr_manager = sm1;
            fb_manager = sm2;
        } else if (response1 == FB_IDENTIFIER && response2 == LR_IDENTIFIER) {
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

}