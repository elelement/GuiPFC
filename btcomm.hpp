#ifndef BTCOMM_HPP
#define	BTCOMM_HPP

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

//using namespace std;
class BTComm {
    //Solo va a enviar datos al brick de lego, no tiene nada que recibir de él.
    //vamos que en realidad el pc funciona como un cliente que envía datos y el
    //brick es el servidor que los recibe, interpreta y ejecuta.
public:
    struct sockaddr_rc server_addr ;
    int sock;
    
    BTComm();
    
    ~BTComm();
    
    int create_socket();
    int init_bluetooth(char *btAddress);
    bool isConnected();
    int connect_nxt();
    int disconnect_nxt();
    
    int send_message(char *message, int length);
    int send_int(int a);
    
    int read_data();
    char* read_buffer(char* buffer);
    
    int close_connection();
    
private:
    char *dest;
    socklen_t opt;
    char *bans;
    
    void clean();
};

#endif	/* BTCOMM_HPP */
