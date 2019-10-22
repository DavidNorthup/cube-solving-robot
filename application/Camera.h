#include <opencv2/opencv.hpp>
#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <fstream>
#include <vector>

#define RADIUS 7

#define DATA_ORDER {"BLUE", "GREEN", "ORANGE", "RED", "WHITE", "YELLOW"}
#define COLOR_ORDER {cv::Scalar(255,0,0), cv::Scalar(26,112,0), cv::Scalar(0, 130, 255), cv::Scalar(0,0,255), cv::Scalar(255,255,255), cv::Scalar(0,255,255)}
#define SFML_COLOR_ORDER {sf::Color(0, 0, 255), sf::Color(0, 112,26), sf::Color(255, 130,0), sf::Color(255, 0, 0), sf::Color(255,255,255), sf::Color(255,255,0)} 

#define BLUE_LOW    cv::Scalar(100, 50, 20)
#define BLUE_HIGH   cv::Scalar(140, 250, 250)

#define GREEN_LOW   cv::Scalar(40, 30, 0)
#define GREEN_HIGH  cv::Scalar(95, 255, 255)

#define ORANGE_LOW  cv::Scalar(15, 40, 40)
#define ORANGE_HIGH cv::Scalar(23, 255, 255)
#define ORANGE_LOW_2 cv::Scalar(10, 60, 45)
#define ORANGE_HIGH_2 cv::Scalar(15, 255, 255)

#define RED_LOW     cv::Scalar(0, 40, 0)
#define RED_HIGH    cv::Scalar(9, 255, 255)
#define RED_LOW_2   cv::Scalar(150, 20, 20)
#define RED_HIGH_2  cv::Scalar(255, 200, 200)

#define WHITE_LOW   cv::Scalar(0, 0, 60)
#define WHITE_HIGH  cv::Scalar(255, 40, 255)

#define YELLOW_LOW cv::Scalar(23, 40, 20)
#define YELLOW_HIGH  cv::Scalar(45, 255, 255)

namespace camera {

class CameraManager {

    public:
    CameraManager(int camera_index, int image_width, int image_height, std::string sampling_filename);
    ~CameraManager();
    static int getNumberOfCameras();
    bool changeCamera(int camera_index);
    std::vector<int> getData();
    std::vector<std::string> getFacelets();
    void changeCameraBrightness(double value);
    void changeCameraContrast(double value);
    void changeCameraSaturation(double value);
    void update();
    cv::Mat getFrame();
    sf::Image getDisplayableImage();
    cv::Mat getUnmarkedImage();

    private:
    cv::VideoCapture cam;
    cv::Mat current_frame, current_frame_alpha;
    std::vector<int> scanned_data;
    std::vector<cv::Point> sampling_centers;
    std::vector<std::string> sampling_center_facelets;
};


class CSRImageProcessing {
    public:
    static std::vector<int> highlightMat(cv::Mat& image, std::vector<cv::Point> sampling_centers);
    static void saveImageToFile(std::string path, cv::Mat& image);
    static bool pointInCircle(cv::Point p, cv::Point center, int radius);
    static int getReccomendation(int x, int y, int rad, cv::Mat masks[]);
};
}