#include "utils.hpp"

Utils::Utils()
{
}

Utils::~Utils(){

}

void Utils::clearMatrix(Mat &matrix) {
    //TODO
}

Point Utils::calcPoint(Point2f& center, double r, double angle) {
    return center + Point2f((float)cos(angle), (float)-sin(angle))*(float)r;
}

void Utils::drawCross(Mat &img, Point center, Scalar color, int d){
    line(img, Point(center.x - d, center.y - d),
         Point(center.x + d, center.y + d), color, 2, CV_AA, 0);
    line(img, Point(center.x + d, center.y - d),
         Point(center.x - d, center.y + d), color, 2, CV_AA, 0);
}

void Utils::setRect(Rect &r1, Rect const& r2){
    r1.height = r2.height;
    r1.width = r2.width;
    r1.x = r2.x;
    r1.y = r2.y;
}

float Utils::getAngleOX(Point& p1, Point& p2) {
    float a, b;//coseno y seno
    float alfa;//hipotenusa y anulo

    a = p2.x - p1.x;
    b = p2.y - p1.y;

    if(a != 0){
        alfa = std::atan(b/a);
    }else{
        return 0;
    }
    return alfa;
}

/*The angle it is in radians*/
Point Utils::getCirclePoint(Point &center, double radius, double angle){
    double x = radius * cos(angle) + center.x;
    double y = radius * sin(angle) + center.y;

    return Point(x, y);
}

/*
 * We don't need the static keyword in the definition.
 * The compiler still knows that the variable/method is part of the class and not the instance.
 */

void Utils::mythreshold(Mat& gray, Mat& bw, uint8_t th, int max){
//    uint8_t mmin = MAX_DEPTH;
//    uint8_t mmax = 0;//clustering k-means

    for(int i=0; i<gray.rows; i++){
        for(int j=0; j<gray.cols; j++){
//            if(gray.at <uint8_t> (i, j) < mmin){mmin = gray.at <uint8_t> (i, j) ;}
//            if(gray.at <uint8_t> (i, j) > mmax){mmax = gray.at <uint8_t> (i, j) ;}
            if(gray.at <uint8_t> (i, j) < th){
                bw.at <uint8_t> (i, j) = (uint8_t)0;
            }else{
                bw.at <uint8_t> (i, j) = max;
                gray.at <uint8_t> (i, j) = max;
            }
        }
    }
}

bool Utils::existsPoint(Point &p, vector<Point> &points){
    //Empezamos por la mitad
   int index;
   index = (int)points.size();
   for(int i=0; i<index; i++){
       if(p == points.at(i)){
           return true;
       }
   }
   return false;
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

/**
  * Tambien llamado centro de gravedad
  */
Point Utils::centroid(vector<Point>& polygon){
    //Para n particulas o puntos que forman la imagen...
    //B[i,j] = 1 si (i,j) en la region; 0 en otro caso.
    int xPos = 0, yPos = 0;
    int area = 0, sumX = 0, sumY = 0;

    for(uint i=0; i<polygon.size()-1; i++){
        sumX = sumX + ( (polygon[i].y - polygon[i+1].y) * ( (polygon[i].x * polygon[i].x) + (polygon[i].x * polygon[i+1].x) + (polygon[i+1].x * polygon[i].x) ) );
            sumY = sumY + ( (polygon[i+1].x - polygon[i].x) * ( (polygon[i].y * polygon[i].y) + (polygon[i].y * polygon[i+1].y) + (polygon[i+1].y * polygon[i].y) ) );
            area = area + ((polygon[i].x * polygon[i+1].y) - (polygon[i+1].x * polygon[i].y));
    }

    //cerramos el poligono con el punto inicial
    area = abs(6 * area / 2);
    xPos = abs(sumX / area);
    yPos = abs(sumY / area);
    return Point(xPos, yPos);
}

Point Utils::getRectCenter(Rect const& r){
    return Point(r.x + r.width / 2, r.y + r.height / 2);
}

void Utils::opening(Mat& hand, Mat& dst){
    Mat eroded = dst.clone();
    cv::dilate(hand, dst, Mat());
    cv::erode(dst,eroded,Mat());
}
void Utils::dilateHand(Mat& hand, Mat& dilated){
    cv::dilate(hand, dilated, Mat());
}

void Utils::erodeHand(Mat& hand, Mat& eroded){
    cv::erode(hand,eroded,Mat(Size(30,30), CV_8UC1));
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

void Utils::applyColor(const Mat &gray, Mat &dst, Scalar &color){
    for(int i=0; i<(int)gray.rows; i++){
        for(int j=0; j<(int)gray.rows; j++){
            uint8_t ngris = gray.at<uint8_t>(i,j);
            dst.at<uint8_t>(i,j) = ngris&0xFF;
        }
    }


}

//RGB& Utils::GetRGB(cv::Mat &mat, cv::Point p)
//{
//  assert((mat.step/mat.cols) == sizeof(RGB));
//  RGB *data = (RGB*)mat.data;
//  data += p.y * mat.cols + p.x;
//  return *data;
//}

//cv::invert(const Mat &a, Mat &c, int flags)
//Mat& Utils::invertMatrix(Mat const& src) const{
//    Mat dst(ROWS, COLS);
//    for(int i=0; i<src.rows; i++){
//        for(int j=0; j<src.rows; j++){
//            dst.at(ROWS - i, j) = src.at(i, j);
//        }
//    }
//}

//void Utils::setDepthColor(uint8_t *cptr, int d)
//{
//    int pval = t_gamma[d];
//    int lb = pval & 0xff;
//    switch (pval >> 8)
//    {
//    case 0:
//        cptr[2] = 255;
//        cptr[1] = 255 - lb;
//        cptr[0] = 255 - lb;
//        break;
//    case 1:
//        cptr[2] = 255;
//        cptr[1] = lb;
//        cptr[0] = 0;
//        break;
//    case 2:
//        cptr[2] = 255 - lb;
//        cptr[1] = 255;
//        cptr[0] = 0;
//        break;
//    case 3:
//        cptr[2] = 0;
//        cptr[1] = 255;
//        cptr[0] = lb;
//        break;
//    case 4:
//        cptr[2] = 0;
//        cptr[1] = 255 - lb;
//        cptr[0] = 255;
//        break;
//    case 5:
//        cptr[2] = 0;
//        cptr[1] = 0;
//        cptr[0] = 255 - lb;
//        break;
//    default:
//        cptr[2] = 0;
//        cptr[1] = 0;
//        cptr[0] = 0;
//        break;
//    }
//}
