#ifndef GUICONTROLLER_HPP
#define GUICONTROLLER_HPP

#include <QMainWindow>
#include <QTimer>
#include "libfreenect.hpp"
#include "btcomm.hpp"
#include "mykinect.hpp"
#include "handdetection.hpp"
#include "utils.hpp"

/*!
  * \class GUIController
  * \brief Capa media para la gestión de eventos y ejecución de las acciones asociadas a estos.
  * Clase que gestiona las peticiones de la interfaz y las traduce en
  * peticiones a la aplicación, como pueden ser la conexión via bluetooth, iniciar
  * dispositivo, procesar un frame, calibrar la distancia, etc.
  *
  * \file guicontroller.cpp
  *
  * \author José Manuel Cabrera Canal
  * \version 1.0
  */
class GUIController
{
public:
    Freenect::Freenect freenect;/*!< Objeto que representa la biblioteca libfreenect, la cual proporciona funciones básicas de acceso al dispositivo. */
    MyFreenectDevice* device;/*!< Objeto que representa un dispositivo Kinect. */
    Mat* depthMat;/*!< Matriz con datos "puros (raw)" de profundidad (valores uint16_t entre 0 y 2048) */
    Mat* depthf; /*!< Matriz filtrada convertida a un formato de 8 bits, con la consecuente perdida de precision. */
    Mat* rgbMat; /*!< Imagen de la camara. Solo se muestra como "decoracion" realmente. */
    Mat* img_bw;/*!< Imagen en blanco y negro con el límite puesto en función de la profundidad */
    Mat* dst; /*!< Aqui es donde vamos a pintar los resultados. */
    BTComm* btc1;
    Point* cDistance;

    /*!
      * Constructor por defecto. Inicializa variables.
      */
    GUIController();
    /*!
      * Destructor. Libera la memoria asociada a los objetos creados en el constructor.
      */
    ~GUIController();
    /*!
      * \return Matriz de datos "puros" o "brutos". Representa los datos de profundidad sin normalizar.
      */
    Mat& getDepthMat();
    /*!
      * \return Matriz de profundidad normalizada. Representa una imagen en escala de grises (valores de 0 a 255).
      */
    Mat& getDepthF();
    /*!
      * \return Matriz de color. Representa la imagen a color obtenida de la cámara.
      */
    Mat& getRGBMat();
    /*!
      * \return Matriz con dos posibles valores: 0 ó 1. Representa una imagen segmentada, en blanco y negro.
      */
    Mat& getImgBW();
    /*!
      * \return Matriz auxiliar, utilizada para diversos cálculos.
      */
    Mat& getDst();
    /*!
      * Procesa un frame, es decir, realiza una segmentación por profunidad
      * y por color, detecta la posición de las manos y de los dedos y actualiza
      * el estado de la marcha y del giro, que más tarde se enviarán.
      *
      * \return Si el frame se ha procesado con éxito en todas sus etapas.
      */
    bool process();
    /*!
      * Indica al Kinect que tiene que subir su ángulo de visión 1 grado.
      */
    void moveUp();
    /*!
      * Indica al Kinect que tiene que bajar su ángulo de visión 1 grado.
      */
    void moveDown();
    /*!
      * Arranca el dispositivo, esto es, inicia los flujos de
      * vídeo y del sensor de infrarrojos.
      */
    void startDevice();
    /*!
      * Da un tiempo al usuario para que se sitúe a una distancia
      * del Kinect y, posteriormente, poder filtrar todos los objetos
      * que están a partir de su persona.
      */
    void calibrar();
    /*!
      * Envía una petición de conexión al vehículo. Véase #C_TIME
      */
    int connect();
    /*!
      * \return Devuelve el objeto correspondiente al análisis y detección de las manos,
      *         lo que nos permite tener acceso a todas las funcionalidades de este módulo.
      */
    HandDetection& getHDObject();


private:
    HandDetection* ht; /*!< Objeto que engloba toda la funcionalidad relativa al análisis y detección de manos y dedos. */
    uint8_t umbral; /*!< Establece hasta qué profundidad en metros aparecen los objetos en nuestra pantalla. */
};

#endif // GUICONTROLLER_HPP
