#ifndef GUIAPP_HPP
#define GUIAPP_HPP

#include <QMainWindow>
#include <QTimer>
#include <QFuture>
#include <QtConcurrentRun>
#include <time.h>
#include <stdint.h>
#include <QGraphicsPixmapItem>
#include "libfreenect.hpp"
#include "btcomm.hpp"
#include "mykinect.hpp"
#include "handdetection.hpp"
#include "guicontroller.hpp"
#include "steeringwheel.hpp"
#include "utils.hpp"
#include "glwidget.hpp"
#include "ui_guiapp.h"
#include "guiappheaders.hpp"

/*! \namespace Ui */
namespace Ui {
    class GuiApp;
}

/*!
  * \class GuiApp
  * \brief Clase principal. Punto de entrada y salida de la aplicación.
  * Crea todos los hilos de ejecución y controla en todo momento
  * qué se está ejecutando. Contiene a nivel muy abstracto toda la lógica
  * del programa.
  *
  * \file guiapp.cpp
  *
  * \author José Manuel Cabrera Canal
  * \version 1.0
  */
class GuiApp : public QMainWindow
{
    Q_OBJECT
/*! Funciones que se ejecutan en base a un evento o un temporizador. */
public slots:
    /*!
      * Indica que ha finalizado la calibración y cambia
      * a modo WORK, en el que ya se puede controlar el
      * vehículo.
      */
    void uncalibrate();
    /*!
      * Pasa a GuiController la petición para que realice
      * la calibración de la imagen en base a la profundidad
      * del sujeto.
      */
    void calibrateKinect();
    /*!
      * Permite elevar el punto de mira del Kinect en
      * 1 grado.
      */
    void kinectUp();
    /*!
      * Permite bajar el punto de mira del Kinect en
      * 1 grado.
      */
    void kinectDown();
    /*!
      * Se ejecuta periódicamente: captura la información
      * de color y de profundidad del Kinect y en base al
      * modo de operación realiza una operación u otra.
      * Existen dos operaciones: CALIBRATION y WORK (STOP
      * no se incluyó finalmente).
      */
    void refresh();
    /*!
      * Como uncalibrate, cambia a modo "trabajo", es decir
      * el modo en el que se analiza la imagen recibida, se
      * extraen los gestos y se construye el mensaje a enviar
      * al robot.
      */
    void switch2WorkMode();
    /*!
      * Delega en GuiController la llamada al objeto bluetooth
      * para conectar el equipo al vehículo.
      */
    void connectBT();
    /*!
      * Delega en GuiController la llamada al objeto bluetooth
      * para enviar un mensaje del equipo al vehículo.
      */
    void sendBTMessage();

public:
    GUIController* m_gc; /*!< Descriptor del socket.*/

    /*!
      * Constructor de clase. Construye la interfaz sobre
      * un widget.
      */
    explicit GuiApp(QWidget *parent = 0);
    ~GuiApp();
    /*!
      * Define qué imágenes se mostrarán en el widget.
      * \param big Imagen más grande, que ocupa el centro
                    de la interfaz.
      * \param small Imagen más pequeña, se muestra en un
               pequeño recuadro, en la esquina inferior izquierda.
      */
    void showImage(const cv::Mat& big, const cv::Mat& small);
    /*!
      * Define la imagen pequeña.
      * \param small Imagen más pequeña, se muestra en un
               pequeño recuadro, en la esquina inferior izquierda.
      */
    void showSmallImage(cv::Mat& small);
    /*!
      * Define la imagen principal.
      * \param big Imagen más grande, que ocupa el centro
                    de la interfaz.
      */
    void showBigImage(cv::Mat& big);
    /*!
      * Evento que se ejecuta al cerrar la aplicación.
      * Libera memoria y cierra el programa.
      */
    void closeEvent(QCloseEvent *event);
    /*!
      * Eventos genéricos.
      */
    QMenu* createPopupMenu();
    void contextMenuEvent(QContextMenuEvent *event);
    bool event(QEvent *event);

private:
    Ui::GuiApp *m_ui;/*!< Descriptor del socket.*/
    SteeringWheel* m_wheel;/*!< Objeto gráfico que representa el volante dibujado en pantalla.*/
    bool m_mano_anterior_abierta;/*!< Indica si en el frame anterior, nuestra mano derecha estaba abierta.*/
    bool m_initiated;/*!< Controla que tanto la nube de puntos como la primera conexión por bluetooth, se hagan al principio de la ejecución. */
    int m_sentido;/*!< Sentido de la marcha del vehículo. #MARCHA_ADELANTE #MARCHA_ATRAS.*/
    QTimer* m_timer, m_calib_timer;/*!< Temporizadores. Controlan cuándo se ejecuta la calibración y cuando el modo "trabajo".*/
    QTimer* m_send_timer;/*!< Controla el envío periódico de mensajes al vehículo. Ver #SEND_TIME.*/
    enum operation_mode_t {CALIBRATION, WORK, STOP};/*!< Valores posibles del modo de operación de la aplicación. */
    enum operation_mode_t m_operation_mode;/*!< Modo de operación de la aplicación. */
    GLWidget *m_gl_widget;/*!< Widget donde aparecerá la nube de puntos. */

};

#endif // GUIAPP_HPP
