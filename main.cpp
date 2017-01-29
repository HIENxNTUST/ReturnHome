#include <iostream>
#include <cmath>
#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>

//#include "./Way.h"
#include "./Rotate.h"
#include "./Move.h"

#define FILE_PATH      "home.png"
#define DEFAULT_HEIGHT 3.0

using namespace std;
using namespace cv;

/*
*Author: blackkite0206233
*Description: The main function of return-home,
    the current height of drone and the image's path are default value,
    they will be changed into parameters later.
*/
int main() {
    string path   = FILE_PATH;
    double height = DEFAULT_HEIGHT;
    double angle;
    Mat    image;
    Dir    d;
    
    image        = imread(path, CV_LOAD_IMAGE_UNCHANGED);
    angle        = checkAngle(image);
    d.vertical   = 0;
    d.horizontal = 0;

    if(!rotate(angle)) {
        checkDistance(image, d);
        move(d, height);
    }

    return 0;
}