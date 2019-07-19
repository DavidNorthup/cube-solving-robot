#include <opencv2/opencv.hpp>
#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <iostream>
#include <thread>


namespace camera {

class CameraManager {

    public:
    CameraManager(int camera_index, int image_width, int image_height);
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