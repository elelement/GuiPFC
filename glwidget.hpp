#ifndef GLWIDGET_HPP
#define GLWIDGET_HPP

#include <QGLWidget>
#include <opencv/cv.h>
#include <opencv/cv.hpp>

using namespace cv;

class GLWidget : public QGLWidget
 {
     Q_OBJECT

 public:
    //R y T son las matrices de rotacion y de traslacion, respectivamente
    //Valores obtenidos de la calibracion realizada por Nicolas Burrus
    //http://nicolas.burrus.name/index.php/Research/KinectCalibration
    static const float R[];
    static const float T[];

    //Color
    static const float fx_rgb;
    static const float fy_rgb;
    static const float cx_rgb;
    static const float cy_rgb;
    static const float k1_rgb;
    static const float k2_rgb;
    static const float p1_rgb;
    static const float p2_rgb;
    static const float k3_rgb;
    //Depth
    static const float fx_d;
    static const float fy_d;
    static const float cx_d;
    static const float cy_d;
    static const float k1_d;
    static const float k2_d;
    static const float p1_d;
    static const float p2_d;
    static const float k3_d;
     GLWidget(QWidget *parent = 0);
     ~GLWidget();

     QSize minimumSizeHint() const;
     QSize sizeHint() const;
     void setImages(Mat& color, Mat& depth, Mat& dst);

 public slots:
     void setXRotation(int angle);
     void setYRotation(int angle);
     void setZRotation(int angle);

 signals:
     void xRotationChanged(int angle);
     void yRotationChanged(int angle);
     void zRotationChanged(int angle);

 protected:
     void initializeGL();
     void paintGL();
     void resizeGL(int width, int height);
     void mousePressEvent(QMouseEvent *event);
     void mouseMoveEvent(QMouseEvent *event);
     void keyPressEvent(QKeyEvent *event);

 private:
     GLuint makeObject();
     GLuint makeImage();
     void quad(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2,
               GLdouble x3, GLdouble y3, GLdouble x4, GLdouble y4);
     void extrude(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
     void normalizeAngle(int *angle);

     Mat _color, _depth, _dst;
     GLuint object;
     GLuint image;
     int xRot;
     int yRot;
     int zRot;
     QPoint lastPos;
     QColor trolltechGreen;
     QColor trolltechPurple;
 };

#endif // GLWIDGET_HPP
