#include "steeringwheel.hpp"

SteeringWheel::SteeringWheel()
{
    _center = new Point(ROWS, COLS/3);
    _last = new Point(ROWS/2, COLS/2);
    _radius = (ROWS*0.5/2) / 2;
    _alpha = 0.0;
    _arms = 3;
}

SteeringWheel::SteeringWheel(const Point &center, const double &radius, const double &alpha, const int &arms){
    _center->x = center.x;
    _center->y = center.y;
    _radius = radius;
    _alpha = alpha;
    _arms = arms;
}

SteeringWheel::~SteeringWheel(){
}

double const& SteeringWheel::getAlpha(){
    return _alpha;
}

double const& SteeringWheel::getRadius(){
    return _radius;
}

Point* const& SteeringWheel::getCenter(){
    return _center;
}

Point* const& SteeringWheel::getLast(){
    return _last;
}

int const& SteeringWheel::getArms(){
    return _arms;
}

void SteeringWheel::setAlpha(double const& alpha){
    _alpha = alpha;
}

void SteeringWheel::setRadius(double const& radius){
    _radius = radius;
}

void SteeringWheel::setCenter(const Point &center){
    _center->x = center.x;
    _center->y = center.y;
}

void SteeringWheel::setArms(const int &arms){
    _arms = arms;
}

void SteeringWheel::draw(Mat &dst, Point &hand){
    double r1 = _radius;
    double r2 = r1*2/2.5;
    double r3 = r1*2/6;
    double alpha = _alpha;
    double beta = 0.0;
    double gamma = 0.0;

    //Pintamos dos circulos concentricos
    circle(dst, *_center, r1, Scalar(59,58,54), 10);
    circle(dst, *_center, r2, Scalar(200,200,200), 5);

    //Calculo de las ecuaciones de las rectas que cortan al volante
    //x = cos(angle) * radius + center.x <--> y = sin(angle) * radius + center.y
    if(hand.x == 0 && hand.y == 0){
        alpha = (double)Utils::getAngleOX(*_center, *_last);
    }else{
        alpha = (double)Utils::getAngleOX(*_center, hand);
    }
    beta = (2.0*M_PI/3.0) + alpha;
    gamma = (2.0*M_PI/3.0) + beta;

    //Recta que pasa por 'center' con un angulo de (alpha, beta o gamma)
    //Ver donde corta cada recta a r2. Resolvemos el sistema de ecuaciones
    Point p1 = Utils::getCirclePoint(*_center, r2, alpha);
    Point p2 = Utils::getCirclePoint(*_center, r2, beta);
    Point p3 = Utils::getCirclePoint(*_center, r2, gamma);

    line(dst, *_center, p1, Scalar(100,100,100), 3);
    line(dst, *_center, p2, Scalar(100,100,100), 3);
    line(dst, *_center, p3, Scalar(100,100,100), 3);
    //Los radios del volante, van por detras de la zona central
    circle(dst, *_center, r3, Scalar(58,58,54), -1);
    if(hand.x == 0 && hand.y == 0){
        line(dst, *_center, *_last, Scalar(0,0,0), 1);
    }else{
        line(dst, *_center, hand, Scalar(0,0,0), 1);
    }

    _alpha = alpha;
    if(hand.x != 0 && hand.y != 0){
        _last->x = hand.x;
        _last->y = hand.y;
    }


}
