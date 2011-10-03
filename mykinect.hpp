/* 
 * File:   mykinect.hpp
 * Author: cabreracanal
 *
 * Created on 26 de febrero de 2011, 13:51
 */

#ifndef MYKINECT_HPP
#define	MYKINECT_HPP
#define	 FRAMES_BETWEEN 24 // determina el num. de frames entre el 
                                    // 1er capturado y el ultimo.

#include "libfreenect.hpp"
#include "btcomm.hpp"
#include <iostream>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <cmath>
#include <exception>
#include <pthread.h>


#include <opencv/cv.h>
#include <opencv/cv.hpp>
#include <opencv/cxcore.h>
#include <opencv/cxcore.hpp>
#include <opencv/highgui.h>
#include <opencv/highgui.hpp>
#include <opencv/cxmat.hpp>

/*! \namespace std */
using namespace std;
/*! \namespace cv */
using namespace cv;


/*!
  * \class Mutex
  * \brief Representa un mutex (semáforo) cualquiera.
  * Clase que encapsula el comportamiento de un mutex.
  * proporciona funcionalidad para realizar bloquear el
  * acceso a un recurso y viceversa (lock y unlock).
  * Necesaria para el acceso a los datos del Kinect en
  * tiempo real.
  *
  * \file mykinect.hpp
  *
  * \author José Manuel Cabrera Canal
  * \version 1.0
  */
class Mutex {
public:
    /*!
      * Constructor de clase.
      */
    Mutex();
    /*!
      * Función lock
      */
    void lock();
    /*!
      * Unlock
      */
    void unlock();
private:
    pthread_mutex_t m_mutex;/*!< El mutex es en realidad un objeto de tipo pthread_mutex_t. */
};

class Mutex;

/*!
  * \class MyFreenectDevice
  * \brief Extensión de la clase #FreenctDevice, da acceso al vídeo y a los datos de profundidad.
  *
  * Representa al dispositivo Kinect, hereda de FreenectDevice
  * e implemetna los métodos de acceso a vídeo y profundidad.
  *
  * \file mykinect.hpp
  *
  * \author José Manuel Cabrera Canal
  * \version 1.0
  */
class MyFreenectDevice : public Freenect::FreenectDevice {
public:
    ///Atributos
    double m_degrees; /*!< Ángulo de inclinación del dispositivo. */

    ///Métodos
    /*!
      * Constructor de clase.
      */
    MyFreenectDevice(freenect_context *_ctx, int _index);
    /*!
      * Destructor de clase.
      */
    ~MyFreenectDevice();

    /*!
      * Protege el acceso al recurso de vídeo mientras
      * extrae los datos de la cámara de color.
      * \param _rgb Vector destino con la información de color.
      */
    void VideoCallback(void* _rgb, uint32_t timestamp);
    /*!
      * Protege el acceso al sensor de infrarrojos mientras
      * extrae los datos de la cámara de color.
      * \param _depth Vector destino con la información de profundidad (datos en bruto).
      */
    void DepthCallback(void* _depth, uint32_t timestamp);

    /*!
      * Devuelve los datos de color en una matriz, de forma que resulte
      * mucho más cómodo de trabajar con ella.
      * \param output Matriz destino.
      * \return true si la operación se ha realizado con éxito, false en otro caso.
      */
    bool getVideo(Mat& output);
    /*!
      * Devuelve los datos de profundidad en una matriz, de forma que resulte
      * mucho más cómodo de trabajar con ella.
      * \param output Matriz destino.
      * \return true si la operación se ha realizado con éxito, false en otro caso.
      */
    bool getDepth(Mat& output);

private:
    std::vector<uint8_t> m_buffer_depth; /*!< Representación de los datos de profundidad en 11 bits (escala de grises). */
    std::vector<uint8_t> m_buffer_rgb; /*!< Buffer de la imagen de color. Cada posición es un color. */
    std::vector<uint16_t> m_gamma; /*!< Buffer de datos en bruto. Cada posición puede tomar un valor entre 0 y 2048. */
    bool m_new_rgb_frame; /*!< Indica si se acaba de recuperar un nuevo frame de color. */
    bool m_new_depth_frame; /*!< Indica si se acaba de recuperar un nuevo frame de datos en brutos (a partir, del sensor de infrarrojos). */
    Mat m_depth_mat; /*!< Matriz de profundidad. Trabaja con datos en bruto. */
    Mat m_rgb_mat; /*!< Matriz de color. */
    Mat m_own_mat; /*!< Matriz auxiliar. */
    Mutex m_rgb_mutex; /*!< Cerrojo para proteger el acceso a la cámara de color. */
    Mutex m_depth_mutex; /*!< Cerrojo para proteger el acceso al dispositivo de infrarrojos. */
};

#endif	/* MYKINECT_HPP */

