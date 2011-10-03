#include "steeringwheel.hpp"

SteeringWheel::SteeringWheel()
{
    m_center = new Point(ROWS, COLS/2);
    m_last = new Point(ROWS/2, COLS/2);
    m_radius = (ROWS*0.5/2) / 2;
    m_alpha = 0.0;
    m_arms = 3;
}

SteeringWheel::SteeringWheel(const Point &center, const double &radius, const double &alpha, const int &arms){
    m_center->x = center.x;
    m_center->y = center.y;
    m_radius = radius;
    m_alpha = alpha;
    m_arms = arms;
}

SteeringWheel::~SteeringWheel(){
}

double const& SteeringWheel::getAlpha(){
    return m_alpha;
}

double const& SteeringWheel::getRadius(){
    return m_radius;
}

Point* const& SteeringWheel::getCenter(){
    return m_center;
}

Point* const& SteeringWheel::getLast(){
    return m_last;
}

int const& SteeringWheel::getArms(){
    return m_arms;
}

void SteeringWheel::setAlpha(double const& alpha){
    m_alpha = alpha;
}

void SteeringWheel::setRadius(double const& radius){
    m_radius = radius;
}

void SteeringWheel::setCenter(const Point &center){
    m_center->x = center.x;
    m_center->y = center.y;
}

void SteeringWheel::setArms(const int &arms){
    m_arms = arms;
}

void SteeringWheel::draw(Mat &dst, Point &hand){
    double r1 = m_radius;
    double r2 = r1*2/2.5;
    double r3 = r1*2/6;
    double alpha = m_alpha;
    double beta = 0.0;
    double gamma = 0.0;

    //Pintamos dos circulos concentricos
    circle(dst, *m_center, r1, Scalar(59,58,54), 10);
    circle(dst, *m_center, r2, Scalar(200,200,200), 5);

    //Calculo de las ecuaciones de las rectas que cortan al volante
    //x = cos(angle) * radius + center.x <--> y = sin(angle) * radius + center.y
    if(hand.x == 0 && hand.y == 0){
        alpha = (double)Utils::getAngleOX(*m_center, *m_last);
    }else{
        alpha = (double)Utils::getAngleOX(*m_center, hand);
    }
    beta = (2.0*M_PI/3.0) + alpha;
    gamma = (2.0*M_PI/3.0) + beta;

    //Recta que pasa por 'center' con un angulo de (alpha, beta o gamma)
    //Ver donde corta cada recta a r2. Resolvemos el sistema de ecuaciones
    Point p1 = Utils::getCirclePoint(*m_center, r2, alpha);
    Point p2 = Utils::getCirclePoint(*m_center, r2, beta);
    Point p3 = Utils::getCirclePoint(*m_center, r2, gamma);

    line(dst, *m_center, p1, Scalar(100,100,100), 3);
    line(dst, *m_center, p2, Scalar(100,100,100), 3);
    line(dst, *m_center, p3, Scalar(100,100,100), 3);
    //Los radios del volante, van por detras de la zona central
    circle(dst, *m_center, r3, Scalar(58,58,54), -1);
    if(hand.x == 0 && hand.y == 0){
        line(dst, *m_center, *m_last, Scalar(0,0,0), 1);
    }else{
        line(dst, *m_center, hand, Scalar(0,0,0), 1);
    }

    m_alpha = alpha;
    if(hand.x != 0 && hand.y != 0){
        m_last->x = hand.x;
        m_last->y = hand.y;
    }


}
