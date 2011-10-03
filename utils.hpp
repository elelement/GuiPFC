#ifndef UTILS_HPP
#define UTILS_HPP
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/cv.hpp>
#include <opencv/cxcore.h>
#include <opencv/cxcore.hpp>
#include <opencv/highgui.h>
#include <opencv/highgui.hpp>

#include "guiappheaders.hpp"
#include "mykinect.hpp"

/*! \namespace cv */
using namespace cv ;

/*!
  * \class Utils
  * \brief Biblioteca de métodos estáticos con operaciones frecuentemente utilizadas de carácter genérico.
  *
  *
  * \file utils.hpp
  *
  * \author José Manuel Cabrera Canal
  * \version 1.0
  */
class Utils
{
public:

    /*!
      * Normaliza los datos en bruto recibidos del Kinect, obteniendo
      * una imagen en escala de grises.
      * \param rawMat Datos en bruto.
      * \param depth Matriz resultante en escala de grises.
      */
    static void normalizeRawData(Mat& rawMat, Mat& depth);
    /*!
      * Realiza una segmentación en blanco y negro sobre una imagen
      * en escala de grises, descartando todos los valores que no estén
      * entre "th" y "max".
      * \param gray Imagen en escala de grises
      * \param bw Imagen blanco y negro resultante.
      * \param th Límite inferior.
      * \param max Límite superior.
      */
    static void mythreshold(Mat& gray, Mat& bw, uint8_t th, int max);

    /*!
      * Devuelve el punto más cercano en la imagen, es decir,
      * aquél cuya distancia a la fuente sea menor (en cm.).
      * \param depthImg Imagen en escala de grises.
      * \return El punto más cercano en la imagen.
      */
    static Point getClosestPoint(Mat& depthImg);
    /*!
      * Devuelve la distancia en centímetros de un punto con
      * respecto al Kinect. Se utiliza un algoritmo basado en
      * resultados empíricos obtenido de la siguiente web:
      * <a href="http://vvvv.org/forum/the-kinect-thread">http://vvvv.org/forum/the-kinect-thread</a>
      * Los resultados obtenidos son bastante precisos con la realidad observada.
      * \param depthMat Datos en bruto de profundidad.
      * \param p Un punto cualquiera.
      * \return La distancia en centímetros.
      */
    static float getDistanceFromSource(Mat& depthMat, Point& p);
    /*!
      * Obtiene el centro de un rectángulo dado.
      * \param r El rectángulo en cuestión.
      * \return El centro del rectángulo.
      */
    static Point getRectCenter(Rect const& r);
    /*!
      * Realiza un "opening", o lo que es lo mismo,
      * un "erode" (erosión) más un "dilate" (ensanchamiento).
      * Se trata de una operación típica en visión para
      * eliminar ruido en la imagen de los bordes.
      *
      * \param hand Matriz origen.
      * \param dst Matriz resultante.
      */
    static void opening(Mat& hand, Mat& dst);
    /*!
      * Compara dos puntos en lo que al eje OX se refiere, es decir,
      * si un un punto está más a la derecha en la imagen que otro.
      * \param p1 Un punto cualquiera.
      * \param p2 Un punto cualquiera.
      * \return 1 si está más a la derecha, 0 si son iguales, y -1 si está más a la izquierda.
      */
    static int compareToXAxis(Point& p1, Point& p2);
    /*!
      * Compara dos puntos en lo que al eje OY se refiere, es decir,
      * si un un punto está más abajo en la imagen que otro.
      * \param p1 Un punto cualquiera.
      * \param p2 Un punto cualquiera.
      * \return 1 si está por debajo, 0 si están a la misma altura y -1 en otro caso.
      */
    static int compareToYAxis(Point& p1, Point& p2);
    /*!
      * Devuelve la distancia euclídea entre dos puntos
      * \param p1 Un punto cualquiera.
      * \param ref Un punto cualquiera.
      * \return La distancia entre ambos puntos.
      */
    static double euclideanDistance(Point& p1, Point& ref);
    /*!
      * Devuelve la distancia en píxeles con respecto al eje OX.
      * \param p1 Un punto cualquiera.
      * \param ref Un punto cualquiera.
      * \return La distancia entre ambos puntos.
      */
    static int OXDistance(Point& p1, Point& ref);
    /*!
      * Permite dibujar la transformada de Hull sobre la imagen.
      * \param matrix Matriz sobre la que se pinta.
      * \param contour Contorno del objeto.
      * \param cvxHull Polígono convexo del contorno anterior.
      */
    static void drawConvexHull(Mat& matrix, vector<Point>& contour, vector<int>& cvxHull);
    /*!
      * Calcula el punto de corte de la recta con ángulo "angle"
      * que pasa por el punto "center" y tiene de radio "radius"
      * con la circunferencia situada también en "center".
      * Útil para saber el punto de corte de los radios del volante.
      *
      * \param center Centro de la circunferencia.
      * \param radius Radio de la circunferencia.
      * \param angle Ángulo que forma la recta que pasa por el centro con respecto al eje OX.
      * \return El punto de corte.
      */
    static Point getCirclePoint(Point& center, double radius, double angle);
    /*!
      * Devuelve el ángulo que forma el segmento formado por p1 y p2
      * con respecto al eje OX horizontal.
      * \param p1 Un punto cualquiera.
      * \param p2 Un punto cualquiera.
      * \return El ángulo resultante.
      */
    static float getAngleOX(Point& p1, Point& p2);
    /*!
      * Dibuja una cruz con el color, tamaño y situación indicados.
      * \param img Imagen destino de la cruz.
      * \param center Centro de la cruz.
      * \param color Color de la cruz.
      * \param d Tamaño de la cruz.
      */
    static void drawCross(Mat& img, Point center, Scalar color, int d);
    /*!
      * Permite rotar una imagen un determinado número de ángulos.
      * \param source Imagen fuente.
      * \param angle Ángulo en radianes.
      * \return La imagen resultante.
      */
    static Mat rotateImage(const Mat& source, double angle);
    /*!
      * Devuelve el ángulo formado por los segmentos resultantes
      * de unir dos puntos con un mismo punto de referencia.
      * El cálculo se basa en el clásico producto escalar de dos
      * vectores.
      *
      * \param p1 Un punto cualquiera.
      * \param p2 Un punto cualquiera.
      * \param reference El punto en común de referencia.
      * \return El ángulo entre los dos puntos.
      */
    static float getAngle(Point& p1, Point& p2, Point& reference);

};

#endif // UTILS_HPP
