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
    cam.set(CV_CAP_PROP_FRAME_WIDTH, image_width);
    cam.set(CV_CAP_PROP_FRAME_HEIGHT, image_height);
    cam.set(CV_CAP_PROP_FOURCC, CV_FOURCC('M', 'J', 'P', 'G'));
    std::cout << "Setting camera output dimensions: " << image_width 
        << " x " << image_height << std::endl;
    std::cout << "Opening Sampling Locations File" << std::endl;
    std::fstream sampling_file(SAMPLING_FILE);
    std::string facelets[] = FACELETS;
    int x, y;
    int i = 0;
    while (sampling_file >> x >> y) {
        cv::Point currentPoint(x,y);
        sampling_centers.insert(std::pair<std::string, cv::Point>(facelets[i++], currentPoint));
    }
    sampling_file.close();
    std::cout << "Done reading sampling locations" << std::endl;
    std::cout << "Reading traning data" << std::endl;
    std::string file_paths[] = DATA_FILES;
    for (int i = 0; i < 6; i++) {
        std::string path = file_paths[i];
        std::fstream file_data(path);
        double h=0,s=0,v=0, sh=0,ss=0,sv=0;
        int counted = 0;
        while (file_data >> h >> s >> v) {
            sh += h;
            ss += s;
            sv += v;
            counted++;
        }
        avg_h.insert(avg_h.end(), sh / counted);
        avg_s.insert(avg_s.end(), ss / counted);
        avg_v.insert(avg_v.end(), sv / counted);
        file_data.close();
    }
    std::cout << "Done reading in training data" << std::endl;
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
    cam.set(CV_CAP_PROP_BRIGHTNESS, value);
}

/*
Capture the current image from the webcam.
*/
void CameraManager::update() {
    cam >> current_frame;
    CSRImageProcessing::highlightMat(current_frame, sampling_centers, avg_h, avg_s, avg_v);
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

void CSRImageProcessing::highlightMat(cv::Mat& image, std::map<std::string, cv::Point> sampling_centers,
    std::vector<double> avg_h, std::vector<double> avg_s, std::vector<double> avg_v ) {
    std::string facelets[] = FACELETS;
    cv::Scalar colors[] = COLOR_ORDER;
    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
    for (int i = 0; i < 9; i++) {
        cv::Point center = sampling_centers.find(facelets[i])->second;
        // double sumh=0, sums=0, sumv=0;
        // int counted=0;
        int counts[] = {0,0,0,0,0,0};
        // Attempt to get the approximate color here
        for (int x = center.x - RADIUS; x <= center.x + RADIUS; x++) {
            for (int y = center.y - RADIUS; y <= center.y + RADIUS; y++) {
                cv::Point p(x,y);
                if (pointInCircle(p, center, RADIUS)) {
                    cv::Vec3b pixel_value = hsv.at<cv::Vec3b>(p);
                    // sumh += pixel_value[0];
                    // sums += pixel_value[1];
                    // sumv += pixel_value[2];
                    // counted++;
                    int r = getReccomendation(avg_h, avg_s, avg_v, pixel_value[0], 
                        pixel_value[1], pixel_value[2]);
                        counts[r] += 1; 
                }
            }
        }
        // Get the color from the data
        // int rec = getReccomendation(avg_h, avg_s, avg_v, sumh/counted, sums/counted, sumv/counted);
        int rec = 0, max = 0;
        for (int i = 0; i < 6; i++) {
            if (counts[i] > max) {
                rec = i;
                max = counts[i];
            }
        }
        cv::circle(image, center, RADIUS, colors[rec], -1);
    }
}

void CSRImageProcessing::saveImageToFile(std::string path, cv::Mat& image) {
    cv::imwrite(path, image);
    std::cout << "Saved image to file." << std::endl;
}

bool CSRImageProcessing::pointInCircle(cv::Point p, cv::Point center, int radius) {
    int dx = p.x - center.x, dy = p.y - center.y;
    return std::sqrt(dx*dx + dy*dy) <= radius;
}

int CSRImageProcessing::getReccomendation(std::vector<double> avg_h, std::vector<double> avg_s, 
    std::vector<double> avg_v, double h, double s, double v) {
        double min_delta = 100000;
        int min_index = 0;
        for (size_t i = 0; i < avg_h.size(); i++) {
            int dh = avg_h[i] - h;
            int ds = avg_s[i] - s;
            int dv = avg_v[i] - v;
            double delta = std::sqrt(dh*dh + ds*ds + dv*dv);
            if (delta < min_delta) {
                min_delta = delta;
                min_index = i;
            }
        }
    return min_index;
}