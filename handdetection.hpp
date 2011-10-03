
#ifndef HANDDETECTION_HPP
#define	HANDDETECTION_HPP

#include <iostream>
#include <vector>
#include <stdio.h>
#include <cmath>
#include <exception>
#include <pthread.h>

#include <opencv/cv.h>
#include <opencv/cv.hpp>

#include "guiappheaders.hpp"
#include "utils.hpp"
#include "hand.hpp"
#include "trackobject.hpp"

/*! \namespace std */
using namespace std;
/*! \namespace cv */
using namespace cv;

/*!
  * \class HandDetection
  * \brief Detección y análisis de imagen para reconocer gestos con las manos.
  *
  * A partir de un conjunto de contornos (cada contorno es un conjunto de puntos),
  * se determinará qué conjunto corresponde a la mano izquierda y cuál a la mano
  * derecha mediante una criba por tamaño, posición relativa en la imagen y posición
  * respecto a la predicción realizada por el filtro de Kalman.
  * También, permite detectar el número de dedos extendidos, siendo 0 con el
  * puño cerrado.
  *
  * \file handdetection.hpp
  *
  * \author José Manuel Cabrera Canal
  * \version 1.0
  */
class HandDetection {

private:
    ///Miembros o atributos de la clase
    Hand* m_left_hand; /*!< Representa la mano izquierda. */
    Hand* m_right_hand; /*!< Identifica la mano derecha. */
    bool m_no_template; /*!< Indica si se trata de la mano derecha o de la mano izquierda. */

    ///Métodos de la clase
    /*!
      * Cálculo de los vértices de un polígono convexo. Este se
      * extrae de un vector de puntos a partir de la transformada
      * de Hull. El polígono se aproxima a una cantidad de lados,
      * indicada mediante la variable #APPROX_POLY_SIDES.
      * Esta función consiste en un "wrapper" o adaptación para
      * la función de openCV "cvConvexityDefects" en su versión
      * para el lenguaje C, pues no estaba disponible para C++ y
      * ha sido necesaria "reimplementarla" (utilizando la función
      * original, se sobreentiende).
      *
      * El reconocimiento de los gestos está prácticamente recogido aquí.
      *
      * \param contour El vector de puntos cuyos vértices queremos calcular.
      * \param hull Transformada de Hull, calculada de forma separada.
      * \param defects Parmátro de salida. Contiene el vector resultante con los vértices.
      */
    void findConvexityDefects(vector<Point>& contour, vector<int>& hull, vector<CvConvexityDefect>& defects);

    /*!
      * Función que se apoya en el resultado de #findConvexityDefects.
      * De los vértices del polígono convexo, estima cuáles se pueden corresponder
      * con dedos de la mano, en base a la distancia con el centro de la misma.
      * \param cvxHull Polígono convexo.
      * \param defects Vértices del polígono convexo o defectos de convexidad.
      * \param Vector donde almacenaremos el resultado, es decir, cada uno de los dedos detectados.
      */
    void findFingerTips(vector<Point>& cvxHull, vector<CvConvexityDefect>& defects, vector<CvConvexityDefect>& fingerTips);

public:
    ///Métodos de la clase
    /*!
      * Constructor de clase.
      */
    HandDetection();
    /*!
      * Destructor de clase.
      */
    ~HandDetection();
    /*!
      * \return Devuelve un objeto mano, en concreto la mano izquierda.
      */
    Hand* getLeftHand() const;
    /*!
      * \return Devuelve la mano derecha.
      */
    Hand* getRightHand() const;
    /*!
      * \param which El identificador del objeto Mano. Véase #LEFT_HAND, RIGHT_HAND.
      * \return Un objeto de tipo Mano.
      */
    Hand* getHand(int which) const;

    /*!
      * Detecta tanto la mano izquierda como la derecha  y calcula el número
      * de dedos abiertos para ambas. Además, actualiza la imagen señalando
      * mediante una cruz la posición estimada, la calculada y la predecida por
      * el filtro. Para la mano derecha (izquierda en la imagen) pinta un punto
      * por cada dedo detectado. Los mejores resultados se obtienen a unos 90
      * cm. del objetivo.
      *
      * \param depthImage Imagen segmentada con la escena actual.
      * \param color Imagen destino, representa la escena resultante.
      */
    void detectHands(Mat& depthImage, Mat& color);
    /*!
      * Devuelve el número de dedos extendidos para una mano dada.
      * \param hand El identificador de la mano. Véase #LEFT_HAND, RIGHT_HAND.
      * \return El número de dedos detectados.
      */
    int getFingersCount(int hand) const;

};

#endif	/* HANDDETECTION_HPP */

