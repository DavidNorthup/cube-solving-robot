#include <opencv2/highgui.hpp>
#include "Camera.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdint.h>

using namespace cv;



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
    
    if (argc < 3) {
        std::cout << "Usage example: ./" << argv[0] << " [image file name] [sampling points file]";
    }

    // Load in the sampling data
    std::vector<int> sampling_centers_x, sampling_centers_y;

    std::cout << "Loading sampling points" << std::endl;
    std::fstream sampling_file(argv[2]);
    int x, y;
    std::string facelet;
    while (sampling_file >> x >> y >> facelet) {
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
    
    cv::Mat red_mask, red_mask_2, orange_mask, orange_mask_2,  yellow_mask, white_mask, blue_mask, green_mask;
    cv::inRange(hsv, WHITE_LOW , WHITE_HIGH, white_mask);
    cv::inRange(hsv, RED_LOW , RED_HIGH, red_mask);
    cv::inRange(hsv, RED_LOW_2 , RED_HIGH_2, red_mask_2);
    cv::inRange(hsv, BLUE_LOW , BLUE_HIGH, blue_mask);
    cv::inRange(hsv, GREEN_LOW , GREEN_HIGH, green_mask);
    cv::inRange(hsv, ORANGE_LOW , ORANGE_HIGH, orange_mask);
    cv::inRange(hsv, ORANGE_LOW_2 , ORANGE_HIGH_2, orange_mask_2);
    cv::inRange(hsv, YELLOW_LOW , YELLOW_HIGH, yellow_mask);
    orange_mask = orange_mask | orange_mask_2;
    red_mask = red_mask | red_mask_2;

    Mat masks[] = {blue_mask, green_mask, orange_mask, red_mask, white_mask, yellow_mask};
    Scalar colors[] = COLOR_ORDER;

    std::cout << "Made masks" << std::endl;

    for (size_t i = 0; i < sampling_centers_x.size(); i++) {
        int rec = getRecommendation(sampling_centers_x[i], sampling_centers_y[i], RADIUS, masks);
        cv::circle(image, cv::Point(sampling_centers_x[i], sampling_centers_y[i]), RADIUS,  colors[rec], -1);
    }

    std::string names[] = DATA_ORDER;

    for (int i = 0; i < 6; i++) {
        std::string window_name = names[i] + " mask";
        namedWindow(window_name, WINDOW_AUTOSIZE);
        imshow(window_name, masks[i]);
    }

    namedWindow("Training Client", WINDOW_AUTOSIZE);
    imshow("Training Client", image);
    waitKey(0);

    return 0;
}