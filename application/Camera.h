#include <opencv2/opencv.hpp>
#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <fstream>
#include <vector>

#define RADIUS 20

#define FACELET_1 "facelet_1"
#define FACELET_2 "facelet_2"
#define FACELET_3 "facelet_3"
#define FACELET_4 "facelet_4"
#define FACELET_5 "facelet_5"
#define FACELET_6 "facelet_6"
#define FACELET_7 "facelet_7"
#define FACELET_8 "facelet_8"
#define FACELET_9 "facelet_9"
#define FACELETS {FACELET_1, FACELET_2, FACELET_3, FACELET_4, FACELET_5, FACELET_6, FACELET_7, FACELET_8, FACELET_9}

#define SAMPLING_FILE "training/sampling_locations.dat"

#define DATA_ORDER {"BLUE", "GREEN", "ORANGE", "RED", "WHITE", "YELLOW"}
#define COLOR_ORDER {cv::Scalar(255,0,0), cv::Scalar(30,255,30), cv::Scalar(0, 130, 255), cv::Scalar(0,0,255), cv::Scalar(255,255,255), cv::Scalar(0,255,255)}
#define DATA_FILES {"training/blue.dat", "training/green.dat", "training/orange.dat", "training/red.dat", "training/white.dat", "training/yellow.dat"}

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
    std::map<std::string, cv::Point> sampling_centers;
    std::vector<double> avg_h, avg_s, avg_v;
};


class CSRImageProcessing {
    public:
    static void highlightMat(cv::Mat& image, std::map<std::string, cv::Point> sampling_centers,
        std::vector<double> avg_h, std::vector<double> avg_s, std::vector<double> avg_v);
    static void saveImageToFile(std::string path, cv::Mat& image);
    static bool pointInCircle(cv::Point p, cv::Point center, int radius);
    static int getReccomendation(std::vector<double> avg_h, std::vector<double> avg_s, std::vector<double> avg_v, 
        double h, double s, double v);
};
}