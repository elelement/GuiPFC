#ifndef STEERINGWHEEL_HPP
#define STEERINGWHEEL_HPP

#include <opencv/cv.h>
#include <opencv/cv.hpp>

#include "guiappheaders.hpp"
#include "utils.hpp"

using namespace std;
using namespace cv;

class SteeringWheel
{

public:
    SteeringWheel();
    SteeringWheel(Point const& center, double const& radius, double const& alpha, int const& arms);
    ~SteeringWheel();
    void draw(Mat& dst, Point& hand);

    Point* const& getCenter();
    Point* const& getLast();
    double const& getRadius();
    double const& getAlpha();
    int const& getArms();

    void setCenter(Point const& center);
    void setRadius(double const& radius);
    void setAlpha(double const& alpha);
    void setArms(int const& arms);

private:
    Point* _center;
    Point* _last;
    double _radius;
    double _alpha;
    int _arms;
};

#endif // STEERINGWHEEL_HPP
