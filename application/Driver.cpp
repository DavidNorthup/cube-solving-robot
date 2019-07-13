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
    communication::SerialManager sm2(argv[1]);
    communication::SerialManager sm(argv[2]);
    std::string s;
    while (!done) {
        std::cout << "Enter a binary number (up to 8 bits), or q to quit: ";
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

        std::cout << "Sending: " << message << std::endl;
        // Send message and retrieve output
        uint16_t c = sm.sendMessage(message);
        std::cout << "Recieved: " << c << std::endl << std::endl;
    }

    std::cout << "Exiting" << std::endl;
    return 0;
}