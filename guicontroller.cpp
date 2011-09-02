#include "guicontroller.hpp"
#include "utils.hpp"

//MyFreenectDevice& device = freenect.createDevice<MyFreenectDevice > (0);

GUIController::GUIController()
{
    /*Freenect::Freenect freenect;*/
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
    //umbral = 100;//valor temporal. a la espera de tomar el dato de la imagen
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
    //Inicializamos el video y el sensor de IR
    device->startVideo();
    device->startDepth();
}

void GUIController::moveUp()
{
    device->setTiltDegrees(device->degrees+1.0);
    device->degrees = device->degrees+1.0;
}

void GUIController::moveDown()
{
    device->setTiltDegrees(device->degrees-1.0);
    device->degrees = device->degrees-1.0;
}

QString GUIController::connect()
{
    char *dest;
    dest = (char *) malloc(sizeof (char) * 18);
    bzero(dest, 18);
    // La MAC del dispositivo bluetooth del brick de lego
    strncpy(dest, "00:16:53:0F:1A:2A", 18);
    if(btc1->init_bluetooth(dest) < 0){
        btc1->close_connection();//close(btc1.sock);
        //imprimir mensaje de error en el label
        QString cadena("Error: ha sido imposible entablar una conexión.");
        //limpiar variables para volver a intentarlo
        return cadena;
        //ui->statusBar->showMessage(cadena);
    } else {
        //printf("Bluetooth conectado a %s \n", dest);
        //ui->mensajes->setText("Bluetooth conectado a %s \n", dest);
        QString cadena("Bluetooth conectado a ");
        cadena.append(dest);
        return cadena;
        //ui->statusBar->showMessage(cadena);
        //ui->calibrar->setEnabled(true);
    }

    //Llamar a la parte del análisis de imagen y mostrar vídeo en pantalla
}

/*Hacer que devuelva true, en funcion de si esta a la distancia correcta*/
void GUIController::calibrar(){
    device->getVideo(*rgbMat);
    device->getDepth(*depthMat);
    flip(*rgbMat, *rgbMat, 1);
    Mat dmclone = (depthMat->clone());
//    dmclone.convertTo(*depthf, CV_8UC1, 255.0 / 2048.0);
    Utils::normalizeRawData(dmclone, *depthf);

    Point closestPoint = Utils::getClosestPoint(dmclone);//guardar como var. global
    cDistance->x = closestPoint.x;
    cDistance->y = closestPoint.y;

    //Revisar
    umbral = depthf->at <uint8_t> (closestPoint.x, closestPoint.y);
}

bool GUIController::process(){
    device->getVideo(*rgbMat);
    device->getDepth(*depthMat);

    //Obtenemos la imagen de profundidad, frame a frame
    //Rango de alcance del Kinect basarlo en una constante q se defina al calibrar
    //float escala = ((float) umbral)*255.0/(102.0*2048.0);
    Mat dmclone = (depthMat->clone());
    Utils::normalizeRawData(dmclone, *depthf);
    //Opening
//    namedWindow("hola", CV_WINDOW_AUTOSIZE);

    //La rotamos para ponerla en modo "espejo". De otra forma el control se vuelve
    //antiintuivo e incomodo.
    flip(*rgbMat, *rgbMat, 1);
    flip(*depthf, *depthf, 1);
    Utils::mythreshold(*depthf, *img_bw, umbral, 255); //150

//    threshold(*depthf, *img_bw, umbral, 255, CV_THRESH_BINARY);
//    threshold(*depthf, *depthf, umbral, 255, CV_THRESH_BINARY);//CV_THRESH_TRUN
    erode(*img_bw,*img_bw,Mat(),Point(-1,-1),5);//esto esta bien
    dilate(*img_bw,*img_bw,Mat(),Point(-1,-1),5);

    cv::cvtColor(*depthf, *dst, CV_GRAY2BGR);
//    depthf->convertTo(*dst, CV_8UC3);
//    depthf->copyTo(*dst);
    ht->detectHands(*img_bw, *dst);
    bool ok = true;

    return ok;
}

HandDetection& GUIController::getHDObject(){
    return *ht;
}

//RGB& GetRGB(cv::Mat &mat, cv::Point p)
//{
//  assert((mat.step/mat.cols) == sizeof(RGB));
//  RGB *data = (RGB*)mat.data;
//  data += p.y * mat.cols + p.x;
//  return *data;
//}
