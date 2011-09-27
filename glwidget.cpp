#include "glwidget.hpp"

#include "glwidget.hpp"

#include <QtGui>
#include <QtOpenGL/QtOpenGL>

#include <math.h>

#include "glwidget.hpp"
#include "utils.hpp"

/*
 * Constantes de calibracion basadas en el modelo de Nicolas Burrus.
 * Mas informacion en http://nicolas.burrus.name/index.php/Research/KinectCalibration
 */

const float GLWidget::R[] = {0.99984628826577793, 0.0012635359098409581, -0.017487233004436643, -0.0014779096108364480, 0.99992385683542895, -0.012251380107679535, 0.017470421412464927, 0.012275341476520762, 0.99977202419716948 };
const float GLWidget::T[] = {0.019985242312092553, -0.00074423738761617583, -0.010916736334336222 };
//Color
const float GLWidget::fx_rgb = 5.2921508098293293e+02;
const float GLWidget::fy_rgb = 5.2556393630057437e+02;
const float GLWidget::cx_rgb = 3.2894272028759258e+02;
const float GLWidget::cy_rgb = 2.6748068171871557e+02;
const float GLWidget::k1_rgb = 2.6451622333009589e-01;
const float GLWidget::k2_rgb = -8.3990749424620825e-01;
const float GLWidget::p1_rgb = -1.9922302173693159e-03;
const float GLWidget::p2_rgb = 1.4371995932897616e-03;
const float GLWidget::k3_rgb = 9.1192465078713847e-01;

//Depth
const float GLWidget::fx_d = 594.21434211923247;
const float GLWidget::fy_d = 591.04053696870778;
const float GLWidget::cx_d = 339.30780975300314;
const float GLWidget::cy_d = 242.73913761751615;
const float GLWidget::k1_d = -0.26386489753128833;
const float GLWidget::k2_d = 0.99966832163729757;
const float GLWidget::p1_d = -0.00076275862143610667;
const float GLWidget::p2_d = 0.0050350940090814270;
const float GLWidget::k3_d = -1.3053628089976321;

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    object = 0;
    image = 0;
    xRot = 0;
    yRot = 0;
    zRot = 0;

    trolltechGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
    trolltechPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);
}

GLWidget::~GLWidget()
{
    makeCurrent();
    glDeleteLists(image, 1);
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}

void GLWidget::setXRotation(int angle)
{
    normalizeAngle(&angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setYRotation(int angle)
{
    normalizeAngle(&angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setZRotation(int angle)
{
    normalizeAngle(&angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::initializeGL()
{
    qglClearColor(trolltechPurple.dark());
//    object = makeObject();
    setImages(_color, _depth, _dst);
    image = makeImage();
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.8f, 0.8f, 0.8f, 0.8f);
    glLoadIdentity();
    glTranslated(0.0, 0.0, -10.0);
    glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotated(zRot / 16.0, 0.0, 0.0, 1.0);
    glCallList(image);
}

void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5, +0.5, +0.5, -0.5, 4.0, 15.0);
    glMatrixMode(GL_MODELVIEW);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}


void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }
    lastPos = event->pos();
}

void GLWidget::keyPressEvent(QKeyEvent *event){
    int key = tolower(event->key());
    printf("key %c",key);

}

void GLWidget::setImages(Mat &color, Mat &depth, Mat &dst){
    _color = Mat(color);
    _depth = Mat(depth);
    _dst = Mat(dst);
    makeImage();
}

//Mejor pasar un vector<Point3d>
GLuint GLWidget::makeImage(){
    int minp = 10000;
    int maxp = 0;
    for(int i=0; i<_dst.rows; i++){
        for(int j=0; j<_dst.cols; j++){
            Point p = Point(i,j);
            float d = Utils::getDistanceFromSource(_depth, p);//_depth.at<uint16_t>(i,j);
            if(d > maxp)
                maxp = d;
            if(d < minp)
                minp = d;
        }
    }
//    gluLookAt(xoff,yoff,zoff, cent[0] + cxoff, cent[1]+cyoff, cent[2]+czoff, 0,1,0);
    GLuint list = glGenLists(1);
    glNewList(list, GL_COMPILE);
    glPointSize(2.0);
    glBegin(GL_POINTS);
    for(int i=0; i<_dst.rows; i++){
        for(int j=0; j<_dst.cols; j++){
            Point3f p3d;

//            p3d.x = ((float)i - cx_d) * ((float)_depth.at <uint8_t> (i,j)) / fx_d;
//            p3d.y = ((float)j - cy_d) * ((float)_depth.at <uint8_t> (i,j)) / fy_d;
            Point p = Point(i,j);
            p3d.z = _depth.at <uint16_t>(i, j)*255.0/DEPTH_RANGE;//_depth.at<uint16_t>(i,j);

            //p3dprima = R*p3d + T;
//            Point3f p3dprima;
//            p3dprima.x = (R[0]*p3d.x + R[1]*p3d.y + R[2]*p3d.z) + T[0];
//            p3dprima.y = (R[3]*p3d.x + R[4]*p3d.y + R[5]*p3d.z) + T[1];
//            p3dprima.z = (R[6]*p3d.x + R[7]*p3d.y + R[8]*p3d.z) + T[2];

//            GLdouble xp = p3dprima.x;
//            GLdouble yp = p3dprima.y;
            GLdouble zp = p3d.z;
//            Vec3b color = _color.at<Vec3b>(i,j);
            int color = _color.at<uint8_t>(i,j);
            //Coger el color que hay en p2d.x, p2d.y y pintarlo en dst
//            qglColor(QColor(color[0], color[1], color[2]));
            qglColor(QColor(color, color, color));
//            GLdouble angle = 30.0;

//            Point2f p2d;
//            xp = (p3dprima.x * fx_rgb / p3dprima.z) + cx_rgb;
//            yp = (p3dprima.y * fy_rgb / p3dprima.z) + cy_rgb;

            //y = (ymax-ymin)*(x-xmin)/(xmax-xmin) + ymin

            double d = zp/(maxp-minp);
            if (d >= 0.2 && d<=0.95)
            glVertex3d(i/800.0, j/800.0, zp/(maxp-minp));//100.0
//            printf("profundidad %f", zp);
//            glRotated(angle, xp,yp,zp);
//            printf("xp %f, yp %f, zp %f", xp, yp, zp);

//            GLdouble xp = abs(p3dprima.x);
//            GLdouble yp = p3dprima.y;
//            GLdouble zp = p3dprima.z;
//            qglColor(trolltechGreen);
//           glVertex3d(xp, yp, zp/100);

//            GLdouble x5 = abs(p3dprima.x);
//            GLdouble y6 = (double)p3dprima.y;
//            GLdouble x7 = (double)p3dprima.z;
//           quad(x5, y6, x7, 1.0, 200.5, 100.6, 3.5, 4.6);

//            //_dst.at <uint8_t> (i, j) = 0;
//            if(p2d.x >=0 && p2d.y >= 0){
//                gl
////                dst.at <uint8_t> (p2d.x, p2d.y) = depth.at <uint8_t> (i, j);
////                printf("p2d.x %f, p2d.y %f\n", p2d.x, p2d.y);
//                if(_depth.at <uint8_t> (i, j) == 255){

//                    //_dst.at <uint8_t> (i, j) = 0;
//                }else{
//                    //_dst.at <uint8_t> (p2d.x, p2d.y) = _depth.at <uint8_t> (i, j);
//                }
//            }
        }
    }
    glEnd();

    glEndList();

    printf("minp %d, maxp %d\n", minp, maxp);
    return list;
}

GLuint GLWidget::makeObject()
{
    GLuint list = glGenLists(1);
    glNewList(list, GL_COMPILE);

    glBegin(GL_QUADS);

    const double Pi = 3.14159265358979323846;
    const int NumSectors = 200;

    for (int j = 0; j < 2; ++j) {
        double r = 0.1 + (j * 0.2);

        for (int i = 0; i < NumSectors; ++i) {
            double angle1 = (i * 2 * Pi) / NumSectors;
            GLdouble x5 = (r + 0.1) * sin(angle1);
            GLdouble y5 = (r + 0.1) * cos(angle1);
            GLdouble x6 = r * sin(angle1);
            GLdouble y6 = r * cos(angle1);

            double angle2 = ((i + 1) * 2 * Pi) / NumSectors;
            GLdouble x7 = r * sin(angle2);
            GLdouble y7 = r * cos(angle2);
            GLdouble x8 = (r + 0.1) * sin(angle2);
            GLdouble y8 = (r + 0.1) * cos(angle2);

            quad(x5, y5, x6, y6, x7, y7, x8, y8);

            extrude(x6, y6, x7, y7);
            extrude(x8, y8, x5, y5);
        }
    }

    glEnd();

    glEndList();
    return list;
}

void GLWidget::quad(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2,
                    GLdouble x3, GLdouble y3, GLdouble x4, GLdouble y4)
{
    qglColor(trolltechGreen);

    glVertex3d(x1, y1, -0.05);
    glVertex3d(x2, y2, -0.05);
    glVertex3d(x3, y3, -0.05);
    glVertex3d(x4, y4, -0.05);

    glVertex3d(x4, y4, +0.05);
    glVertex3d(x3, y3, +0.05);
    glVertex3d(x2, y2, +0.05);
    glVertex3d(x1, y1, +0.05);
}

void GLWidget::extrude(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2)
{
    qglColor(trolltechGreen.dark(250 + int(100 * x1)));

    glVertex3d(x1, y1, +0.05);
    glVertex3d(x2, y2, +0.05);
    glVertex3d(x2, y2, -0.05);
    glVertex3d(x1, y1, -0.05);
}

void GLWidget::normalizeAngle(int *angle)
{
    while (*angle < 0)
        *angle += 360 * 16;
    while (*angle > 360 * 16)
        *angle -= 360 * 16;
}
