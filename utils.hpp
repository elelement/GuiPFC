#ifndef UTILS_HPP
#define UTILS_HPP
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/cv.hpp>
#include <opencv/cxcore.h>
#include <opencv/cxcore.hpp>
#include <opencv/highgui.h>
#include <opencv/highgui.hpp>

#include "guiappheaders.hpp"
#include "mykinect.hpp"
using namespace cv ;

class Utils
{
public:
    Utils();
    ~Utils();
    //static void setDepthColor(uint8_t* ptr, int d);
    static void normalizeRawData(Mat& rawMat, Mat& depth);
    static void mythreshold(Mat& gray, Mat& bw, uint8_t th, int max);
    static bool compareYAxis(Point& p1, Point& p2);
    static Point getClosestPoint(Mat& depthImg);
    static float getDistanceFromSource(Mat& depthMat, Point& p);
    static Point centroid(vector<Point>& polygon);
    static Point getRectCenter(Rect const& r);
    static void opening(Mat& hand, Mat& dst);
    static int compareToXAxis(Point& p1, Point& p2);
    static int compareToYAxis(Point& p1, Point& p2);
    static void dilateHand(Mat& hand, Mat& dilated);
    static void erodeHand(Mat& hand, Mat& eroded);
    static void paintPoints(vector<Point>& points, Mat& img);
    static bool existsPoint(Point& p, vector<Point>& points);
    static double euclideanDistance(Point& p1, Point& ref);
    static int OXDistance(Point& p1, Point& ref);
    static void clearMatrix(Mat& matrix);
    static void drawConvexHull(Mat& matrix, vector<Point>& contour, vector<int>& cvxHull);
    static Point getCirclePoint(Point& center, double radius, double angle);
    static void setRect(Rect& r1, Rect const & r2);
    static float getAngleOX(Point& p1, Point& p2);
    static Point calcPoint(Point2f& center, double r, double angle);
    static void drawCross(Mat& img, Point center, Scalar color, int d);
    static Mat rotateImage(const Mat& source, double angle);
    static void applyColor(const Mat& gray, Mat& dst, Scalar& color);

//    static Mat invertMatrix(Mat& src);

 private:
};

#endif // UTILS_HPP
