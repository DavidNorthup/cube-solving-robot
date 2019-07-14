#include <iostream>


#include "Robot.h"

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

    csr::Robot robot(argv[1], argv[2]);
    std::cout << "Exiting" << std::endl;
    return 0;
}