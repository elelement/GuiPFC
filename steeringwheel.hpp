#ifndef STEERINGWHEEL_HPP
#define STEERINGWHEEL_HPP

#include <opencv/cv.h>
#include <opencv/cv.hpp>

#include "guiappheaders.hpp"
#include "utils.hpp"

/*! \namespace std */
using namespace std;
/*! \namespace cv */
using namespace cv;

/*!
  * \class SteeringWheel
  * \brief Objeto gráfico que representa un volante.
  * Nos permitirá mostrar un volante con en pantalla
  * con efecto gráfico de giro a la vez que la mano.
  *
  * \file steeringwheel.cpp
  *
  * \author José Manuel Cabrera Canal
  * \version 1.0
  */
class SteeringWheel
{

public:
    /*!
      * Constructor por defecto.
      */
    SteeringWheel();
    /*!
      * Constructor de clase. Sitúa un volante en la posición
      * que le indiquemos por parámetro y con los brazos que
      * queramos.
      *
      * \param center Centro del volante.
      * \param radius Radio exterior del volante.
      * \param alpha Ángulo que forma el primer brazo con respecto al centro del volante. Los demás se situarán en base a esto.
      * \param arms Número de brazos internos que unen el centro con el exterior del volante.
      */
    SteeringWheel(Point const& center, double const& radius, double const& alpha, int const& arms);
    /*!
      * Destructor de clase.
      */
    ~SteeringWheel();

    /*!
      * Actualiza el estado del volante y dibuja una línea
      * desde el centro del mismo a la posición de la mano
      * (indicada por un punto).
      *
      * \param dst Imagen sobre la cual actualizaremos el estado del volante.
      * \param hand Punto que indica la posición de la mano.
      */
    void draw(Mat& dst, Point& hand);

    /*!
      * \return El centro del volante.
      */
    Point* const& getCenter();
    /*!
      * \return La última posición en la que estuvo el volante (con respecto a la mano que lo controla).
      */
    Point* const& getLast();
    /*!
      * \return El radio externo del volante.
      */
    double const& getRadius();
    /*!
      * \return El ángulo que forma el primer radio o brazo del volante con el centro de este.
      */
    double const& getAlpha();
    /*!
      * \return El número de brazos o radios del volante. Son equidistantes en radianes.
      */
    int const& getArms();

    /*!
      * Cambia de posición el volante.
      * \param center Nueva posición.
      */
    void setCenter(Point const& center);
    /*!
      * Permite cambiar de tamaño el volante.
      * \param radius Nuevo radio externo (corona externa) del volante.
      */
    void setRadius(double const& radius);
    /*!
      * Permite cambiar el ángulo de partida que formarán el primer radio o brazo con respecto del centro.
      * \param alpha Ángulo entre el radio y el centro.
      */
    void setAlpha(double const& alpha);
    /*!
      * Asigna un nuevo número de brazos o radios, que se dibujarán a una distancia equidistante entre ellos.
      * \param arms Número de radios.
      */
    void setArms(int const& arms);

private:
    Point* m_center; /*!< Centro del volante. */
    Point* m_last; /*!< Última posición de la mano con respecto del volante. */
    double m_radius; /*!< Radio de la corona externa del volante. */
    double m_alpha; /*!< Ángulo formado por el primer brazo del volante con respecto al centro de este. */
    int m_arms; /*!< Número de brazos o radios. */
};

#endif // STEERINGWHEEL_HPP
