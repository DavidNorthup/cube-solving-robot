#include "Camera.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <cmath>

#define SAMPLE_LOCATIONS_FILE "training/sampling_locations.dat"
#define TEST_IMAGE_PATH "sample.jpg"

#define RADIUS 10

using namespace cv;
using namespace std;


bool pointInCircle(Point p, Point center, int radius) {
    int dx = center.x - p.x, dy = center.y - p.y;
    double distance = sqrt(dx*dx + dy*dy);

    return distance <= radius;
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cout << "Please provide data output file" << std::endl;
        return -1;
    }

    std::string outfile_name(argv[1]);
    fstream outfile(outfile_name);

    Mat image;
    Mat hsv;
    image = imread(TEST_IMAGE_PATH, CV_LOAD_IMAGE_COLOR);
    cvtColor(image, hsv, COLOR_BGR2HSV);

    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    // Load in the points
    fstream file(SAMPLE_LOCATIONS_FILE);
    std::vector<Point> points;
    int x, y;
    while (file >> x >> y) {
        Point newPoint(x,y);
        points.insert(points.end(), newPoint);
    }


    for (int i = 0; i < points.size(); i++) {
        Point center = points[i];
        circle(image, center, RADIUS, Scalar(255,0,255), -1);
    }

    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            Point p(x,y);
            for (int i = 0; i < points.size(); i++) {
                if (pointInCircle(p, points[i], RADIUS)) {
                    cv::Vec3b pixel_value = hsv.at<cv::Vec3b>(p);
                    int h = pixel_value[0];
                    int s = pixel_value[1];
                    int v = pixel_value[2];
                    outfile << h << " " << s << " " << v << std::endl;
                    break;
                }
            }
        }
    }

    file.close();
    outfile.close();
    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", image );                   // Show our image inside it.
    waitKey(0);

    return 0;
}

