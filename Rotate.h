#pragma once

#include <vector>

#define PI                        3.1415926
#define MAX_LENGTH                5.0    
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

void getPoint(vecPoint &contours) {
    for(int i = 0; i < contours.size(); i++){
        for(int j = 0; j < contours[i].size(); j++) {
            int x1 = contours[i][j].x;
            int y1 = contours[i][j].y;
            for(int k = j + 1; k < contours[i].size(); k++) {
                int x2 = contours[i][k].x;
                int y2 = contours[i][k].y;
                if(LENGTH(x1, x2, y1, y2) < MAX_LENGTH) {
                    contours[i].erase(contours[i].begin() + k);
                    k--;
                }
            }
        }
    }
}

void getSquare(vecPoint &contours, vector<Point> &square) {
    int  i, j;
    bool flag = false;
    for(i = 0; i < contours.size() && !flag; i++) {
        vector<Point> c = contours[i];
        if(c.size() == 4) {
            double d[4];
            for(j = 0; j < 4; j++) {
                d[j] = LENGTH(c[j % 4].x, c[(j + 1) % 4].x, c[j % 4].y, c[(j + 1) % 4].y);
                if(j > 0 && fabs(d[j] - d[j - 1]) > MAX_LENGTH)
                    break;
            }
            if(j == 4) flag = true;  
        }
    }
    if(flag) square.assign(contours[i - 1].begin(), contours[i - 1].end());
}

double getAngle(Mat &I, vector<Point> &square) {
    Point         mid[2];
    Point         o, qut;
    double        angle;

    mid[0] = MID_POINT(square[0].x, square[1].x, square[0].y, square[1].y);
    mid[1] = MID_POINT(square[2].x, square[3].x, square[2].y, square[3].y);
    o      = MID_POINT(mid[0].x, mid[1].x, mid[0].y, mid[1].y);
    qut    = MID_POINT(mid[0].x, o.x, mid[0].y, o.y);

    angle = atan2(mid[1].y - mid[0].y, mid[1].x - mid[0].x) * 180 / PI;

    int r = I.at<Vec3b>(qut.y, qut.x)[2];
    int g = I.at<Vec3b>(qut.y, qut.x)[1];
    int b = I.at<Vec3b>(qut.y, qut.x)[0];

    if(IS_RED(r, g, b))        angle += 180; //red
    else if(IS_BLUE(r, g, b))  angle -= 90;  //blue
    else if(IS_GREEN(r, g, b)) angle += 90;  //green

    return angle;
}

bool rotate(double angle) {
    if(fabs(angle) > 5) {
        cout << angle << endl;
        return true;
    } else
        return false;
}