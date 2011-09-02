#include "libfreenect.hpp"
#include "btcomm.hpp"
#include "mykinect.hpp"
#include "handdetection.hpp"
#include "guiapp.hpp"
#include <iostream>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <cmath>
#include <exception>
#include <pthread.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <QApplication>

using namespace std;
using namespace cv;

int main(int argc, char **argv) {

    QApplication a(argc, argv);
    GuiApp w;
    w.show();

//    int num_frames = 0;

//    HandTracking ht;
    
//    /*
//     * Conectamos
//     */
////    int limite;
////    BTComm btc1; //*usbc
////    char *dest;
////    dest = (char *) malloc(sizeof (char) * 18);
////    bzero(dest, 18);
////    //La MAC del dispositivo bluetooth del brick de lego
////    strncpy(dest, "00:16:53:0F:1A:2A", 18);
////    if(btc1.init_bluetooth(dest) < 0){
////        close(btc1.sock);
////        return 1;
////    }
////    printf("Bluetooth conectado a %s \n", dest);
////

//    //IF COMENZAR = TRUE ENTONCES TODO LO DE ABAJO
//    //usar el DELETE
//    /*
//     * Inicializamos las matrices que contendran las imagenes.
//     */
//    Mat depthMat(Size(640, 480), CV_16UC1);
//    Mat depthf(Size(640, 480), CV_8UC1);
//    Mat rgbMat(Size(640,480),CV_8UC3,Scalar(0));
//    Mat ownMat(Size(640,480),CV_8UC3,Scalar(0));

//    Freenect::Freenect freenect;
//    MyFreenectDevice& device = freenect.createDevice<MyFreenectDevice > (0);

//    //namedWindow("rgb",CV_WINDOW_AUTOSIZE);
////    namedWindow("depth", CV_WINDOW_AUTOSIZE);
//    device.startVideo();
//    device.startDepth();
    
////    limite = 0;
    
//    //Usar qt
//    unsigned char k = 0;
    
    
//    //Recuadro imagen profundidad
    
//    //Controles del kinect
    
//    //Boton iniciar calibracion
    
//    //Asociar eventos
    
//    /*
//     * Muy importante realizar un programa de calibracion inicial y guardar los valores en un
//     * fichero.
//     */
    
//    while (k != 27) {//while no pulsada boton de salir
//        /*
//         * Tanto para el primer como el BEWTEEN_FRAMES frame, sacamos la posicion inicial de las manos,
//         * esto es, la asignacion de lastHandI, lastHandR, actualHandI y actualHandR.
//         *
//         * Por cada imagen, se binariza y se obtienen los contornos con la funcion
//         * findContours proporcionada por openCV.
//         *
//         * Despues, se saca la posicion de ambas manos y se calcula el angulo de giro, la velocidad
//         * de este y la profundidad en cada mano.
//         *
//         * Despues se envian las ordenes correspondientes por BT a cada servomotor, en funcion de
//         * los valores calculados previamente.
//         *
//         */
//        if (num_frames % 2 == 0) {//24
//            device.getVideo(rgbMat);
//            //Obtenemos la imagen de profundidad, frame a frame
//            device.getDepth(depthMat);
////            num_frames;
            
//            //Rango de alcance del Kinect basarlo en una constante q se defina al calibrar
//            Mat img_bw = Mat(Size(640, 480), CV_8UC1);
//            depthMat.convertTo(depthf, CV_8UC1, 255.0 / 2048.0); //20487 Cuanto mayor sea, mayor es el rango 2050
////            //Opening
//            erode(depthf,depthf,Mat(),Point(-1,-1),10);
//            dilate(depthf,depthf,Mat(),Point(-1,-1),10);
////            //Binarizamos con un umbral 100
//            threshold(depthf, img_bw, 100, 255, CV_THRESH_BINARY); //150

//            /*Region de interes*/
//            vector<vector<Point> > contornos;
//            vector<Vec4i> hierarchy;
////
//            Mat aux = img_bw.clone();
//            //Buscamos los contornos de los bordes
//            findContours(aux, contornos, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
//            ht.searchHands(contornos);
//            //Nos quedamos con los pixeles mas cercanos
            
//          /*Reducimos el area de busqueda para el centro de gravedad*/
//            Rect alh = ht.getActualLeftHand();
//            if(alh.x >= 0 && alh.y >= 0)
//                alh = Rect(alh.x+alh.width-70, alh.y, 70, 70);//hacerla mas ancha
//            Rect arh = ht.getActualRightHand();
//            if(arh.x >= 0 && arh.y >= 0)
//                arh = Rect(arh.x, arh.y, 70, 70);

////            La profundidad minima es el valor mas cercano
////            int minL = 255;
////            int minR = 255;
////            for (int i = alh.x; i < alh.x+alh.width; i++) {
////                for (int j = alh.y; j < alh.y+alh.y; j++) {
//////                    img_bw.at <uchar> (i,j) = depthf.at <uchar> (i,j);
////                    if(depthf.at <uchar> (i,j) <= minL){
////                        minL = depthf.at <uchar> (i,j);
////                    }
////                }
////            }
////            for (int i = arh.x; i < arh.x+arh.width; i++) {
////                for (int j = arh.y; j < arh.y+arh.y; j++) {
//////                    img_bw.at <uchar> (i,j) = depthf.at <uchar> (i,j);
////                    if(depthf.at <uchar> (i,j) <= minR){
////                        minR = depthf.at <uchar> (i,j);
////                    }
////                }
////            }
////
//            Point p1 = ht.centerOfMass(alh, depthf);
//            Point p2 = ht.centerOfMass(arh, depthf);//truco: expandir un rectangulo con centro ahi
////
////            rectangle(depthf, alh, Scalar(59,58,54), 2);
////            rectangle(depthf, arh, Scalar(59,58,54), 2);
//            circle(img_bw, Point(p1.x,p1.y), 5, Scalar(135, 206, 250), 8, -1);
//            circle(img_bw, Point(p2.x,p2.y), 5, Scalar(135, 206, 250), 8, -1);

            
////            /*
////             * Estableces el limite de parada del robot, a partir del cual, el
////             * robot no avanzara.
////             */
////            limite = minL;
////
////            /*
////             * Determinamos las ordenes y movemos el vehiculo.
////             * Se envia todo via bluetooth.
////             */
//            int giro = ht.getHandsAngle(p1,p2);
//            if(giro >= 60){
//                cv::imwrite("cog.png",img_bw);
//            }
//            printf("Giro %i",giro);
//////            int giro = ht.getTurnAngle();

//////
////            char *msg = (char *) malloc(sizeof (char) * espacio+4);
////            bzero(msg, espacio+4);
//////            msg[0]=(char)giro;
////            sprintf(msg, "*%i*.%d.", limite, giro);//enviar profundidad y hacer pruebas
            
////            printf("\nGiro final: %s\n", msg);;
////            printf("Prof. mano izqda: %i\n", nivelL);
////            printf("Prof. mano izqda: %i\n", nivelR);
////            printf("Limite: %i\n", limite);

///*********************/
///*INICIO ENVIO ORDENES            */
///***********************/
////            int c = 0;
////            int giro = 0;
////
////            struct termios org_opts, new_opts;
////            int res = 0;
////            //-----  store old settings -----------
////            res = tcgetattr(STDIN_FILENO, &org_opts);
////            assert(res == 0);
////            //---- set new terminal parms --------
////            memcpy(&new_opts, &org_opts, sizeof (new_opts));
////            new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
////            tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
////            c = getchar();
////            //------  restore old settings ---------
////            res = tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
////            assert(res == 0);
////
////            printf("%02X ", c);
////
////            //arriba77 abajo73 izqda61 dcha64
////            if(c == 119){
////                c = 100;
////            }
////            if(c == 97){
////                giro = -60;
////            }
////            if(c == 100){
////                giro = 60;
////            }
////            if(c != 119 && c != 97 && c != 100){
////                c = 0;
////                giro = 0;
////            }
////            //EL CONTROL DEL ROBOT SE BASA EN ENVIAR SOLO CUANDO HAYA CAMBIOS
////            //NO CADA DOS POR TRES
////            int espacio = 5;
//////            espacio = espacio + 5;
////            if(giro < 0){
////                espacio = espacio + 1;
////            }
////            if (abs(giro) < 10) {
////                espacio = espacio + 1;
////            } else if (abs(giro) < 100) {
////                espacio = espacio + 2;
////            } else if (abs(giro) < 1000) {
////                espacio = espacio + 3;
////            }
////            espacio = espacio + 2;//los puntos
////            if(espacio < 10){
////                espacio = 10;
////            }
////            char *msg = (char *) malloc(sizeof (char) * espacio);
////            bzero(msg, espacio);
////
////            if (abs(c) < 10) {
////                sprintf(msg, "*00%i*", c);
////            } else if (abs(c) < 100) {
////                sprintf(msg, "*0%i*", c);
////            } else if (abs(c) < 1000) {
////                sprintf(msg, "*%i*", c);
////            }
////            sprintf(msg+5, ".%i.", giro);
//////            bzero(msg, espacio+2);
////
////
////            printf("Longitud %i", espacio);
////
////            btc1.send_message(msg,espacio);//enviamos el giro *45*.-135.
            
///*********************/
///*FIN ENVIO ORDENES            */
///***********************/
//            //SI SUELTAS LA TECLA, DEBERIAS MANDAR UN STOP GENERAL
////            sleep(1);

//w.showImage(img_bw, rgbMat);
//           // cv::imshow("depth", img_bw);
////
//            k = cv::waitKey(10) & 0xff;
//            if (k == 'q') {
//                //cvDestroyWindow("rgb");
//                cvDestroyWindow("depth");
//                printf("Quit\n");
//                device.stopDepth();
//                break;
//            }
//            if(k == 'w'){
//                device.setTiltDegrees(+10);
//            }
//            if(k == 's'){
//                device.setTiltDegrees(-10);
//            }
//            if(k == 's'){
//                device.setTiltDegrees(0);
//            }
////
////            if (k == 8) {
////                std::ostringstream file;
////                file << filename << i_snap << suffix;
////                //cv::imwrite(file.str(),rgbMat);
////                i_snap++;
////            }
////            //		if(iter >= 1000) break;
////            //		iter++;
//        }//fin del if
//    }//fin while

//    device.stopVideo();
//    device.stopDepth();


    return a.exec();
}
