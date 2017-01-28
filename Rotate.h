#pragma once

#include <vector>

#define PI                        3.1415926
#define MAX_DIFF_LEN              5.0 //The maximum length of difference
#define MAX_DIFF_ANGLE            5.0 //The maximum angle of difference

#define LENGTH(x1, x2, y1, y2)    sqrt((pow((x1) - (x2), 2) + pow((y1) - (y2), 2)))
#define MID_POINT(x1, x2, y1, y2) Point((((x1) + (x2)) / 2), ((y1) + y2) / 2)

#define IS_RED(r, g, b)           (r > 128 && g < 50  && b < 50  ? 1 : 0)
#define IS_BLUE(r, g, b)          (r < 50  && g < 50  && b > 128 ? 1 : 0)
#define IS_GREEN(r, g, b)         (r < 50  && g > 128 && b < 50  ? 1 : 0)
#define IS_YELLOW(r, g, b)        (r > 200 && g > 200 && b < 50  ? 1 : 0)

using namespace std;
using namespace cv;

typedef vector<vector<Point>> vecPoint;

double checkAngle(Mat &);
double getAngle(Mat &, vector<Point> &);
void   getPoint(vecPoint &);
void   getSquare(vecPoint &, vector<Point> &);
bool   rotate(double);


/*
*Author: blackkite0206233
*Description: This function is used to calculate the angle which is the drone should be rotated
*Usage: 
    I: The picture took by drone
*/
double checkAngle(Mat &I) {
    vector<Point> square;
    vecPoint      contours;
    Mat           gray;

    cvtColor(I, gray, CV_BGR2GRAY);
    findContours(gray, contours, RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    getPoint(contours);
    getSquare(contours, square);

    return getAngle(I, square);
}

/*
*Author: blackkite0206233
*Description: This function is used to filter the correct feature points
*Usage: 
    contours: A set with all the feature points made by findContours function
*/
void getPoint(vecPoint &contours) {

    //Traveled all the feature points in the set
    for(int i = 0; i < contours.size(); i++){
        for(int j = 0; j < contours[i].size(); j++) {
            int x1 = contours[i][j].x;
            int y1 = contours[i][j].y;

            /* 
            * Compare a feature point with others, 
            * if the distance between two points is less than the difference value, 
            * then they can be considered the same point
            */
            for(int k = j + 1; k < contours[i].size(); k++) {
                int x2 = contours[i][k].x;
                int y2 = contours[i][k].y;
                if(LENGTH(x1, x2, y1, y2) < MAX_DIFF_LEN) {
                    contours[i].erase(contours[i].begin() + k);
                    k--;
                }
            }
        }
    }
}

/*
*Author: blackkite0206233
*Description: This function is used to find the square we want
*Usage: 
    contours: A set with all the feature points made by findContours function
    square  : A vector used to record the vertex of the square
*/
void getSquare(vecPoint &contours, vector<Point> &square) {
    int  i, j;
    bool flag = false;

    //Traveled all the shapes in the set
    for(i = 0; i < contours.size() && !flag; i++) {
        vector<Point> c = contours[i];

        /*
        * If the shape is made up of 4 sides,
        * and the difference length of 4 sides are lass then the difference value, 
        * then the shape is which we want
        */
        if(c.size() == 4) {
            double d[4];
            for(j = 0; j < 4; j++) {
                d[j] = LENGTH(c[j % 4].x, c[(j + 1) % 4].x, c[j % 4].y, c[(j + 1) % 4].y);
                if(j > 0 && fabs(d[j] - d[j - 1]) > MAX_DIFF_LEN)
                    break;
            }
            if(j == 4) flag = true;  
        }
    }
    if(flag) square.assign(contours[i - 1].begin(), contours[i - 1].end());
}

/*
*Author: blackkite0206233
*Description: This function is used to get the rotated angle of the image took by drone
*Usage: 
    I: The image took by drone
    square  : A vector used to record the vertex of the square
*/
double getAngle(Mat &I, vector<Point> &square) {
    Point         mid[2];
    Point         o, qut;
    double        angle;

    mid[0] = MID_POINT(square[0].x, square[1].x, square[0].y, square[1].y); //The mid-point of one side
    mid[1] = MID_POINT(square[2].x, square[3].x, square[2].y, square[3].y); //The mid-point of another side
    o      = MID_POINT(mid[0].x, mid[1].x, mid[0].y, mid[1].y);             //The center point og the shape
    qut    = MID_POINT(mid[0].x, o.x, mid[0].y, o.y);                       //The point between mid[0] and o

    /*
    * Calculate the slope between mid[0] and mid[1],
    * and use atan2 to get the rotated angle of the shape 
    */
    angle = atan2(mid[1].y - mid[0].y, mid[1].x - mid[0].x) * 180 / PI;     

    int r = I.at<Vec3b>(qut.y, qut.x)[2];
    int g = I.at<Vec3b>(qut.y, qut.x)[1];
    int b = I.at<Vec3b>(qut.y, qut.x)[0];

    if(IS_RED(r, g, b))        angle += 180; 
    else if(IS_BLUE(r, g, b))  angle -= 90;  
    else if(IS_GREEN(r, g, b)) angle += 90;  

    return angle;
}

/*
*Author: blackkite0206233
*Description: This function is used to control the drone
*Usage: 
    angle: The angle which is the drone should be rotated
*/
bool rotate(double angle) {
    if(fabs(angle) > MAX_DIFF_ANGLE) {
        cout << angle << endl;
        return true;
    } else
        return false;
}