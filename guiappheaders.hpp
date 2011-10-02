#ifndef GUIAPPHEADERS_HPP
#define GUIAPPHEADERS_HPP

#define ROWS 480 /*! \def ROWS número de filas de la imagen principal. */
#define COLS 640/*! \def COLS número de columnas de la imagen principal. */
#define C_TIME 3000 /*! \def C_TIME Tiempo de espera para que el usuario se coloque frente al Kinect antes de realizar la segmentación por profundidad. */
#define REFRESH_TIMER 80 /*! \def REFRESH_TIMER Tiempo de refresco de la imagen. Se solicitan datos a Kinect cada 80 ms. */
#define SEND_TIME 300 /*! \def SEND_TIME Ciclo de tiempo de envío de datos. */
#define MIN_DEPTH 0 /*! \def MIN_DEPTH Representa un valor de sombra en la imagen normalizada. */
#define MAX_DEPTH 255 /*! \def MAX_DEPTH En la imagen normalizada, valor máximo que puede tener un punto (256 niveles de gris). */
#define MAX_DEPTH_16B 2047 /*! \def MAX_DEPTH_16B Rango máximo de la matriz de datos brutos recibidos del Kinect. Es el valor máximo y representa los objetos más lejanos detectados. */
#define APPROX_POLY_SIDES 7 /*! \def APPROX_POLY_SIDES Número de lados del polígono al cual queremos aproximarnos en el cálculo del polígono convexo. */
#define DISTANCE_THRESHOLD 75 /*! \def ROWS Valor empírico que permite calcular a qué distancia está un dedo de la palma de la mano. */
#define LEFT_HAND 0 /*! \def LEFT_HAND Entero que representa la mano izquierda. */
#define RIGHT_HAND 1 /*! \def RIGHT_HAND Entero que representa la mano derecha. */
#define DEPTH_RANGE 700 /*! \def Valor empírico utilizado para normalizar los datos y extraer una imagen normalizada detallada, que permite ver un modelo 3D con suficiente profundidad. Válido para objetos cercanos al Kinect. */
#define MARCHA_ADELANTE 1 /*! \def MARCHA_ADELANTE Indica que el vehículo se encuentra moviéndose hacia adelante. */
#define MARCHA_ATRAS -1 /*! \def MARCHA_ATRAS Indica que el vehículo se encuentra moviéndose hacia atrás. */


#endif // GUIAPPHEADERS_HPP
