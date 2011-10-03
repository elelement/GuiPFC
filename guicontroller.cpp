#include "guicontroller.hpp"

GUIController::GUIController()
{
    MyFreenectDevice& dev = freenect.createDevice<MyFreenectDevice > (0);
    device = &dev;
    GUIController::depthMat = new Mat(Size(640, 480), CV_16UC1);
    GUIController::depthf = new Mat(Size(640, 480), CV_8UC1);
    rgbMat = new Mat(Size(640, 480), CV_8UC3);
    img_bw = new Mat(Size(640, 480), CV_8UC1);
    dst = new Mat(Size(640, 480), CV_8UC3);
    cDistance = new Point();
    ht = new HandDetection();
    btc1 = new BTComm();
}

GUIController::~GUIController(){
    delete depthMat;
    delete depthf;
    delete rgbMat;
    delete img_bw;
    delete dst;
    delete btc1;
    delete ht;
    //delete &device; //solo se puede hacer un delete sobre objetos creados dinamicamente con new
    //delete &freenect;
}

Mat& GUIController::getDepthMat(){
    return (*depthMat);
}

Mat& GUIController::getDepthF(){
    return (*depthf);
}

Mat& GUIController::getRGBMat(){
    return (*rgbMat);
}

Mat& GUIController::getDst(){
    return (*dst);
}

void GUIController::startDevice(){
    device->startVideo();//Inicializamos el video y el sensor de IR
    device->startDepth();
}

void GUIController::moveUp()
{
    device->setTiltDegrees(device->m_degrees+1.0);
    device->m_degrees = device->m_degrees+1.0;
}

void GUIController::moveDown()
{
    device->setTiltDegrees(device->m_degrees-1.0);
    device->m_degrees = device->m_degrees-1.0;
}

int GUIController::connect()
{
    char *dest;
    dest = (char *) malloc(sizeof (char) * 18);
    bzero(dest, 18);
    strncpy(dest, "00:16:53:0F:1A:2A", 18);// La MAC del dispositivo bluetooth del brick de lego
    return btc1->init_bluetooth(dest);
}

/*Hacer que devuelva true, en funcion de si esta a la distancia correcta*/
void GUIController::calibrar(){
    device->getVideo(*rgbMat);
    device->getDepth(*depthMat);
    flip(*rgbMat, *rgbMat, 1);
    Mat dmclone = (depthMat->clone());
    dmclone.convertTo(*depthf, CV_8UC1, 255.0 / 2048.0);
//    Utils::normalizeRawData(dmclone, *depthf);

    Point closestPoint = Utils::getClosestPoint(dmclone);//guardar como var. global
    cDistance->x = closestPoint.x;
    cDistance->y = closestPoint.y;

    umbral = depthf->at <uint8_t> (closestPoint.x, closestPoint.y);
}

bool GUIController::process(){
    device->getVideo(*rgbMat);
    device->getDepth(*depthMat);

    /* Obtenemos la imagen de profundidad, frame a frame
     * Rango de alcance del Kinect basarlo en una constante q se defina al calibrar
     * float escala = ((float) umbral)*255.0/(102.0*2048.0);
     */
    Mat dmclone = (depthMat->clone());
    dmclone.convertTo(*depthf, CV_8UC1, 255.0 / 2048.0);

    /* La rotamos para ponerla en modo "espejo". De otra forma el control
     * se vuelve antiintuivo e incomodo.
     */
    flip(*rgbMat, *rgbMat, 1);
    flip(*depthf, *depthf, 1);
    Utils::mythreshold(*depthf, *img_bw, umbral, 255); //150

    /* Opening: consiste en realizar primero un "erode" y postierormente un "dilate". */
    erode(*img_bw,*img_bw,Mat(),Point(-1,-1),5);
    dilate(*img_bw,*img_bw,Mat(),Point(-1,-1),5);

    cv::cvtColor(*depthf, *dst, CV_GRAY2BGR); //Conversión de gris a color para poder pintar otros elementos a color.

    ht->detectHands(*img_bw, *dst);
    bool ok = true;

    return ok;
}

HandDetection& GUIController::getHDObject(){
    return *ht;
}

