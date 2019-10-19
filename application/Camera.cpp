#include "Camera.h"
#include <stdlib.h>
#include <cmath>

using namespace camera;

/*
This is the constructor for the Camera_Manager class. It's tasks are to 
fetch images from the connected webcam using opencv, and format them for 
display in the sfml window. For this to occur, the window will call the
update function continulosly, and a new image will be fetched as often as possible. 
*/
CameraManager::CameraManager(int camera_index, int image_width, int image_height) {
    bool opened = changeCamera(camera_index);
    if (!opened) {
        std::cout << "Failed to open the camera, closing program" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Setting camera output dimensions: " << image_width 
        << " x " << image_height << std::endl;
    std::cout << "Opening Sampling Locations File" << std::endl;
    std::fstream sampling_file(SAMPLING_FILE);
    int x, y;
    while (sampling_file >> x >> y) {
        cv::Point currentPoint(x,y);
        sampling_centers.push_back(currentPoint);
    }
    sampling_file.close();
    std::cout << "Done reading sampling locations" << std::endl;
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
This function counts the number of cameras that are connected to the computer.
*/
int CameraManager::getNumberOfCameras() {
    int count = 0;
    cv::VideoCapture camera;
    while(camera.open(count++)){}
    return count;
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

void CameraManager::changeCameraBrightness(double value) {
    cam.set(cv::CAP_PROP_BRIGHTNESS, value);
}

/*
Capture the current image from the webcam.
*/
void CameraManager::update() {
    cam >> current_frame;
    CSRImageProcessing::highlightMat(current_frame, sampling_centers);
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

/*
This function uses defined HSV ranges to figure out what color each sample (given by sampling_centers and RADIUS)
are. It then highlights the image. It will return the data for use in solving the cube as well.
*/
std::vector<int> CSRImageProcessing::highlightMat(cv::Mat& image, std::vector<cv::Point> sampling_centers) {
    std::vector<int> data;
    
    cv::Scalar colors[] = COLOR_ORDER;

    cv::Mat hsv ;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
    
    cv::Mat red_mask, red_mask_2, orange_mask, yellow_mask, white_mask, blue_mask, green_mask;
    cv::inRange(hsv, WHITE_LOW , WHITE_HIGH, white_mask);
    cv::inRange(hsv, RED_LOW , RED_HIGH, red_mask);
    cv::inRange(hsv, RED_LOW_2 , RED_HIGH_2, red_mask_2);
    cv::inRange(hsv, BLUE_LOW , BLUE_HIGH, blue_mask);
    cv::inRange(hsv, GREEN_LOW , GREEN_HIGH, green_mask);
    cv::inRange(hsv, ORANGE_LOW , ORANGE_HIGH, orange_mask);
    cv::inRange(hsv, YELLOW_LOW , YELLOW_HIGH, yellow_mask);

    red_mask = red_mask | red_mask_2;

    cv::Mat masks[] = {blue_mask, green_mask, orange_mask, red_mask, white_mask, yellow_mask};


    for (size_t i = 0; i < sampling_centers.size(); i++) {
        int rec = getReccomendation(sampling_centers[i].x, sampling_centers[i].y, RADIUS, masks);
        data.push_back(rec);
        cv::circle(image, cv::Point(sampling_centers[i].x, sampling_centers[i].y), RADIUS,  colors[rec], -1);
    }

    return data;
}


void CSRImageProcessing::saveImageToFile(std::string path, cv::Mat& image) {
    cv::imwrite(path, image);
    std::cout << "Saved image to file." << std::endl;
}

bool CSRImageProcessing::pointInCircle(cv::Point p, cv::Point center, int radius) {
    int dx = p.x - center.x, dy = p.y - center.y;
    return std::sqrt(dx*dx + dy*dy) <= radius;
}

int CSRImageProcessing::getReccomendation(int x, int y, int rad, cv::Mat masks[]) {
    int maxCount = 0, maxIndex = 0;
    for (int i = 0; i < 6; i++) {
        int count = 0;
        cv::Mat mask = masks[i].clone();
        cv::cvtColor(mask, mask, cv::COLOR_GRAY2BGR);
        for (int row = y - rad; row <= y + rad; row++) {
            for (int col = x - rad; col <= x + rad; col++) {
                if (pointInCircle(cv::Point(col, row), cv::Point(x,y), rad)) {
                    cv::Vec3b pv = mask.at<cv::Vec3b>(cv::Point(col, row));
                    if (pv[0] || pv[1] || pv[2]) {
                        count ++;
                    }
                }
            }
        }
        if (count > maxCount) {
            maxCount = count; maxIndex = i;
        }
    }
    return maxIndex;
}
