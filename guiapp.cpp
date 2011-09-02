#include "guiapp.hpp"
#include "ui_guiapp.h"
#include "guiappheaders.hpp"
#include <stdint.h>
#include <QGraphicsPixmapItem>


GuiApp::GuiApp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GuiApp)
{
    ui->setupUi(this);

    _glWidget = new GLWidget;

    gc = new GUIController();
    _wheel = new SteeringWheel();
    initiated = false;
    operation_mode = CALIBRATION;

    gc->startDevice();

    calibrating = false;
    ui->statusBar->setStyleSheet("color: rgb(215, 71, 114);");
    QString cadena("Bienvenido");
    ui->statusBar->showMessage(cadena);
    timer = new QTimer(this);
    timer->setInterval(REFRESH_TIMER);//aprox 24 fps
    ui->lcdNumber->display(0);
    timer->start();

//    QObject::connect(ui->conectar, SIGNAL(clicked()), this, SLOT(connect2lego()));
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));
    //QTimer::singleShot(200,this,SLOT(refresh()));
    //QObject::connect(ui->startButton, SIGNAL(clicked()), this, SLOT(process()));
    QObject::connect(ui->calibrar, SIGNAL(clicked()), this, SLOT(switch2WorkMode()));
    QObject::connect(ui->up, SIGNAL(clicked()), this, SLOT(kinectUp()));
    QObject::connect(ui->down, SIGNAL(clicked()), this, SLOT(kinectDown()));
    //QObject::connect(ui->startButton, SIGNAL(clicked()), this, SLOT(stopKinect()));

//    _glWidget->setImages(*(gc->rgbMat), *(gc->depthf), *(gc->dst));
//    _glWidget->show();
}

GuiApp::~GuiApp()
{
    //timer->stop();
    //delete timer;5
    delete gc;
    delete ui;
    delete _glWidget;
}

void GuiApp::connect2lego(){
    gc->connect();
}

void GuiApp::refresh()
{
    //solo es mostrar la imagen q toque en cada momento
//    tStart = clock();
    switch(operation_mode){
    case 0:
    {
        calibrateKinect();

        QString cadena("     You are \n");
        cadena.append("         ");
        //printf("Valor %i ",gc->depthf->at <uchar> (gc->cDistance->x, gc->cDistance->y));
        //En realidad debe ser el valor de la funcion get distanceCM
        int distance = Utils::getDistanceFromSource(*(gc->depthMat), *(gc->cDistance));

        cadena.append(QString::number(distance));//gc->depthf->at <uint8_t> (gc->cDistance->x, gc->cDistance->y))
        cadena.append("  cm.\n             away");
//        ui->impresion->setText(cadena);

        Mat gris(*(gc->depthf));
        flip(gris, gris, 1);
//        Mat grayI(gris);
//        Scalar color(0,0,255);
//        Utils::applyColor(gris, grayI, color);

        showImage(gris, *(gc->rgbMat));

//        flip(*(gc->rgbMat), *(gc->rgbMat), 1);
        flip(*(gc->depthMat), *(gc->depthMat), 1);
        if(initiated == false){
            printf("una vez\n");
           _glWidget->setImages(*(gc->depthf), *(gc->depthMat), *(gc->dst));
           _glWidget->resize(640,480);
           _glWidget->show();
//           _glWidget->show();
           initiated = true;
        }
    }
        break;
    case 1:
    {
        bool ok = false;
        ok = gc->process();
        Hand* manita = gc->getHDObject().getRightHand();
        Point position = manita->getCenter();
        _wheel->draw(gc->getDst(), position);

        if(ok == true){
            ui->okLED->setStyleSheet(QString("image: url(:/resources/img/img/green-led16.png);"));
        }else{
            ui->okLED->setStyleSheet(QString("image: url(:/resources/img/img/red-led16.png);"));
        }
        //mapDepth2Color(*(gc->dst), *(gc->rgbMat));
        showImage(*(gc->dst), *(gc->rgbMat));
        ui->lcdNumber->display(gc->getHDObject().getFingersCount(LEFT_HAND));
        //pasar por parametro un valor booleano que
        //permita o no enviar los mensajes por BT
        //Quiza, dejar configurar el threshold al usuario
    }
        break;
    case 2:

        break;

    };//No olvidar el ';'
//    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
}

void GuiApp::kinectUp()
{
    gc->moveUp();
}

void GuiApp::kinectDown()
{
    gc->moveDown();
}

void GuiApp::switch2WorkMode(){
    ui->statusBar->showMessage("CALIBRANDO!");
    ui->calibrar->setEnabled(false);
    calibTimer.singleShot(C_TIME, this, SLOT(uncalibrate()));//10 segundos...
}

void GuiApp::uncalibrate(){
    ui->statusBar->showMessage("CALIBRACION FINALIZADA");
    operation_mode = WORK;//WORK
    //calibrating = false;
}

void GuiApp::calibrateKinect() {
    gc->calibrar();
//    calibrating = true;

}

//Mostrar en la esquina superior derecha la imagen a color y haciendo clic sobre ella que cambie
void GuiApp::showImage(const cv::Mat& big, const cv::Mat& small)
{
    Mat rgb1;
    Mat rgb2;

    // convert image to RGB format
    //Las matrices de destino tienen que tener los 3 canales. Realiza una copia implicita.
//    cv::cvtColor(big, rgb1, CV_GRAY2RGB);
//    cv::cvtColor(small, rgb2, CV_BGR2RGB);
    if(big.channels() == 1){
        cv::cvtColor(big, rgb1, CV_GRAY2RGB);//big is CV_8UC1 opencv type
    }else{
        cv::cvtColor(big, rgb1, CV_BGR2RGB);//it is CV_8UC3 3 channels opencv type
    }
    if(small.channels() == 1){
        cv::cvtColor(small, rgb2, CV_GRAY2RGB);
    }else{
        cv::cvtColor(small, rgb2, CV_BGR2RGB);
    }
    QImage toShow((const unsigned char*)(rgb1.data), rgb1.cols, rgb1.rows, QImage::Format_RGB888);
    QImage toShow2((const unsigned char*)(rgb2.data), rgb2.cols, rgb2.rows, QImage::Format_RGB888);
    QImage small2 = toShow2.scaled(ui->alternativa->width(), ui->alternativa->height(),Qt::KeepAspectRatio);

    ui->impresion->setPixmap(QPixmap::fromImage(toShow, Qt::AutoColor));
    ui->alternativa->setPixmap(QPixmap::fromImage(small2, Qt::AutoColor));
}

void GuiApp::showBigImage(cv::Mat& big)
{
    Mat rgb1(Size(ROWS,COLS),CV_16UC1,Scalar(0));

    if(big.channels() == 1){
        cv::cvtColor(big, rgb1, CV_GRAY2RGB);
    }else{
        cv::cvtColor(big, rgb1, CV_BGR2RGB);
    }
    QImage toShow((const unsigned char*)(rgb1.data), rgb1.cols, rgb1.rows, QImage::Format_RGB888);
    QImage big2 = toShow.scaled(ui->impresion->width(), ui->impresion->height(),Qt::KeepAspectRatio);

    ui->impresion->setPixmap(QPixmap::fromImage(big2, Qt::AutoColor));
}

void GuiApp::showSmallImage(cv::Mat& small)
{
    Mat rgb1(Size(ROWS,COLS),CV_16UC1,Scalar(0));

    if(small.channels() == 1){
        cv::cvtColor(small, rgb1, CV_GRAY2RGB);
    }else{
        cv::cvtColor(small, rgb1, CV_BGR2RGB);
    }
    QImage toShow((const unsigned char*)(rgb1.data), rgb1.cols, rgb1.rows, QImage::Format_RGB888);
    QImage small2 = toShow.scaled(ui->alternativa->width(), ui->alternativa->height(),Qt::KeepAspectRatio);

    ui->alternativa->setPixmap(QPixmap::fromImage(small2, Qt::AutoColor));
}

void GuiApp::closeEvent(QCloseEvent *event)
{
//Si llamo al deleteDevice o si destruyo el dispositivo sale lo de "pure virtual method called...2
    gc->freenect.deleteDevice(0);
    delete gc;//Ya está. GC usaba device, luego hay q liberarlo antes y en su propio destructor llama al de device
    //gc->freenect.deleteDevice(0);
    QMainWindow::closeEvent(event);
    //exit(0);
}

void GuiApp::contextMenuEvent(QContextMenuEvent *event){
    QMainWindow::contextMenuEvent(event);
}

QMenu * GuiApp::createPopupMenu(){
    return QMainWindow::createPopupMenu();
}

bool GuiApp::event(QEvent *event){
    return QMainWindow::event(event);
}
