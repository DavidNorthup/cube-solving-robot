#include <opencv2/opencv.hpp>
#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <fstream>
#include <vector>

#define RADIUS 10

#define SAMPLING_FILE "training/sampling_locations.dat"

#define DATA_ORDER {"BLUE", "GREEN", "ORANGE", "RED", "WHITE", "YELLOW"}
#define COLOR_ORDER {cv::Scalar(255,0,0), cv::Scalar(30,255,30), cv::Scalar(0, 130, 255), cv::Scalar(0,0,255), cv::Scalar(255,255,255), cv::Scalar(0,255,255)}

#define BLUE_LOW    cv::Scalar(100, 70, 50)
#define BLUE_HIGH   cv::Scalar(140, 250, 250)

#define GREEN_LOW   cv::Scalar(40, 35, 50)
#define GREEN_HIGH  cv::Scalar(95, 255, 255)

#define ORANGE_LOW  cv::Scalar(10, 40, 80)
#define ORANGE_HIGH cv::Scalar(25, 255, 255)

#define RED_LOW     cv::Scalar(0, 100, 0)
#define RED_HIGH    cv::Scalar(8, 255, 230)
#define RED_LOW_2   cv::Scalar(150, 20, 20)
#define RED_HIGH_2  cv::Scalar(255, 200, 200)

#define WHITE_LOW   cv::Scalar(0, 0, 100)
#define WHITE_HIGH  cv::Scalar(255, 45, 255)

#define YELLOW_HIGH cv::Scalar(26, 20, 20)
#define YELLOW_LOW  cv::Scalar(45, 255, 255)

namespace camera {

class CameraManager {

    public:
    CameraManager(int camera_index, int image_width, int image_height);
    ~CameraManager();
    static int getNumberOfCameras();
    bool changeCamera(int camera_index);
    void changeCameraBrightness(double value);
    void update();
    cv::Mat getFrame();
    sf::Image getDisplayableImage();

    private:
    cv::VideoCapture cam;
    cv::Mat current_frame, current_frame_alpha;
    std::vector<cv::Point> sampling_centers;
    std::vector<double> avg_h, avg_s, avg_v;
};


class CSRImageProcessing {
    public:
    static std::vector<int> highlightMat(cv::Mat& image, std::vector<cv::Point> sampling_centers);
    static void saveImageToFile(std::string path, cv::Mat& image);
    static bool pointInCircle(cv::Point p, cv::Point center, int radius);
    static int getReccomendation(int x, int y, int rad, cv::Mat masks[]);
};
}