#include <opencv2/highgui.hpp>
#include "Camera.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdint.h>

using namespace cv;

#define RADIUS 10



bool inCircle(int x, int y, int center_x, int center_y) {
    int dx = x - center_x, dy = y - center_y;
    return dx*dx + dy*dy <= RADIUS * RADIUS;
}

int getRecommendation (int x, int y, int rad, Mat masks[]) {
    int maxCount = 0, maxIndex = 0;
    for (int i = 0; i < 6; i++) {
        int count = 0;
        Mat mask = masks[i].clone();
        cvtColor(mask, mask, COLOR_GRAY2BGR);
        for (int row = y - rad; row <= y + rad; row++) {
            for (int col = x - rad; col <= x + rad; col++) {
                if (inCircle(col, row, x, y)) {
                    Vec3b pv = mask.at<Vec3b>(cv::Point(col, row));
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

int main (int argc, char* argv[]) {
    
    if (argc < 2) {
        std::cout << "Usage example: ./" << argv[0] << " [image file name]";
    }

    // Load in the sampling data
    std::vector<int> sampling_centers_x, sampling_centers_y;

    std::cout << "Loading sampling points" << std::endl;
    std::fstream sampling_file(SAMPLING_FILE);
    int x, y;
    while (sampling_file >> x >> y) {
        sampling_centers_x.push_back(x);
        sampling_centers_y.push_back(y);
    }
    sampling_file.close();

    // Load the image 
    Mat image = imread(argv[1], IMREAD_COLOR);
    int index = 0;

    if(! image.data )                              // Check for invalid input
    {
        std::cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    Mat hsv ;
    cvtColor(image, hsv, COLOR_BGR2HSV);
    
    cv::Scalar red_low(0, 100, 0);
    cv::Scalar red_high(8, 255, 170);
    cv::Scalar red_low_2(150, 20, 20);
    cv::Scalar red_high_2(255, 200, 200);
    cv::Mat red_mask;
    cv::Mat red_mask_2;
    cv::Scalar orange_low(10, 40, 80);
    cv::Scalar orange_high(25, 255, 250);
    cv::Mat orange_mask;
    cv::Scalar yellow_low(26, 20, 20);
    cv::Scalar yellow_high(45, 255, 255);
    cv::Mat yellow_mask;
    cv::Scalar white_low(0, 0, 100);
    cv::Scalar white_high(255, 35, 255);
    cv::Mat white_mask;
    cv::Scalar blue_low(100, 70, 50);
    cv::Scalar blue_high(140, 250, 250);
    cv::Mat blue_mask;
    cv::Scalar green_low(40, 35, 50);
    cv::Scalar green_high(95, 255, 255);
    cv::Mat green_mask;



    inRange(hsv,white_low , white_high, white_mask);
    inRange(hsv,red_low , red_high, red_mask);
    inRange(hsv,red_low_2 , red_high_2, red_mask_2);
    inRange(hsv,blue_low , blue_high, blue_mask);
    inRange(hsv,green_low , green_high, green_mask);
    inRange(hsv, orange_low , orange_high, orange_mask);
    inRange(hsv, yellow_low , yellow_high, yellow_mask);

    red_mask = red_mask | red_mask_2;

    Mat masks[] = {blue_mask, green_mask, orange_mask, red_mask, white_mask, yellow_mask};
    Scalar colors[] = COLOR_ORDER;

    std::cout << "Made masks" << std::endl;

    for (size_t i = 0; i < sampling_centers_x.size(); i++) {
        int rec = getRecommendation(sampling_centers_x[i], sampling_centers_y[i], RADIUS, masks);
        std::cout << sampling_centers_x[i] << ", " << sampling_centers_y[i] << " color: " << rec << std::endl;
        cv::circle(image, cv::Point(sampling_centers_x[i], sampling_centers_y[i]), RADIUS,  colors[rec], -1);
    }


    for (int i = 2; i < 4; i++) {
        namedWindow("Training Client", WINDOW_AUTOSIZE);
        imshow("Training Client", masks[i]);
        waitKey(0);
    }

    namedWindow("Training Client", WINDOW_AUTOSIZE);
    imshow("Training Client", image);
    waitKey(0);

    return 0;
}