/*! \headerfile iostream <iostream> */
#include "btcomm.hpp"

BTComm::BTComm() {
    m_sock = -1;
}

bool BTComm::isConnected(){
    return m_sock >= 0;
}

int BTComm::init_bluetooth(char* btAddress){
    struct sockaddr_rc addr;// = {0};
    int status = -1;

    //Alojamos un socket
    if((m_sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM)) >= 0){
        //Establecemos el tipo y a quien nos queremos conectar
        addr.rc_family = AF_BLUETOOTH;
        addr.rc_channel = (uint8_t) 1;
        str2ba(btAddress, &addr.rc_bdaddr);

        //Conectamos al robotin
        printf("por lo menos el socket se crea");
        status = connect(m_sock, (struct sockaddr *)&addr, sizeof(addr));

    }
    return status;
}


int BTComm::send_message(const char *message, int length) {
    //Si la conexion se realizo con exito
    return send(m_sock, message, sizeof(char)*length, MSG_DONTWAIT);
}

int BTComm::close_connection() {
    return close(m_sock);
}

