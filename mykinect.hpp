/* 
 * File:   mykinect.hpp
 * Author: cabreracanal
 *
 * Created on 26 de febrero de 2011, 13:51
 */

#ifndef MYKINECT_HPP
#define	MYKINECT_HPP
#define	 FRAMES_BETWEEN 24 // determina el num. de frames entre el 
                                    // 1er capturado y el ultimo.

#include "libfreenect.hpp"
#include "btcomm.hpp"
#include <iostream>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <cmath>
#include <exception>
#include <pthread.h>


#include <opencv/cv.h>
#include <opencv/cv.hpp>
#include <opencv/cxcore.h>
#include <opencv/cxcore.hpp>
#include <opencv/highgui.h>
#include <opencv/highgui.hpp>
#include <opencv/cxmat.hpp>

using namespace std;
using namespace cv;


class Mutex {
public:

    Mutex();
    void lock();
    void unlock();
private:
    pthread_mutex_t m_mutex;
};

class Mutex;
class MyFreenectDevice : public Freenect::FreenectDevice {
public:
    double degrees;
    MyFreenectDevice(freenect_context *_ctx, int _index);
    ~MyFreenectDevice();

    void VideoCallback(void* _rgb, uint32_t timestamp);
    void DepthCallback(void* _depth, uint32_t timestamp);

    bool getVideo(Mat& output);
    bool getDepth(Mat& output);

private:
    std::vector<uint8_t> m_buffer_depth;
    std::vector<uint8_t> m_buffer_rgb;
    std::vector<uint16_t> m_gamma;
    bool m_new_rgb_frame;
    bool m_new_depth_frame;
    Mat depthMat; //matriz profundidad
    Mat rgbMat; //matriz rgb
    Mat ownMat;
    Mutex m_rgb_mutex;
    Mutex m_depth_mutex;
};

#endif	/* MYKINECT_HPP */

