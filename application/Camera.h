#include <opencv2/opencv.hpp>
#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <iostream>
#include <thread>

#define FRAMERATE 60

namespace camera {

class CameraManager {

    public:
    CameraManager(int camera_index);
    ~CameraManager();
    bool changeCamera(int camera_index);
    void update();
    cv::Mat getFrame();
    sf::Image getDisplayableImage();

    private:
    cv::VideoCapture cam;
    cv::Mat current_frame, current_frame_alpha;
};

}