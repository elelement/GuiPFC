#ifndef HAND_HPP
#define HAND_HPP

#include <iostream>
#include <vector>
#include <stdio.h>
#include <cmath>

#include <opencv/cv.h>
#include <opencv/cv.hpp>

#include "guiappheaders.hpp"
#include "utils.hpp"
#include "trackobject.hpp"

/*! \namespace cv */
using namespace cv;
/*! \namespace std */
using namespace std;

/*!
  * \class Hand
  * \brief Clase que representa una mano humana.
  * La clase mano permite hacer un "tracking" o
  * seguimiento por cada objeto instanciado, basado
  * en un filtro de Kalman.
  * Registra la posición del centro de mano (generalmente)
  * la palma, los puntos del contorno, si es la mano derecha
  * o izquierda y a qué profundidad se encuentra el punto más
  * cercano. También permite almacenar el último número de
  * dedos que se encontraban extendidos.
  *
  * Hereda de la clase TrackObject.
  *
  * \file hand.cpp
  *
  * \author José Manuel Cabrera Canal
  * \version 1.0
  */
class Hand:public TrackObject {//que herede de track object
public:
    /*!
      * Constructor por defecto.
      */
    Hand();
    /*!
      * Constructor de copia.
      */
    Hand(const Hand& hand);
    /*!
      * Destructor de clase.
      */
    ~Hand();

    /*!
      * \return El número de dedos abiertos en el instante en que se llama a esta función.
      */
    int getFingers() const;
    /*!
      * \return Un vector con los puntos del contorno de la mano.
      */
    vector<Point>* getContour() const;
    /*!
      * \return Valor entero que representa la profundidad a la que se encuentra la mano.
      */
    uint8_t getDepth() const;
    /*!
      * \return Si se trata de la mano izquierda o de la mano derecha.
      */
    int getWhich() const;
    /*!
      * \return El centro de la mano (un punto).
      */
    Point& getCenter() const;

    /*!
      * Asigna un nuevo vector de puntos para el contorno de la mano.
      * \param contour Vector que sobreescribirá al anteriormente asignado.
      */
    void setContour(vector<Point>& contour);
    /*!
      * Asigna un nuevo valor de profundidad (un valor entre 0 y 255).
      * \param depth Valor de profundidad nuevo.
      */
    void setDepth(uint8_t depth);
    /*!
      * Cambia el número de dedos abiertos para esta mano.
      * \param fingers Número de dedos.
      */
    void setFingers(int fingers);
    /*!
      * Asigna un nuevo punto para el centro de la mano.
      * \param center Nuevo centro de la mano.
      */
    void setCenter(Point& center);
    /*!
      * Cambia el tipo de la mano (izquierda o derecha).
      * \param which Un entero que indica el tipo de mano. Véase #LEFT_HAND y #RIGHT_HAND
      */
    void setWhich(int which);

    /*!
      * Asigna un nuevo punto para el centro de la mano.
      * \param center Nuevo centro de la mano.
      */
    double getArea() const;

    //Debe implementar este metodo
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
    void trackObject(Mat &img);
    /*!
      * Inicializa el filtro de Kalman.
      * \param dymParams Dimensión del vector de estado (posición + velocidad, teniendo en cuenta que a = 0)
      * \param measureParams Dimensión del vector de medición (la verdadera posición (un punto)).
      * \param controlParams Parámetros de control, por defecto a 0.
      * \param T Matriz de transición.
      */
    void initStates(const int dymParams, const int measureParams, const int controlParams, const Mat& T);

    /*!
      * \return La matriz de predicción.
      */
    const Mat getPredicted();
    /*!
      * \return El estado o matriz medido.
      */
    const Mat getMeasured();
    /*!
      * \return Estado estimado por el filtro.
      */
    const Mat getEstimated();
    /*!
      * \return El siguiente punto estimado por el filtro de Kalman en base a la medida y a la corrección.
      */
    Point& getEstimatedPoint();

private:
    Point m_estimated; /*!< Punto estimado por el filtro de Kalman en base a la medida y a la corrección. */
    vector<Point>* m_contour; /*!< Vector de puntos para identificar el contorno de la mano.*/
    Point* m_center; /*!< Centro de la mano. En nuestro caso se corresopnde con el centro del polígono formado por el contorno. */
    int m_fingers; /*!< Número de dedos que reconocibles (que están extendidos). */
    uint8_t m_depth; /*!< Profundidad a la que se encuentra la mano. Valor entre 0 y 255. */
    int m_which; /*!< Indica si se trata de la mano derecha o de la mano izquierda. */
};

#endif // HAND_HPP
