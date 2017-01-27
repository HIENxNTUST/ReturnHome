#pragma once

#define DISTANCE(distance, x) distance * abs(x) / pow(SAMPLING_RANGE, 2)
#define DEFAULT_DISTANCE      0.01
#define SAMPLING_RANGE        50
#define MIN_HEIGHT            1.0
#define MIN_RANGE             50

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

/*
*Author: blackkite0206233
*Description: This function is used to calculate the distance between the drone and home
*Usage: 
    I: The picture took by drone
    W: A class to record the distance of vertical and horizontal between the drone and home
*/
void check(Mat &I, Way &W) {
    int w  = I.cols;
    int h  = I.rows;
    int ox = w / 2; //The x coordinate of center point
    int oy = h / 2; //The y coordinate of center point

    /*
    * Traveled all the pixels in the range of 
    * x in [ox - SAMPLING_RANGE, ox + SAMPLING_RANGE] and 
    * y in [oy - SAMPLING_RANGE, oy + SAMPLING_RANGE]
    */
    for(int i = ox - SAMPLING_RANGE; i < ox + SAMPLING_RANGE; i++) {
        for(int j = oy - SAMPLING_RANGE; j < oy + SAMPLING_RANGE; j++) {
            //get RGB value
            int r = I.at<Vec3b>(j, i)[2];
            int g = I.at<Vec3b>(j, i)[1];
            int b = I.at<Vec3b>(j, i)[0];

            //calculate the distance
            if(     r > 200 && g > 200 && b < 50 ) W.horizontal++;
            else if(r > 128 && g < 50  && b < 50 ) W.horizontal--;
            else if(r < 50  && g < 50  && b > 128) W.vertical++;
            else if(r < 50  && g > 128 && b < 50 ) W.vertical--;
        }
    }
}

/*
*Author: blackkite0206233
*Description: This function is used to control the drone
*Usage: 
    W: A class to record the distance of vertical and horizontal between the drone and home
    height: The height of drone
*/
void move(const Way &W, double height) {
    double distance = DEFAULT_DISTANCE; //The reference value of flying distance

    distance += height / 100;
    
    if(abs(W.vertical) < MIN_RANGE && abs(W.horizontal) < MIN_RANGE){
        if(height < MIN_HEIGHT)
            landing();
        else
            flyDown(MIN_HEIGHT);
    } else {
        double dVer = DISTANCE(distance, W.vertical);   //The vertical flying distance of the drone
        double dHor = DISTANCE(distance, W.horizontal); //The horizontal flying distance of the drone

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