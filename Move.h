#pragma once

#define INTERVAL(interval, x) interval * abs(x) / pow(SAMPLING_RANGE, 2)
#define DEFAULT_INTERVAL      0.01
#define SAMPLING_RANGE        50
#define MIN_RANGE             50
#define MIN_HIGH              1.0

using namespace std;
using namespace cv;

void flyFront(double);
void flyRight(double);
void flyBack(double);
void flyLeft(double);
void flyDown(double);
void landing();
void check(Mat &, Way &);
void move(const Way &, double);

void check(Mat &I, Way &W) {
    int w  = I.cols;
    int h  = I.rows;
    int ox = w / 2;
    int oy = h / 2;

    for(int i = ox - SAMPLING_RANGE; i < ox + SAMPLING_RANGE; i++) {
        for(int j = oy - SAMPLING_RANGE; j < oy + SAMPLING_RANGE; j++) {
            int r = I.at<Vec3b>(j, i)[2];
            int g = I.at<Vec3b>(j, i)[1];
            int b = I.at<Vec3b>(j, i)[0];

            if(     r > 200 && g > 200 && b < 50 ) W.horizontal++;
            else if(r > 128 && g < 50  && b < 50 ) W.horizontal--;
            else if(r < 50  && g < 50  && b > 128) W.vertical++;
            else if(r < 50  && g > 128 && b < 50 ) W.vertical--;
        }
    }
}

void move(const Way &W, double high) {
    cout << "vertical " << W.vertical << " horizontal " << W.horizontal << endl;
    cout << pow(SAMPLING_RANGE, 2) << endl;

    double interval = DEFAULT_INTERVAL;

    interval += high / 100;
    
    if(abs(W.vertical) < MIN_RANGE && abs(W.horizontal) < MIN_RANGE){
        if(high < MIN_HIGH)
            landing();
        else
            flyDown(MIN_HIGH);
    } else {
        double dVer = INTERVAL(interval, W.vertical);
        double dHor = INTERVAL(interval, W.horizontal);

        if(W.vertical > MIN_RANGE)
            flyFront(dVer);
        else if(W.vertical < MIN_RANGE * -1)
            flyBack(dVer);

        if(W.horizontal > MIN_RANGE)
            flyRight(dHor);
        else if(W.horizontal < MIN_RANGE * -1)
            flyLeft(dHor);
    }
}

//TODO:封裝在drone
void landing() {
    cout << "landing " << endl;
}

void flyDown(double distance) {
    cout << "fly down " << distance << endl;
}

void flyFront(double distance) {
    cout << "fly front " << distance << endl;
}

void flyBack(double distance) {
    cout << "fly back " << distance << endl;
}

void flyRight(double distance) {
    cout << "fly right " << distance << endl;
}

void flyLeft(double distance) {
    cout << "fly doleftwn " << distance << endl;
}