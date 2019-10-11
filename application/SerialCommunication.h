/*
This file implements a serial communication manager  class between this application 
and the arduino unit. It will manage the communication to a single unit.
*/
#include <string>
#include <stdint.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h> 
#include <unistd.h>

// The size to be allocated for the buffer to read.
#define BUFFER_SIZE 16

// Only read 1 byte at a time
#define READ_SIZE 1

#define PING_PACKET 254

#define IDENTIFICATION_PACKET 255

namespace communication {

class SerialManager {
    public:
    SerialManager(std::string port_name);
    ~SerialManager();
    uint16_t sendMessage(uint16_t message);
    bool isConnected();

    private:
    char buffer[BUFFER_SIZE]; 
    int file_descriptor;
};
}