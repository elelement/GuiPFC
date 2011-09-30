#ifndef GLWIDGET_HPP
#define GLWIDGET_HPP

#include <QGLWidget>
#include <opencv/cv.h>
#include <opencv/cv.hpp>
#include <QtGui>
#include <QtOpenGL/QtOpenGL>
#include <math.h>
#include "utils.hpp"

/*! \namespace cv */
using namespace cv;

/*!
  * \class GLWidget
  * \brief Widget que sirve para mostrar el primer frame
            capturado por Kinect en 3D, en forma de nube
            de puntos.
  * Implementa los métodos para rotar la escena y pintar
  * los puntos en el espacio tridimensional.
  * Se trata de una clase adapatada de los ejemplos disponibles
  * en la web de Nokia: <a href="http://doc.qt.nokia.com/4.7-snapshot/opengl-hellogl.html">Ejemplo Nokia QT OpenGL</a>
  *
  * \file glwidget.cpp
  *
  * \author José Manuel Cabrera Canal
  * \version 1.0
  */
class GLWidget : public QGLWidget
 {
     Q_OBJECT

 public:
    /*!
      * Constructor de clase.
      * \param parent Objeto padre sobre el que se va a crear el Widget.
      */
     GLWidget(QWidget *parent = 0);
     ~GLWidget();

     QSize minimumSizeHint() const;
     QSize sizeHint() const;
     /*!
       * Asigna valores a las imágenes que va a utilizar para
       * construir la imagen en 3D.
       * \param color Imagen de color, por si queremos una nube
                        de puntos en color (necesita calibración).
       * \param depth Imagen de profundidad normalizada.
       * \param dst Datos "brutos" de la profundidad.
       */
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
     /*!
       * Método modificado para construir la nube de puntos
       * a partir de la imagen de profundidad obtenida del
       * Kinect.
       */
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
