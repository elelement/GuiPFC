#include "hand.hpp"

/*
 * Representa cada punto de la mano
 * En HandDetection poner un vector <Hand >
 */
Hand::Hand()
    :TrackObject::TrackObject(){
    m_fingers = 0;
    m_contour = new vector<Point>();
    m_depth = 0;
    m_center = new Point(ROWS/2, COLS/2);
//    randu(*m_state, Scalar(0), Scalar(0.1));
//    randu(m_kf.statePost, Scalar(0), Scalar(0.1));
}

Hand::Hand(const Hand& myhand)
    :TrackObject::TrackObject() {
    m_contour = new vector<Point>();
    vector<Point>* copy = myhand.getContour();
    for(size_t i=0; i<copy->size(); i++){
        Point p = copy->at(i);
        m_contour->push_back(p);
    }
    m_fingers = myhand.getFingers();
    m_depth = myhand.getDepth();
    Point p = myhand.getCenter();
    m_center->x = p.x;
    m_center->y = p.y;
    m_estimated = Point(p.x, p.y);

//    randu(*m_state, Scalar(_center->x), Scalar(_center->y));
//    randu(m_kf.statePost, Scalar(_center->x), Scalar(_center->y));
}

Hand::~Hand(){
    delete(m_center);
    delete(m_contour);
}

int Hand::getFingers() const{
    return m_fingers;
}

Point& Hand::getCenter() const{
    return *m_center;
}

vector<Point>* Hand::getContour() const{
    return m_contour;
}

uint8_t Hand::getDepth() const{
    return m_depth;
}

int Hand::getWhich() const{
    return m_which;
}

Point& Hand::getEstimatedPoint() {
    return m_estimated;
}

void Hand::setFingers(int fingers){
    m_fingers = fingers;
}

void Hand::setCenter(Point& center){
    m_center->x = center.x;
    m_center->y = center.y;
}

void Hand::setContour(vector<Point>& contour){
    if(contour.size() > 0){
        for(size_t i=0; i<contour.size(); i++){
            m_contour->push_back(contour.at(i));
        }
    }
}

void Hand::setDepth(uint8_t depth){
    m_depth = depth;
}

void Hand::setWhich(int which){
    m_which = which;
}

double Hand::getArea() const{
    if(m_contour->size() > 0)
        return contourArea(Mat(*m_contour));
    return 0;
}

void Hand::initStates(const int dymParams, const int measureParams, const int controlParams, const Mat& T){
    //1st param = dimension of the state vector (position + velocity, considering a=0)
    //2nd param = dimension of the measurement vector -> the real position (a point)
        m_kf = KalmanFilter(dymParams, 2, controlParams);
        m_state = Mat_<float>(dymParams, measureParams, CV_32F);
        m_process_noise = Mat(dymParams, measureParams, CV_32F);
        m_measurement = Mat_<float>(2, 1, CV_32F);
        m_kf.statePre.at<float>(0,0) = m_center->x;
        m_kf.statePre.at<float>(1,0) = m_center->y;
        m_kf.statePre.at<float>(2,0) = 0;
        m_kf.statePre.at<float>(3,0) = 0;
        m_kf.statePre.at<float>(4,0) = 0;
        m_kf.statePre.at<float>(5,0) = 0;
        m_kf.transitionMatrix = T;
        m_kf.measurementMatrix = *(Mat_<float>(2, 6) << 1,0,1,0,0.5,0, 0,1,0,1,0,0.5);
        setIdentity(m_kf.processNoiseCov, Scalar::all(1e-4));
        setIdentity(m_kf.measurementNoiseCov, Scalar::all(1e-1));
        setIdentity(m_kf.errorCovPost, Scalar::all(.1));

}

void Hand::trackObject(Mat& img){
        Mat prediction = m_kf.predict();
        Point predictPt(prediction.at<float>(0,0),prediction.at<float>(1,0));

        // Get hand center
        m_measurement(0,0) = m_center->x;
        m_measurement(1,0) = m_center->y;

        Point measPt(m_measurement(0,0),m_measurement(1,0));
        // generate measurement
        //m_measurement += m_kf.measurementMatrix * (m_state);

        // The "correct" phase that is going to use the predicted value and our measurement
        Mat estimated = m_kf.correct(m_measurement);//statePost es el corregido
        Point estimatedPt(estimated.at<float>(0,0),estimated.at<float>(1,0));

        // plot points
        if(m_which == RIGHT_HAND){
            Utils::drawCross(img, predictPt, Scalar(255,0,0), 3 );//El predicho
            Utils::drawCross(img, measPt, Scalar(0,0,0), 3 );//El observado
            Utils::drawCross(img, estimatedPt, Scalar(0,255,0), 3 );//El estimado
            line(img, *m_center, measPt, Scalar(0,0,255), 3, CV_AA, 0 );
            line(img, *m_center, predictPt, Scalar(0,255,255), 3, CV_AA, 0 );
        }else{
            Utils::drawCross(img, predictPt, Scalar(128,204,133), 3 );//El predicho
            Utils::drawCross(img, measPt, Scalar(255,0,255), 3 );//El observado
            Utils::drawCross(img, estimatedPt, Scalar(255,255,0), 3 );//El estimado
            line(img, *m_center, measPt, Scalar(0,0,128), 3, CV_AA, 0 );
            line(img, *m_center, predictPt, Scalar(0,128,0), 3, CV_AA, 0 );
        }
        m_estimated = estimatedPt;
        randn(m_process_noise, Scalar(0), Scalar::all(sqrt(m_kf.processNoiseCov.at<float>(0, 0))));
        m_state = m_kf.transitionMatrix * (m_state) + m_process_noise;
//        m_state = m_kf.transitionMatrix * (m_state) + 0;
//    }

}

const Mat Hand::getPredicted() {
    return m_kf.statePre;
}

const Mat Hand::getMeasured() {
    return m_kf.measurementMatrix;
}

