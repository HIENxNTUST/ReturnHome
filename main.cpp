#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>

#include "./Way.h"
#include "./Rotate.h"
#include "./Move.h"

#define FILE_PATH    "home.png"
#define DEFAULT_HIGH 3.0

using namespace std;
using namespace cv;

int main() {
    string path  = FILE_PATH;
    double high  = DEFAULT_HIGH; 
    Mat    image = imread(path, CV_LOAD_IMAGE_UNCHANGED);
    Way    w;

    w.vertical   = 0;
    w.horizontal = 0;

    check(image, w);
    move(w, high);

    return 0;
}