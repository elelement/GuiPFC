#ifndef BTCOMM_HPP
#define	BTCOMM_HPP

/*! \header iostream <iostream> */
#include <iostream>
/*! \header sys/socket.h <sys/socket.h> */
#include <sys/socket.h>
/*! \header sys/types.h <sys/types.h> */
#include <sys/types.h>
/*! \header stdio.h <stdio.h> */
#include <stdio.h>
/*! \header bluetooth.h <bluetooth.h> */
#include <bluetooth/bluetooth.h>
/*! \header bluetooth/rfcomm.h <bluetooth/rfcomm.h> */
#include <bluetooth/rfcomm.h>

/*!
  * \class BTComm
  * \brief Comunicación via Bluetooth
  * Clase que gestiona la conexión bluetooth y el
  * envío de datos con el vehículo. No se reciben
  * datos pues sólo interesa enviar información.
  *
  * \file btcomm.cpp
  *
  * \author José Manuel Cabrera Canal
  * \version 1.0
  */
class BTComm {

public:

    /*!
      * Constructor de clase.
      */
    BTComm();

    /*!
      * Inicia una conexión entre el vehículo y el ordenador.
      * \param[in] btAddress Dirección MAC del dispositivo bluetooth del brick de Lego
      * \return Un valor positivo si la conexión se realiza con éxito y negativo en
                en caso contrario.
      */
    int init_bluetooth(char *btAddress);

    /*!
      * \return Si hay una conexión activa o no.
      */
    bool isConnected();

    /*!
      * Envía un mensaje del equipo al coche.
      * \param message El mensaje que se quiere enviar.
                        La trama enviada siempre es la misma: *vlc#ang@, donde
                        vlc es la velocidad del vehículo y ang el ángulo de giro.
      * \param length La longitud del mensaje.
      * \return Un valor positivo si se envía correctamente y negativo en otro caso.
      */
    int send_message(const char *message, int length);

    /*!
      * Cierra la conexión.
      * \return Un valor positivo si se envía correctamente y negativo en otro caso.
      */
    int close_connection();

private:
    struct sockaddr_rc m_server_addr ; /*!< Estructura de direccionamiento para bluetooth. La utiliza el equipo. */
    int m_sock;/*!< Descriptor del socket.*/
};



#endif	/* BTCOMM_HPP */
