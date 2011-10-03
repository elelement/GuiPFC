#ifndef TRACKOBJECT_HPP
#define TRACKOBJECT_HPP

#include <opencv/cv.h>
#include <opencv/cv.hpp>
#include <stdio.h>
#include "guiappheaders.hpp"

/*! \namespace cv */
using namespace cv;

/*!
  * \class TrackObject
  * \brief Clase abstracta para la implementación de un "tracking" o seguimiento sobre un objeto.
  * Las clases que extiendan de TrackObject deberán implementar los métodos marcados como virtual.
  * El "tracking" implementa un filtro de Kalman y como tal serán necesarias dos funciones:
  * una de inicialización y otra de seguimiento (predicción, corrección y estimación).
  * El modelo del problema lo debe definir la clase que hereda.
  *
  * Ha sido de gran ayuda el siguiente ejemplo de aplicación:
  * <a href="http://www.marcad.com/cs584/Tracking.html">http://www.marcad.com/cs584/Tracking.html</a>
  *
  * \file trackobject.cpp
  *
  * \author José Manuel Cabrera Canal
  * \version 1.0
  */
class TrackObject
{
public:
    /*!
      * Constructor por defecto.
      */
    TrackObject();
    /*!
      * Destructor.
      */
    virtual ~TrackObject();

    /*!
      * La inicialización consiste en la construcción de una matriz
      * de transición del modelo dinámico (T), un matriz de covarianza
      * que representa el ruido del proceso (Q), la matriz de covarianza
      * para la medición (R) y la matriz de transición de la medición.
      * \param dymParams Dimensión del vector de estado.
      * \param measureParams Dimensión del vector de medición.
      * \param controlParams Parámetros de control, por defecto a 0.
      * \param T Matriz de transición.
      */
    virtual void initStates(const int dymParams, const int measureParams, const int controlParams, const Mat& T) = 0;
    /*!
      * Método que debe implementar toda clase que herede de TrackObject.
      * Especifica la forma mediante la cual se realiza el seguimiento o
      * "trackeo" del objeto. En nuestro caso, se produce una inicialización
      * del filtro de Kalman en el constructor y este método simplemente
      * actualiza la posición actual y corrige la predicción realizada por
      * el filtro.
      * El filtro de Kalman utiliza un modelo de sistema dinámico, como por
      * ejemplo, en nuestro caso, leyes físicas (posición, velocidad y
      * aceleración).
      *
      * \param img Región donde se encuentra el objeto bajo seguimiento.
      */
    virtual void trackObject(Mat& img) = 0;

protected:
    Mat_<float> m_state; /*!< Estado actual del sistema. */
    Mat m_process_noise; /*!< Ruido del proceso. */
    Mat_<float> m_measurement; /*!< Matriz de medición. */
    Mat m_measurement_noise; /*!< Ruido de la medición. */
    KalmanFilter m_kf; /*!< Filtro de Kalman. Librería OpenCV. */
};

#endif // TRACKOBJECT_HPP
