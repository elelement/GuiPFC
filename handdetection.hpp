/* 
 * File:   handdetection.hpp
 * Author: cabreracanal
 *
 * Created on 7 de mayo de 2011, 12:51
 */

#ifndef HANDDETECTION_HPP
#define	HANDDETECTION_HPP

#include <iostream>
#include <vector>
#include <stdio.h>
#include <cmath>
#include <exception>
#include <pthread.h>

#include <opencv/cv.h>
#include <opencv/cv.hpp>

#include "guiappheaders.hpp"
#include "utils.hpp"
#include "hand.hpp"
#include "trackobject.hpp"

using namespace std;
using namespace cv;

/*
 * A partir de un conjunto de contornos (cada contorno es un conjunto de puntos),
 * se determinará qué conjunto corresponde a la mano izquierda y cuál a la mano
 * derecha. Así pues, en función de la posición relativa de ambas manos se
 * determinará una acción a realizar, representada por un valor numérico.
 */
class HandDetection {

private:
    Hand* _leftHand;
    Hand* _rightHand;
    TrackObject* _to;
    bool _noTemplate;
    int _stopLimit;
    void findConvexityDefects(vector<Point>& contour, vector<int>& hull, vector<CvConvexityDefect>& defects);
    void findFingerTips(vector<Point>& cvxHull, vector<CvConvexityDefect>& defects, vector<CvConvexityDefect>& fingerTips);

public:
    HandDetection();
    ~HandDetection();
    Hand* getLeftHand() const;
    Hand* getRightHand() const;
    Hand* getHand(int which) const;

    void setHand(Hand& hand, int& which);

    /*
     * Devuelve el ángulo entre la mano izquierda y derecha
     */
    float getHandsAngle(Point& p1, Point& p2);

    /*
     * Encuentra ambas manos y asigna valor a leftHand y rightHand.
     */
    void detectHands(Mat& depthImage, Mat& color);

    int getTurnAngle();//float alfa
    float getTurnSpeed(Point& pOld, Point& pNew);
    int getStopLimit();
    void setStopLimit(int limit);
    int getFingersCount(int hand) const;

    /*Metodos para definir la accion a realizar*/

};

#endif	/* HANDDETECTION_HPP */

