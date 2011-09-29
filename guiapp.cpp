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
    ui->label_2->setVisible(false);
    _glWidget = new GLWidget;

    gc = new GUIController();
    _wheel = new SteeringWheel();
    initiated = false;
    operation_mode = CALIBRATION;
    sentido = MARCHA_ADELANTE;
    manoAnteriorAbierta = false;

    gc->startDevice();

    calibrating = false;
    ui->statusBar->setStyleSheet("color: rgb(59, 58, 54);font: 75 11pt 'Ubuntu';");
    ui->statusBar->showMessage("Conectando... Espere por favor.");
    timer = new QTimer(this);
    timer->setInterval(REFRESH_TIMER);//aprox 24 fps

    ui->lcdNumber->display(0);
    timer->start();
    ui->reconnect->setVisible(false);

    QObject::connect(ui->reconnect, SIGNAL(clicked()), this, SLOT(connectBT()));
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));

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

void GuiApp::sendBTMessage(){
    Hand* manita = gc->getHDObject().getRightHand();
    Point position = manita->getCenter();
    _anterior = Point(position.x, position.y);

    int marcha = sentido*(gc->getHDObject().getFingersCount(LEFT_HAND));

    //Enviamos los datos via bluetooth
    //El formato sera *marcha*giro_respecto_perpendicular_centro_volante*

    printf(("Position: %d, %d \n"), position.x, position.y);
    printf(("Wheel: %d, %d \n"), _wheel->getCenter()->x, _wheel->getCenter()->y);
    int angulo = Utils::getAngleOX(position, *(_wheel->getCenter())) * 180/M_PI;

    if(angulo < 90 && angulo > 20){
        angulo = angulo - 90;
    }else if(angulo > -90 && angulo < -20){
        angulo = angulo + 90;
    }else{
        angulo = 0;
    }

    printf("Marcha %d, Giro %d\n", marcha, angulo);

    QString qs("*");
    qs.append(QString::number(marcha)).append("#").append(QString::number(angulo)).append("@");
    string to_send = qs.toStdString();
    printf("Enviar : %s\n", to_send.c_str());
    ui->statusBar->showMessage(qs);
    if(gc->btc1->send_message(to_send.c_str(), to_send.size()+1) < 0){//+1 por el car. null
        //error, hay que reabrir la conexión
        ui->reconnect->setVisible(true);
        ui->statusBar->showMessage("Se ha perdido la conexion. Reintentelo pulsando el boton de reconectar.");
        ui->btLED->setStyleSheet(QString("image: url(:/resources/img/img/red-led16.png);"));
    }
}

void GuiApp::connectBT(){
    ui->statusBar->showMessage("Conectando... Espere por favor.");
    if(gc->connect() < 0){
        ui->reconnect->setVisible(true);
        ui->statusBar->showMessage("Imposible conectar. Reintentelo pulsando el boton de reconectar.");
         ui->btLED->setStyleSheet(QString("image: url(:/resources/img/img/red-led16.png);"));
    }else{
        ui->statusBar->showMessage("Conectado.");
        ui->btLED->setStyleSheet(QString("image: url(:/resources/img/img/green-led16.png);"));
        //Iniciamos el hilo
        send_timer = new QTimer(this);
        send_timer->setInterval(SEND_TIME);
        send_timer->start();
        QObject::connect(send_timer, SIGNAL(timeout()), this, SLOT(sendBTMessage()));
        //_thread = QtConcurrent::run(send_message, message, mlength);
    }
}

void GuiApp::refresh()
{
    //solo es mostrar la imagen q toque en cada momento
//    tStart = clock();
    switch(operation_mode){
    case 0:
    {
        if(initiated == false){
            //Conectamos via bluetooth
            connectBT();
        }
        calibrateKinect();

        QString cadena("     You are \n");
        cadena.append("         ");
        int distance = Utils::getDistanceFromSource(*(gc->depthMat), *(gc->cDistance));

        cadena.append(QString::number(distance));//gc->depthf->at <uint8_t> (gc->cDistance->x, gc->cDistance->y))
        cadena.append("  cm.\n             away");
        ui->impresion->setText(cadena);

        Mat gris(*(gc->depthf));
        flip(gris, gris, 1);

//        flip(*(gc->rgbMat), *(gc->rgbMat), 1);
        flip(*(gc->depthMat), *(gc->depthMat), 1);
        if(initiated == false){
            //Creamos la nube de puntos
           _glWidget->setImages(*(gc->depthf), *(gc->depthMat), *(gc->dst));
           _glWidget->resize(640,480);
           _glWidget->show();
           initiated = true;
        }
    }
        break;
    case 1:
    {
        bool ok = false;
        ui->label_2->setVisible(true);
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
        int angulo = Utils::getAngleOX(position, *(_wheel->getCenter())) * 180/M_PI;

        if(angulo < 90 && angulo > 20){
            angulo = angulo - 90;
        }else if(angulo > -90 && angulo < -20){
            angulo = angulo + 90;
        }else{
            angulo = 0;
        }

        ui->label_2->setText(QString::number(angulo));

        int marcha = (gc->getHDObject().getFingersCount(LEFT_HAND));
        int dedos = gc->getHDObject().getFingersCount(RIGHT_HAND);
        if(dedos > 3 && !manoAnteriorAbierta ){
            if(sentido == MARCHA_ATRAS){
                sentido = MARCHA_ADELANTE;
            }else if(dedos > 3){
                sentido = MARCHA_ATRAS;
            }
            manoAnteriorAbierta = true;
        }
        if(dedos <= 3){
            manoAnteriorAbierta = false;
        }else{
            manoAnteriorAbierta = true;
        }
        marcha = sentido * marcha;
        ui->lcdNumber->display(marcha);
        showImage(*(gc->dst), *(gc->rgbMat));

        //Actualizamos el valor anterior de la mano
        _anterior.x = position.x;
        _anterior.y = position.y;

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
