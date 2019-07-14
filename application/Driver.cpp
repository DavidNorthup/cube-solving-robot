#include <iostream>


#include "SerialCommunication.h"

/*
Currently the program attempts to communicate with the arudinos one byte 
at a time via serial connenctions on the COM ports. 
*/
int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout << "Usage example: " << argv[0] << " [Path to first] [Path to second]" << std::endl;
        return 1;
    }
    bool done = false;
    communication::SerialManager sm(argv[1]);
    communication::SerialManager sm2(argv[2]);
    std::string s;
    while (!done) {
        std::cout << "Enter a binary number (up to 5 bits), or q to quit: ";
        std::cin >> s;
        // Convert input to an int
        uint16_t message = 0;

        // Process the input string
        for (size_t i = 0; i < s.size(); i++) {
            // Add to the message input if the character is valid
            if (s[i] == '1' || s[i] == '0') {
                uint16_t on = 1;
                if (s[i] == '0') { on = 0; }
                message = (message << 1) + on;
            } 
            // Quit if the message contains q.
            if (s[i] == 'q') {
                message = 0;
                done = true;
                break;
            }
        }

        
        communication::SerialManager sm_temp = sm;
        std::string selection;
        if (!done) {
            std::cout << "Which serial port would you like to output to? [1:" << argv[1] << "] or [2:" <<  argv[2] << "] - ";
            std::cin >> selection;
            std::cout << "Sending: " << message << std::endl;
            if (selection.compare("1") != 0) {sm_temp = sm2;}
            // Send message and retrieve output
            uint16_t c = sm_temp.sendMessage(message);
            std::cout << "Recieved: " << c << std::endl << std::endl;
        } else {
            uint16_t c = sm.sendMessage(message);
            c = sm2.sendMessage(message);
        }
    }

    std::cout << "Exiting" << std::endl;
    return 0;
}