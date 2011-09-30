#include "guiapp.hpp"

GuiApp::GuiApp(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::GuiApp)
{
    m_ui->setupUi(this);
    m_ui->label_2->setVisible(false);
    m_gl_widget = new GLWidget;

    m_gc = new GUIController();
    m_wheel = new SteeringWheel();
    m_initiated = false;
    m_operation_mode = CALIBRATION;
    m_sentido = MARCHA_ADELANTE;
    m_mano_anterior_abierta = false;

    m_gc->startDevice();

    m_ui->statusBar->setStyleSheet("color: rgb(59, 58, 54);font: 75 11pt 'Ubuntu';");
    m_ui->statusBar->showMessage("Conectando... Espere por favor.");
    m_timer = new QTimer(this);
    m_timer->setInterval(REFRESH_TIMER);//aprox 24 fps

    m_ui->lcdNumber->display(0);
    m_timer->start();
    m_ui->reconnect->setVisible(false);

    QObject::connect(m_ui->reconnect, SIGNAL(clicked()), this, SLOT(connectBT()));
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(refresh()));

    QObject::connect(m_ui->calibrar, SIGNAL(clicked()), this, SLOT(switch2WorkMode()));
    QObject::connect(m_ui->up, SIGNAL(clicked()), this, SLOT(kinectUp()));
    QObject::connect(m_ui->down, SIGNAL(clicked()), this, SLOT(kinectDown()));
}

GuiApp::~GuiApp()
{
    delete m_gc;
    delete m_ui;
    delete m_gl_widget;
}

void GuiApp::sendBTMessage(){
    Hand* manita = m_gc->getHDObject().getRightHand();
    Point position = manita->getCenter();

    int marcha = m_sentido*(m_gc->getHDObject().getFingersCount(LEFT_HAND));

    //Enviamos los datos via bluetooth
    //El formato sera *marcha*giro_respecto_perpendicular_centro_volante*
    int angulo = Utils::getAngleOX(position, *(m_wheel->getCenter())) * 180/M_PI;

    if(angulo < 90 && angulo > 20){
        angulo = angulo - 90;
    }else if(angulo > -90 && angulo < -20){
        angulo = angulo + 90;
    }else{
        angulo = 0;
    }

    QString qs("*");
    qs.append(QString::number(marcha)).append("#").append(QString::number(angulo)).append("@");
    string to_send = qs.toStdString();
    printf("Enviar : %s\n", to_send.c_str());
    m_ui->statusBar->showMessage(qs);
    if(m_gc->btc1->send_message(to_send.c_str(), to_send.size()+1) < 0){//+1 por el car. null
        //error, hay que reabrir la conexión
        m_ui->reconnect->setVisible(true);
        m_ui->statusBar->showMessage("Se ha perdido la conexion. Reintentelo pulsando el boton de reconectar.");
        m_ui->btLED->setStyleSheet(QString("image: url(:/resources/img/img/red-led16.png);"));
    }
}

void GuiApp::connectBT(){
    m_ui->statusBar->showMessage("Conectando... Espere por favor.");
    if(m_gc->connect() < 0){
        m_ui->reconnect->setVisible(true);
        m_ui->statusBar->showMessage("Imposible conectar. Reintentelo pulsando el boton de reconectar.");
         m_ui->btLED->setStyleSheet(QString("image: url(:/resources/img/img/red-led16.png);"));
    }else{
        m_ui->statusBar->showMessage("Conectado.");
        m_ui->btLED->setStyleSheet(QString("image: url(:/resources/img/img/green-led16.png);"));
        //Iniciamos el hilo
        m_send_timer = new QTimer(this);
        m_send_timer->setInterval(SEND_TIME);
        m_send_timer->start();
        QObject::connect(m_send_timer, SIGNAL(timeout()), this, SLOT(sendBTMessage()));
        //_thread = QtConcurrent::run(send_message, message, mlength);
    }
}

void GuiApp::refresh()
{
    //solo es mostrar la imagen q toque en cada momento
    switch(m_operation_mode){
    case 0:
    {
        if(m_initiated == false){
            //Conectamos via bluetooth
            connectBT();
        }
        calibrateKinect();

        QString cadena("     You are \n");
        cadena.append("         ");
        int distance = Utils::getDistanceFromSource(*(m_gc->depthMat), *(m_gc->cDistance));

        cadena.append(QString::number(distance));//m_gc->depthf->at <uint8_t> (m_gc->cDistance->x, m_gc->cDistance->y))
        cadena.append("  cm.\n             away");
        m_ui->impresion->setText(cadena);

        Mat gris(*(m_gc->depthf));
        flip(gris, gris, 1);

        flip(*(m_gc->depthMat), *(m_gc->depthMat), 1);
        if(m_initiated == false){
            //Creamos la nube de puntos
           m_gl_widget->setImages(*(m_gc->depthf), *(m_gc->depthMat), *(m_gc->dst));
           m_gl_widget->resize(640,480);
           m_gl_widget->show();
           m_initiated = true;
        }
    }
        break;
    case 1:
    {
        bool ok = false;
        m_ui->label_2->setVisible(true);
        ok = m_gc->process();
        Hand* manita = m_gc->getHDObject().getRightHand();
        Point position = manita->getCenter();
        m_wheel->draw(m_gc->getDst(), position);

        if(ok == true){
            m_ui->okLED->setStyleSheet(QString("image: url(:/resources/img/img/green-led16.png);"));
        }else{
            m_ui->okLED->setStyleSheet(QString("image: url(:/resources/img/img/red-led16.png);"));
        }
        //mapDepth2Color(*(m_gc->dst), *(m_gc->rgbMat));
        int angulo = Utils::getAngleOX(position, *(m_wheel->getCenter())) * 180/M_PI;

        if(angulo < 90 && angulo > 20){
            angulo = angulo - 90;
        }else if(angulo > -90 && angulo < -20){
            angulo = angulo + 90;
        }else{
            angulo = 0;
        }

        m_ui->label_2->setText(QString::number(angulo));

        int marcha = (m_gc->getHDObject().getFingersCount(LEFT_HAND));
        int dedos = m_gc->getHDObject().getFingersCount(RIGHT_HAND);
        if(dedos > 3 && !m_mano_anterior_abierta ){
            if(m_sentido == MARCHA_ATRAS){
                m_sentido = MARCHA_ADELANTE;
            }else if(dedos > 3){
                m_sentido = MARCHA_ATRAS;
            }
            m_mano_anterior_abierta = true;
        }
        if(dedos <= 3){
            m_mano_anterior_abierta = false;
        }else{
            m_mano_anterior_abierta = true;
        }
        marcha = m_sentido * marcha;
        m_ui->lcdNumber->display(marcha);
        showImage(*(m_gc->dst), *(m_gc->rgbMat));

    }
        break;
    case 2:

        break;

    };//No olvidar el ';'
//    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
}

void GuiApp::kinectUp()
{
    m_gc->moveUp();
}

void GuiApp::kinectDown()
{
    m_gc->moveDown();
}

void GuiApp::switch2WorkMode(){
    m_ui->statusBar->showMessage("CALIBRANDO!");
    m_ui->calibrar->setEnabled(false);
    m_calib_timer.singleShot(C_TIME, this, SLOT(uncalibrate()));//10 segundos...
}

void GuiApp::uncalibrate(){
    m_ui->statusBar->showMessage("CALIBRACION FINALIZADA");
    m_operation_mode = WORK;//WORK
    //m_calibrating = false;
}

void GuiApp::calibrateKinect() {
    m_gc->calibrar();
//    m_calibrating = true;

}

//Mostrar en la esquina superior derecha la imagen a color y haciendo clic sobre ella que cambie
void GuiApp::showImage(const cv::Mat& big, const cv::Mat& small)
{
    Mat rgb1;
    Mat rgb2;

    //Las matrices de destino tienen que tener los 3 canales. Realiza una copia implicita.
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
    QImage small2 = toShow2.scaled(m_ui->alternativa->width(), m_ui->alternativa->height(),Qt::KeepAspectRatio);

    m_ui->impresion->setPixmap(QPixmap::fromImage(toShow, Qt::AutoColor));
    m_ui->alternativa->setPixmap(QPixmap::fromImage(small2, Qt::AutoColor));
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
    QImage big2 = toShow.scaled(m_ui->impresion->width(), m_ui->impresion->height(),Qt::KeepAspectRatio);

    m_ui->impresion->setPixmap(QPixmap::fromImage(big2, Qt::AutoColor));
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
    QImage small2 = toShow.scaled(m_ui->alternativa->width(), m_ui->alternativa->height(),Qt::KeepAspectRatio);

    m_ui->alternativa->setPixmap(QPixmap::fromImage(small2, Qt::AutoColor));
}

void GuiApp::closeEvent(QCloseEvent *event)
{
    m_gc->freenect.deleteDevice(0);
    delete m_gc;
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
