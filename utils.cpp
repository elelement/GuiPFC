#include "utils.hpp"

void Utils::drawCross(Mat &img, Point center, Scalar color, int d){
    line(img, Point(center.x - d, center.y - d),
         Point(center.x + d, center.y + d), color, 2, CV_AA, 0);
    line(img, Point(center.x + d, center.y - d),
         Point(center.x - d, center.y + d), color, 2, CV_AA, 0);
}


float Utils::getAngle(Point& p1, Point& p2, Point& reference) {
    float v1, v2, w1, w2;//coseno y seno

    v1 = p1.x - reference.x;
    v2 = p1.y - reference.y;
    w1 = p2.x - reference.x;
    w2 = p2.y - reference.y;

    float m1 = sqrt(pow(v1, 2) + pow(v2, 2));
    float m2 = sqrt(pow(w1, 2) + pow(w2, 2));
    float p = v1*v2 + w1*w2;

    return acos(p / (m1 * m2));
}

float Utils::getAngleOX(Point& p1, Point& p2) {
    float a, b;//coseno y seno

    a = p2.x - p1.x;
    b = p2.y - p1.y;

    if(a != 0){
       return (float)(std::atan(b/a));//hipotenusa y anulo
    }
    return 0;
}

/*The angle it is in radians*/
Point Utils::getCirclePoint(Point &center, double radius, double angle){
    double x = radius * cos(angle) + center.x;
    double y = radius * sin(angle) + center.y;

    return Point(x, y);
}

void Utils::mythreshold(Mat& gray, Mat& bw, uint8_t th, int max){
    for(int i=0; i<gray.rows; i++){
        for(int j=0; j<gray.cols; j++){
            if(gray.at <uint8_t> (i, j) < th){
                bw.at <uint8_t> (i, j) = (uint8_t)0;
            }else{
                bw.at <uint8_t> (i, j) = max;
                gray.at <uint8_t> (i, j) = max;
            }
        }
    }
}

void Utils::normalizeRawData(Mat &rawMat, Mat &depth){
    Mutex mymutex;
    float m_min = 10000.0, m_max = 0.0;
    for(int i=0; i<rawMat.rows; i++){
        for(int j=0; j<rawMat.cols; j++){
            float depthValue = (float)rawMat.at <uint16_t> (i, j);
            float dist = (float)((tan((depthValue/1024.0)+0.5)) * 33.825 + 5.7);
            if (dist < m_min){m_min = dist;}
            if (dist > m_max){m_max = dist;}
        }
    }
    for(int i=0; i<rawMat.rows; i++){
        for(int j=0; j<rawMat.cols; j++){
            float depthValue = (float)rawMat.at <uint16_t> (i,j);
            float dist = ((tan((depthValue/1024.0)+0.5)) * 33.825 + 5.7);
            if(dist <= m_min){
                mymutex.lock();
                depth.at <uint8_t> (i, j) = (uint8_t)255;//Shadow needs to be white for thresholding
                mymutex.unlock();
            }else{
                float f1 = (float)dist;
                float f2 = (float) (m_max-m_min);
                mymutex.lock();
                depth.at <uint8_t> (i, j) = (uint8_t)(f1 * 255.0/f2);
                mymutex.unlock();
            }
        }
    }
}

double Utils::euclideanDistance(Point& p1, Point& ref){
    return sqrt( pow( (ref.x - p1.x), 2) + pow( (ref.y - p1.y), 2) );
}

int Utils::OXDistance(Point& p1, Point& ref){
    return abs(p1.x - ref.x);
}

/*
 * DIST_CM=TAN(DEPTH_VAL/1024+0,5)*33.825+5.7
 * Empirical results.
 */
float Utils::getDistanceFromSource(Mat& depthMat, Point& p){
    float depthValue = (float)depthMat.at <uint16_t> (p.x,p.y);
    return (tan((depthValue/1024.0)+0.5)) * 33.825 + 5.7;
}

Point Utils::getClosestPoint(Mat& depthImg){
    // La profundidad minima es el valor mas cercano
    uint16_t minL = MAX_DEPTH_16B;
    int minX = 0;
    int minY = 0;
    for (int i = 0; i < depthImg.rows; i++) {
        for (int j = 0; j < depthImg.cols; j++) {
            if(depthImg.at <uint16_t> (i,j) <= minL){
                minL = depthImg.at <uint16_t> (i,j);
                minX = i;
                minY = j;
            }
        }
    }
    return Point(minX, minY);
}

Point Utils::getRectCenter(Rect const& r){
    return Point(r.x + r.width / 2, r.y + r.height / 2);
}

void Utils::opening(Mat& hand, Mat& dst){
    Mat eroded = dst.clone();
    cv::dilate(hand, dst, Mat());
    cv::erode(dst,eroded,Mat());
}


/* Comparamos solo en el eje horizontal, interesa saber si esta a la izquierda
 * o a la derecha solamente.
 */
int Utils::compareToXAxis(Point& p1, Point& p2){
    int resultado = 0;
    if(p1.x > p2.x){
        resultado = 1;//p1 esta a la derecha de p2
    }else if(p1.x < p2.x){
        resultado = -1;//p1 esta a la izquierda de p2
    }

    return resultado;
}

int Utils::compareToYAxis(Point& p1, Point& p2){
    int resultado = 0;
    if(p1.y > p2.y){
        resultado = 1;//p1 esta a la derecha de p2
    }else if(p1.y < p2.y){
        resultado = -1;//p1 esta a la izquierda de p2
    }

    return resultado;
}

void Utils::drawConvexHull(Mat &matrix, vector<Point>& contour, vector<int>& cvxHull){
    int hullcount = (int) cvxHull.size();
    Point pt0 = contour[cvxHull[hullcount-1]];
    for(int i = 0; i < hullcount; i++ ) {
        Point pt = contour[cvxHull[i]];
        line(matrix, pt0, pt, Scalar(0, 255, 0), 1, CV_AA);
        pt0 = pt;
    }
}

Mat Utils::rotateImage(const Mat& source, double angle)
{
    Point2f src_center(source.cols/2.0F, source.rows/2.0F);
    Mat rot_mat = getRotationMatrix2D(src_center, angle, 1.0);
    Mat dst;
    warpAffine(source, dst, rot_mat, source.size());
    return dst;
}

