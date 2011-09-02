#include "btcomm.hpp"

BTComm::BTComm() {
    sock = -1;
    dest = 0;
    bans = 0;
    opt = 0;
//    bans = (char *) malloc(sizeof (char) * 3);//dos bytes de cabecera y el dato esperado
//    bzero(bans, 3);
//    dest = (char *) malloc(sizeof (char) * 18);
//    bzero(dest, 18);
//    //La MAC del dispositivo bluetooth del brick de lego
//    strncpy(dest, "00:16:53:0F:1A:2A", 18);
//
//    if (create_socket() < 0) {
//        printf("Failed to create BT socket.\n");
//    } else {
//        printf("Socket creado.\n");
//        server_addr.rc_family = AF_BLUETOOTH;
//        server_addr.rc_channel = (uint8_t) 1;
//        str2ba(dest, &server_addr.rc_bdaddr);
//        if (connect_nxt() < 0) {//CONTROLAR SI FALLA LA CONEXION REINTENTAR
//            printf("Error en la conexion\n");
//        } else {
//            printf("Conectado\n");
//        }
//    }
}

BTComm::~BTComm() {
//    disconnect_nxt();
    //clean();
}

bool BTComm::isConnected(){
    return sock >= 0;
}

int BTComm::init_bluetooth(char* btAddress){
    struct sockaddr_rc addr;// = {0};
    int status;

    //Alojamos un socket
    sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    
    //Establecemos el tipo y a quien nos queremos conectar
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba(btAddress, &addr.rc_bdaddr);
    
    //Conectamos al robotin
    status = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    if(status < 0) {
        perror("Error connecting BT");
        //return status;
    }
    return status;
}

int BTComm::create_socket() {
    sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if (sock < 0) {
        return -1;
    }
    return 0;
}

int BTComm::connect_nxt() {
    if (connect(sock, (struct sockaddr *) &server_addr, sizeof (server_addr)) < 0) {
        return -1;
    }
    return 0;
}

int BTComm::disconnect_nxt() {
    if (close_connection() < 0) {
        return -1;
    }
    return 0;
}

int BTComm::send_message(char *message, int length) {
    //Si la conexion se realizo con exito
    int ok = ::send(sock, message, sizeof(char)*length,0);
    
    if (ok < 0) {
        printf("No se pudo enviar el mensaje.\n"); //perror?            
        return -1;
    } else {
        printf("Message: %s  SENDED\n", message);
        return 0;
    }
}

int BTComm::send_int(int a) {
    //Si la conexion se realizo con exito
//    int tosend = htonl(a);
//    int ok = ::write(sock, &a, sizeof (int));
    if (::write(sock, &a, 4) < 0) {
        printf("No se pudo enviar el mensaje.\n"); //perror?            
        return -1;
    }
    return 0;
}

int BTComm::read_data() {
    int bread = recv(sock, bans, sizeof (bans),0);
    if (bread < 0) { 
        return -1;
    }
    printf("Received: ");
    for(int i=0; i<3; i++){
        printf("[%i]", bans[i]);
    }
    printf("\n");
    return 0;
}

char* BTComm::read_buffer(char* buffer){
    int bread = recv(sock, buffer, sizeof (bans),0);
    if (bread < 0) { 
        return NULL;
    }
    return bans;
}

int BTComm::close_connection() {
    if (close(sock) < 0) {
        return -1;
    }
    printf("Connection closed.\n");
    return 0;
}

void BTComm::clean() {
//    free(&server_addr);
//    free(&sock);
//    free(dest);
}
