#ifndef GUIAPP_HPP
#define GUIAPP_HPP

#include <QMainWindow>
#include <QTimer>
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
    void connect2lego();//que se haga mediante un gesto y que indique si ha salido bien con una luz
    void switch2WorkMode();

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
    Ui::GuiApp *ui;
    SteeringWheel* _wheel;
    bool initiated;
    unsigned int cDepth;
    QTimer* timer, calibTimer;
    clock_t tStart;
    bool calibrating;
    enum operation_mode_t {CALIBRATION, WORK, STOP};
    enum operation_mode_t operation_mode;
    GLWidget *_glWidget;

};

#endif // GUIAPP_HPP
