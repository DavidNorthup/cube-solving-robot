#include "Camera.h"
#include <stdlib.h>

using namespace camera;

/*
This is the constructor for the Camera_Manager class. It's tasks are to 
fetch images from the connected webcam using opencv, and format them for 
display in the sfml window. For this to occur, the window will call the
update function continulosly, and a new image will be fetched as often as possible. 
*/
CameraManager::CameraManager(int camera_index) {
    bool opened = changeCamera(camera_index);
    if (!opened) {
        std::cout << "Failed to open the camera, closing program" << std::endl;
        exit(EXIT_FAILURE);
    }
    cam >> current_frame; // Assure this is never null
}

/*
This is the deconstructor for the camera. Pretty simple, just make sure that we 
release the camera.
*/
CameraManager::~CameraManager() {
    std::cout << "Shutting down camera" << std::endl;
}


/*
This function allows for a swap of the camera. 
*/
bool CameraManager::changeCamera(int camera_index) {
    std::cout << "Attempting to open camera number: " << camera_index << std::endl;
    cv::VideoCapture temp;
    bool success = temp.open(camera_index);
    if (success) {
        cam = temp;
    }
    return success;
}

/*
Capture the current image from the webcam.
*/
void CameraManager::update() {
    cam >> current_frame;
}

/*
This function returns the current grabbed image in the opencv Mat format.
*/
cv::Mat CameraManager::getFrame() {
    return current_frame;
}

/*
This function returns an sfml image that will be displayed by the application.
*/
sf::Image CameraManager::getDisplayableImage() {
    sf::Image image;
    cv::cvtColor(current_frame, current_frame_alpha, cv::COLOR_BGR2RGBA);
    image.create(current_frame_alpha.cols, current_frame_alpha.rows, current_frame_alpha.ptr());
    return image;
}
