#ifndef TRACKOBJECT_HPP
#define TRACKOBJECT_HPP

#include <opencv/cv.h>
#include <opencv/cv.hpp>
#include <stdio.h>
#include "guiappheaders.hpp"

using namespace cv;

//const float _m[4][4] = {{1.0, 0.0, (float)REFRESH_TIMER/100.0, 0.0}, //parametrizala, asi sera reutilizable
//                  {0.0, 1.0, 0.0, (float)REFRESH_TIMER/100.0},
//                  {0.0, 0.0, 1.0, 0.0},
//                  {0.0, 0.0, 0.0, 1.0}};//transition matrix

//const float _m[2][2] = {{1, 1}, {0, 1}};

class TrackObject
{
public:
    TrackObject();
    ~TrackObject();

    virtual void initStates(const int dymParams, const int measureParams, const int controlParams, const Mat& T) = 0;
    virtual void trackObject(Mat& img) = 0;

protected:
    Mat_<float> _state;
    Mat _processNoise;
    Mat_<float> _measurement;
    Mat _measurementNoise;
    //M(t+1, t) = [I fitI]
    //            [0   I]
    KalmanFilter _kf;
};

#endif // TRACKOBJECT_HPP
