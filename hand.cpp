#include "hand.hpp"

/*
 * Representa cada punto de la mano
 * En HandDetection poner un vector <Hand >
 */
Hand::Hand()
    :TrackObject::TrackObject(){
    _fingers = 0;
    _contour = new vector<Point>();
    _depth = 0;
    _center = new Point(ROWS/2, COLS/2);
//    randu(*_state, Scalar(0), Scalar(0.1));
//    randu(_kf.statePost, Scalar(0), Scalar(0.1));
}

Hand::Hand(const Hand& myhand)
    :TrackObject::TrackObject() {
    _contour = new vector<Point>();
    vector<Point>* copy = myhand.getContour();
    for(size_t i=0; i<copy->size(); i++){
        Point p = copy->at(i);
        _contour->push_back(p);
    }
    _fingers = myhand.getFingers();
    _depth = myhand.getDepth();
    Point p = myhand.getCenter();
    _center->x = p.x;
    _center->y = p.y;

//    randu(*_state, Scalar(_center->x), Scalar(_center->y));
//    randu(_kf.statePost, Scalar(_center->x), Scalar(_center->y));
}

Hand::~Hand(){
    delete(_center);
    delete(_contour);
}

int Hand::getFingers() const{
    return _fingers;
}

Point& Hand::getCenter() const{
    return *_center;
}

vector<Point>* Hand::getContour() const{
    return _contour;
}

uint8_t Hand::getDepth() const{
    return _depth;
}

int Hand::getWhich() const{
    return _which;
}

void Hand::setFingers(int fingers){
    _fingers = fingers;
}

void Hand::setCenter(Point& center){
    _center->x = center.x;
    _center->y = center.y;
}

void Hand::setContour(vector<Point>& contour){
    if(contour.size() > 0){
        for(size_t i=0; i<contour.size(); i++){
            _contour->push_back(contour.at(i));
        }
    }
}

void Hand::setDepth(uint8_t depth){
    _depth = depth;
}

void Hand::setWhich(int which){
    _which = which;
}

double Hand::getArea() const{
    if(_contour->size() > 0)
        return contourArea(Mat(*_contour));
    return 0;
}

void Hand::initStates(const int dymParams, const int measureParams, const int controlParams, const Mat& T){
    //1st param = dimension of the state vector (position + velocity, considering a=0)
    //2nd param = dimension of the measurement vector -> the real position (a point)
        _kf = KalmanFilter(6, 2, controlParams);
        _state = Mat_<float>(6, 1, CV_32F);
        _processNoise = Mat(6, 1, CV_32F);
        _measurement = Mat_<float>(2, 1, CV_32F);
        _kf.statePre.at<float>(0,0) = _center->x;
        _kf.statePre.at<float>(1,0) = _center->y;
        _kf.statePre.at<float>(2,0) = 0;
        _kf.statePre.at<float>(3,0) = 0;
        _kf.statePre.at<float>(4,0) = 0;
        _kf.statePre.at<float>(5,0) = 0;
        _kf.transitionMatrix = T;
        _kf.measurementMatrix = *(Mat_<float>(2, 6) << 1,0,1,0,0.5,0, 0,1,0,1,0,0.5);
        setIdentity(_kf.processNoiseCov, Scalar::all(1e-4));
        setIdentity(_kf.measurementNoiseCov, Scalar::all(1e-1));
        setIdentity(_kf.errorCovPost, Scalar::all(.1));

}

void Hand::trackObject(Mat& img){
        Mat prediction = _kf.predict();
        Point predictPt(prediction.at<float>(0,0),prediction.at<float>(1,0));

        // Get hand center
        _measurement(0,0) = _center->x;
        _measurement(1,0) = _center->y;

        Point measPt(_measurement(0,0),_measurement(1,0));
        // generate measurement
        //_measurement += _kf.measurementMatrix * (_state);

        // The "correct" phase that is going to use the predicted value and our measurement
        Mat estimated = _kf.correct(_measurement);//statePost es el corregido
        Point estimatedPt(estimated.at<float>(0,0),estimated.at<float>(1,0));

        // plot points
        if(_which == RIGHT_HAND){
            Utils::drawCross(img, predictPt, Scalar(255,0,0), 3 );//El predicho
            Utils::drawCross(img, measPt, Scalar(0,0,0), 3 );//El observado
            Utils::drawCross(img, estimatedPt, Scalar(0,255,0), 3 );//El estimado
            line(img, *_center, measPt, Scalar(0,0,255), 3, CV_AA, 0 );
            line(img, *_center, predictPt, Scalar(0,255,255), 3, CV_AA, 0 );
        }else{
            Utils::drawCross(img, predictPt, Scalar(128,204,133), 3 );//El predicho
            Utils::drawCross(img, measPt, Scalar(255,0,255), 3 );//El observado
            Utils::drawCross(img, estimatedPt, Scalar(255,255,0), 3 );//El estimado
            line(img, *_center, measPt, Scalar(0,0,128), 3, CV_AA, 0 );
            line(img, *_center, predictPt, Scalar(0,128,0), 3, CV_AA, 0 );
        }
        randn(_processNoise, Scalar(0), Scalar::all(sqrt(_kf.processNoiseCov.at<float>(0, 0))));
        _state = _kf.transitionMatrix * (_state) + _processNoise;
//    }

}

const Mat Hand::getPredicted() {
    return _kf.statePre;
}

const Mat Hand::getMeasured() {
    return _kf.measurementMatrix;
}

