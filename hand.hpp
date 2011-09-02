#ifndef HAND_HPP
#define HAND_HPP

#include <iostream>
#include <vector>
#include <stdio.h>
#include <cmath>

#include <opencv/cv.h>
#include <opencv/cv.hpp>

#include "guiappheaders.hpp"
#include "utils.hpp"
#include "trackobject.hpp"

using namespace cv;
using namespace std;

class Hand:public TrackObject {//que herede de track object
public:
    Hand();
    Hand(const Hand& hand);
    ~Hand();

    int getFingers() const;
    vector<Point>* getContour() const;
    uint8_t getDepth() const;
    int getWhich() const;
    Point& getCenter() const;

    void setContour(vector<Point>& contour);
    void setDepth(uint8_t depth);
    void setFingers(int fingers);
    void setCenter(Point& center);
    void setWhich(int which);

    double getArea() const;
//    Rect* getBBox() const;

    //Debe implementar este metodo
    void trackObject(Mat &img);
    void initStates(const int dymParams, const int measureParams, const int controlParams, const Mat& T);

private:
    vector<Point>* _contour;//CvBlob
    KalmanFilter* _filter;
    Point* _center;
    int _fingers;
    uint8_t _depth;
    int _which;
};

#endif // HAND_HPP
