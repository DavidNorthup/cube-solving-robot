#include "SerialCommunication.h"


namespace communication {
/*
Source examples are from here:
https://chrisheydrick.com/2012/06/17/how-to-read-serial-data-from-an-arduino-in-linux-with-c-part-3/

This function constructs a serial manager for communication  between the computer 
application and my arduino device. It sends one byte at a time, and blocks until
a reponse is receieved.

Parameter summary:
    std::string port_name: This is the port name for serial communication with the device.
                            in my case these are: /dev/ttyUSB0, and /dev/ttyUSB1.

Returns the constructed manager for communication.
*/
SerialManager::SerialManager(std::string port_name) {
    std::cout << "Connecting to: " << port_name << std::endl;
    // Open file descriptor in non blocking mode
    file_descriptor = open(port_name.c_str(), O_RDWR | O_NOCTTY);

    // Set up the control structure
    struct termios toptions;
    tcgetattr(file_descriptor, &toptions);

    cfsetispeed(&toptions, B9600);
    cfsetospeed(&toptions, B9600);

    toptions.c_cflag &= ~PARENB;
    toptions.c_cflag &= ~CSTOPB;
    toptions.c_cflag &= ~CSIZE;
    toptions.c_cflag |= CS8;
    /* no hardware flow control */
    toptions.c_cflag &= ~CRTSCTS;
    /* enable receiver, ignore status lines */
    toptions.c_cflag |= CREAD | CLOCAL;
    /* disable input/output flow control, disable restart chars */
    toptions.c_iflag &= ~(IXON | IXOFF | IXANY);
    /* disable canonical input, disable echo,
    disable visually erase chars,
    disable terminal-generated signals */
    toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    /* disable output processing */
    toptions.c_oflag &= ~OPOST;

    // Make the read function blocking, until [READ_SIZE] bytes are read.
    toptions.c_cc[VMIN] = READ_SIZE;
    toptions.c_cc[VTIME] = 0;

    // Apply the io options
    tcsetattr(file_descriptor, TCSANOW, &toptions);
}


/*
This function wipes both of the buffers for communication upon destruction of the manager.
*/
SerialManager::~SerialManager() {
    // Wipe the buffers
    tcflush(file_descriptor, TCIOFLUSH); 
}


/*
This function is the implemented method for sending a single byte message to the 
serial port this manager handles, it could be easily modified to support any
number of bytes however. 

Parameter summary:
    uint16_t message: this is the message that is to be sent to the serial port

Return:
    uint16_t: This is the response that the serial port sends back, this function
              WILL block the current thread until a response is recieved. 
*/
uint16_t SerialManager::sendMessage(uint16_t message) {
    // Send the messages
    char msg = (char) message;
    write(file_descriptor, &msg, sizeof(char));
    // Wait for a response, block until recieved 
    memset(buffer, 0, BUFFER_SIZE * sizeof(char));
    read(file_descriptor, buffer, BUFFER_SIZE);
    uint16_t response = (uint8_t) buffer[0];
    // std::cout << "Sent: " << message << " " << "recieved " << response << std::endl;
    return response;
}


/*
Ping the robot using a defined packet of information.
*/
bool SerialManager::isConnected() {
    uint16_t message = PING_PACKET;
    uint16_t response = sendMessage(message);
    return message == response;
}

}