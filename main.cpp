#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>

#define DEFAULT_INTERVAL 0.02
#define DEFAULT_HIGH 3.0
#define SAMPLING_RANGE 50
#define FILE_PATH "home.png"
#define MIN_RANGE 20

using namespace std;
using namespace cv;

class Way {
public:
    Way() {
        vertical = 0;
        horizontal = 0;
    }
    int vertical;
    int horizontal;
};
void check(Mat &, Way &);
void move(const Way &, double);
void landing();
void flyDown(double);
void flyFront(double);
void flyBack(double);
void flyRight(double);
void flyLeft(double);

int main() {
    string path = FILE_PATH;
    double high = DEFAULT_HIGH; 
    Mat image = imread(path, CV_LOAD_IMAGE_UNCHANGED);
    Way w;
    w.vertical = 0;
    w.horizontal = 0;
    check(image, w);
    move(w, high);
    cout << "vertical: " << w.vertical << ", horizontal: " << w.horizontal << endl;
    return 0;
}

void check(Mat &I, Way &W) {
    int w = I.cols;
    int h = I.rows;
    int ox = w / 2;
    int oy = h / 2;
    for(int i = ox - SAMPLING_RANGE; i < ox + SAMPLING_RANGE; i++) {
        for(int j = oy - SAMPLING_RANGE; j < oy + SAMPLING_RANGE; j++) {
            int r = I.at<Vec3b>(j, i)[2];
            int g = I.at<Vec3b>(j, i)[1];
            int b = I.at<Vec3b>(j, i)[0];
            if(r > 200 && g > 200 && b < 50)
                W.horizontal++;
            else if(r > 128 && g < 50 && b < 50)
                W.horizontal--;
            else if(r < 50 && g < 50 && b > 128)
                W.vertical++;
            else if(r < 50 && g > 128 && b < 50)
                W.vertical--;
        }
    }
}

void move(const Way &W, double high) {
    double interval = DEFAULT_INTERVAL;
    interval += high / 100;
    if(abs(W.vertical) < MIN_RANGE && abs(W.horizontal) < MIN_RANGE){
        if(high < 1)
            landing();
        else
            flyDown(1.0);
    } else {
        double dVer = interval * (double)abs(W.vertical) / pow(SAMPLING_RANGE, 2) * 2;
        double dHor = interval * (double)abs(W.horizontal) / pow(SAMPLING_RANGE, 2) * 2;
        if(W.vertical > 0)
            flyFront(dVer);
        else
            flyBack(dVer);
        if(W.horizontal > 0)
            flyRight(dHor);
        else
            flyLeft(dHor);
    }
}

//TODO:封裝在drone
void landing() {

}

void flyDown(double distance) {

}

void flyFront(double distance) {

}

void flyBack(double distance) {

}

void flyRight(double distance) {

}

void flyLeft(double distance) {

}