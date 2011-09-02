#ifndef GUICONTROLLER_HPP
#define GUICONTROLLER_HPP

#include <QMainWindow>
#include <QTimer>
#include "libfreenect.hpp"
#include "btcomm.hpp"
#include "mykinect.hpp"
#include "handdetection.hpp"

class GUIController
{
public:
    Freenect::Freenect freenect;
    MyFreenectDevice* device;
    Mat* depthMat;//Matriz con datos "puros (raw)" de profundidad (valores uint16_t entre 0 y 2048)
    Mat* depthf; //Matriz filtrada convertida a un formato de 8 bits, con la consecuente perdida de precision.
    Mat* rgbMat; //Imagen de la camara. Solo se muestra como "decoracion" realmente.
    Mat* img_bw;//Imagen en blanco y negro con el límite puesto en función de la profundidad
    Mat* dst; //Aqui es donde vamos a pintar los resultados.
    Mutex m_depth_mutex;//por si hiciera falta xD
    BTComm* btc1;
    Point* cDistance;

    GUIController();
    ~GUIController();//hacer un delete de todos los punteros
    Mat& getDepthMat();
    Mat& getDepthF();
    Mat& getRGBMat();
    Mat& getImgBW();
    Mat& getDst();
    bool process();
    void moveUp();
    void moveDown();
    void startDevice();
    void calibrar();
    QString connect();
    HandDetection& getHDObject();

    static void mapDepth2Color(Mat& depth, Mat& color);

private:
    HandDetection* ht;
    uint8_t umbral;
    /*RGB& GetRGB(cv::Mat &mat, cv::Point p);*/

};

#endif // GUICONTROLLER_HPP
