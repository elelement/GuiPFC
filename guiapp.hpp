#ifndef GUIAPP_HPP
#define GUIAPP_HPP

#include <QMainWindow>
#include <QTimer>
#include <QFuture>
#include <QtConcurrentRun>
#include <time.h>
#include "libfreenect.hpp"
#include "btcomm.hpp"
#include "mykinect.hpp"
#include "handdetection.hpp"
#include "guicontroller.hpp"
#include "steeringwheel.hpp"
#include "utils.hpp"
#include "glwidget.hpp"


namespace Ui {
    class GuiApp;
}

class GuiApp : public QMainWindow
{
    Q_OBJECT

public slots:
    void uncalibrate();
    void calibrateKinect();
    void kinectUp();
    void kinectDown();
    void refresh();
    void switch2WorkMode();
    void connectBT();
    void sendBTMessage();

public:
    int MODO_OPERACION;
    GUIController* gc;

    explicit GuiApp(QWidget *parent = 0);
    ~GuiApp();
    void showImage(const cv::Mat& big, const cv::Mat& small);
    void showSmallImage(cv::Mat& small);
    void showBigImage(cv::Mat& big);
    void closeEvent(QCloseEvent *event);
    QMenu* createPopupMenu();
    void contextMenuEvent(QContextMenuEvent *event);
    bool event(QEvent *event);

private:
    QFuture<void> _thread;
    Ui::GuiApp *ui;
    SteeringWheel* _wheel;
    int frameNumber;
    bool initiated;
    int sentido;
    unsigned int cDepth;
    QTimer* timer, calibTimer;
    QTimer* send_timer;
    clock_t tStart;
    bool calibrating;
    enum operation_mode_t {CALIBRATION, WORK, STOP};
    enum operation_mode_t operation_mode;
    GLWidget *_glWidget;

};

#endif // GUIAPP_HPP
