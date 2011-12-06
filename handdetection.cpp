#include "handdetection.hpp"

HandDetection::HandDetection() {
    m_left_hand = new Hand();
    m_right_hand = new Hand();
    m_left_hand->setWhich(LEFT_HAND);
    m_right_hand->setWhich(RIGHT_HAND);
//    _rightHand->initStates(2,1,0, *(Mat_<float>(2, 2) << 1, 1, 0, 1));
    m_left_hand->initStates(6,1,0, *(Mat_<float>(6, 6) << 1,0,1,0,0.5,0, 0,1,0,1,0,0.5, 0,0,1,0,1,0, 0,0,0,1,0,1, 0,0,0,0,1,0, 0,0,0,0,0,1));
    m_right_hand->initStates(6,1,0, *(Mat_<float>(6, 6) << 1,0,1,0,0.5,0, 0,1,0,1,0,0.5, 0,0,1,0,1,0, 0,0,0,1,0,1, 0,0,0,0,1,0, 0,0,0,0,0,1));
}

HandDetection::~HandDetection() {
    delete(m_left_hand);
    delete(m_right_hand);
}

Hand* HandDetection::getHand(int hand) const{
    if(hand == LEFT_HAND){
        return m_left_hand;
    }
    return m_right_hand;
}

Hand* HandDetection::getLeftHand() const{
    return m_left_hand;
}

Hand* HandDetection::getRightHand() const{
    return m_right_hand;
}

//Wrapper de C a CPP
void HandDetection::findConvexityDefects(vector<Point>& contour, vector<int>& hull, vector<CvConvexityDefect>& convexDefects){
    if(hull.size() > 0 && contour.size() > 0){
        //Conversion de objetos CPP a C
        CvSeq* contourPoints;
        CvSeq* defects;
        CvMemStorage* storage;
        CvMemStorage* strDefects;
        CvMemStorage* contourStr;
        CvConvexityDefect *defectArray = 0;

        strDefects = cvCreateMemStorage();
        defects = cvCreateSeq( CV_SEQ_KIND_GENERIC|CV_32SC2, sizeof(CvSeq),sizeof(CvPoint), strDefects );

        //Empezamos con los contornos: los pasamos a un array de objetos Seq
        contourStr = cvCreateMemStorage();
        contourPoints = cvCreateSeq(CV_SEQ_KIND_GENERIC|CV_32SC2, sizeof(CvSeq), sizeof(CvPoint), contourStr);
        for(int i=0; i<(int)contour.size(); i++) {
            CvPoint cp = {contour[i].x,  contour[i].y};
            cvSeqPush(contourPoints, &cp);
        }

        //Ahora con los puntos del poligono convexo
        int count = (int)hull.size();
        //int hullK[count];
        int* hullK = (int*)malloc(count*sizeof(int));
        for(int i=0; i<count; i++){hullK[i] = hull.at(i);}
        CvMat hullMat = cvMat(1, count, CV_32SC1, hullK);

        //Inicializamos la salida
        storage = cvCreateMemStorage(0);
        defects = cvConvexityDefects(contourPoints, &hullMat, storage);
        defectArray = (CvConvexityDefect*)malloc(sizeof(CvConvexityDefect)*defects->total);
        cvCvtSeqToArray(defects, defectArray, CV_WHOLE_SEQ);
        //Conversion de C a CPP
        //float cutoff = lower - (lower - upper) * 0.3f;
        for(int i = 0; i<defects->total; i++){
            convexDefects.push_back(defectArray[i]);
        }
        //Liberar vectores con cvReleaseMemStorage
        //No olvides hacer un free
        //free(hullK);
        cvReleaseMemStorage(&contourStr);
        cvReleaseMemStorage(&strDefects);
        cvReleaseMemStorage(&storage);

        //Devolvemos el vector de puntos con los defectos de convexidad
        //return defects;
    }
    //return defectArray;
}

void HandDetection::detectHands(Mat& depthImage, Mat& color) {
    vector<Point> manoI, manoD;
    vector<vector<Point> > contornos;
    vector<Vec4i> hierarchy;
    vector<int> phull1, phull2;
    vector<CvConvexityDefect> fingerTips1, fingerTips2;
    vector<Point> curva1, curva2;
    Point ref(0, ROWS/2);
    vector<Point> palm2;
    int distancia1;// distancia2;
    distancia1 = 2000;
//    distancia2 = 2000;

    cv::findContours(depthImage, contornos, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    for(size_t i=0; i<contornos.size(); i++) {
        double aux = contourArea(Mat(contornos[i]));

        if(aux > 0 && aux >= 1000 && aux <= 35000) {//35000
//          Comparar cada blob en base a la distancia con su punto de "tracking"
            Rect r = boundingRect(Mat(contornos[i]));
            Point punto = Utils::getRectCenter(r);
            int d = (int)Utils::OXDistance(punto, ref);
            int aux1 = (int)Utils::OXDistance(punto, m_left_hand->getEstimatedPoint());
//            int aux2 = (int)Utils::OXDistance(punto, m_right_hand->getEstimatedPoint());
            bool isleft = false;
            if(d < COLS/2 ){//&& area > _leftHand->getArea()
                manoI = contornos[i];
                m_left_hand->setContour(manoI);
                m_left_hand->setCenter(punto);
                m_left_hand->setWhich(LEFT_HAND);
                isleft = true;
                distancia1 = aux1;
            }
            if(d >= COLS/2){
                manoD = contornos[i];
                m_right_hand->setContour(manoD);
                m_right_hand->setCenter(punto);
                m_right_hand->setWhich(RIGHT_HAND);
            }
        }
    }

    m_left_hand->trackObject(color);//tienes q ver que pasa si lefhand vale lo mismo q antes
//    m_right_hand->trackObject(color);
    Point p = m_right_hand->getCenter();
    circle(color, p, 10, Scalar(200,200,200), 5, -1);

    //Calculamos los poligonos de Hull (convex Hull)
    if(manoD.size() > 0){
        Rect r1 = boundingRect(Mat(manoD));
        rectangle(color, r1, Scalar(0,0,255), 3);
        Mat matriz1(manoD);
        if(matriz1.rows > 0 && matriz1.cols > 0){
            //Calculamos el poligono convexo que recubre la mano
            //Simplificamos, primero, el contorno.
            approxPolyDP(matriz1, curva1, APPROX_POLY_SIDES, true);//20
            Mat curvi(curva1);
            convexHull(curvi, phull1, CV_CLOCKWISE);
            vector<Point> poly;
            for(int i=0; i<(int)phull1.size(); i++){
                poly.push_back(curva1[phull1[i]]);
            }

            //Ahora dibujamos los defectos de convexidad
            vector<CvConvexityDefect> defects;
            findConvexityDefects(curva1, phull1, defects);//pasar curva2 a int*

            //De todos los start y end, te quedas con los que coincidan del poligono aproximado
            findFingerTips(curva1, defects, fingerTips1);
            int fingersCount = (int)fingerTips1.size();
            m_right_hand->setFingers(fingersCount);
        }
    }

    if(manoI.size() > 0){
        Rect r2 = boundingRect(Mat(manoI));
        rectangle(color, r2, Scalar(255,0,0), 3);
        Mat matriz2(manoI);
        if(matriz2.rows > 0 && matriz2.cols > 0){
            //Calculamos el poligono convexo que recubre la mano
            //Simplificamos, primero, el contorno.
            approxPolyDP(matriz2, curva2, APPROX_POLY_SIDES, true);//20
            Mat curvi(curva2);
            convexHull(curvi, phull2, CV_CLOCKWISE);
            vector<Point> poly;
            for(int i=0; i<(int)phull2.size(); i++){
                poly.push_back(curva2[phull2[i]]);
            }

            //Ahora dibujamos los defectos de convexidad
            vector<CvConvexityDefect> defects;
            findConvexityDefects(curva2, phull2, defects);//pasar curva2 a int*

            //De todos los start y end, te quedas con los que coincidan del poligono aproximado
            findFingerTips(curva2, defects, fingerTips2);

            //Analizamos la informacion de los supuestos dedos
            for(int i=0; i<(int)fingerTips2.size(); i++){
                CvPoint cvp1 = *(fingerTips2.at(i).start);
                CvPoint cvp2 = *(fingerTips2.at(i).end);
                CvPoint cvdef = *(fingerTips2.at(i).depth_point);
                Point p1(cvp1.x, cvp1.y);
                Point p2(cvp2.x, cvp2.y);
                Point p3(cvdef.x, cvdef.y);
                line(color, p1, p3, Scalar(0, 255, 0), 1, CV_AA);
                line(color, p2, p3, Scalar(0, 255, 0), 1, CV_AA);
                cv::circle(color, p1, 10, Scalar(200,200,200), -1, CV_AA);
                cv::circle(color, p3, 10, Scalar(0,0,0), 2, CV_AA);
                palm2.push_back(p3);
            }
            //Obtenemos el centro de la palma
//            if(palm2.size() > 0){
//                minEnclosingCircle(Mat(palm2), center2, radius2);
//                cv::circle(color, center2, radius2, Scalar(255,255,255), -1, CV_AA);
//            }
            int fingersCount = (int)fingerTips2.size();
            m_left_hand->setFingers(fingersCount);
        }
    }
}

void HandDetection::findFingerTips(vector<Point> &cvxHull, vector<CvConvexityDefect> &defects, vector<CvConvexityDefect> &fingerTips){
    int upper = ROWS, lower = 0, xu = 0, xl = 0;
    int hullcount = (int) cvxHull.size();
    //printf("Vertices %d y Curva %d\n", hullcount, curva.size());
    for(int i = 0; i < hullcount; i++ ) {
        int idx = i; // corner index
        if (cvxHull[idx].y < upper) {upper = cvxHull[idx].y; xu = cvxHull[idx].x;};
        if (cvxHull[idx].y > lower) {lower = cvxHull[idx].y; xl = cvxHull[idx].x;}
    }

    //Todos los puntos situados en el 60% de la superficie de la imagen (de arriba a abajo)
    float cutoff = lower - (lower - upper) * 0.4f;
//    printf("cutoff %f\n",cutoff);

    Rect contenedor = boundingRect(Mat(cvxHull));
    for (int i=0; i<(int)cvxHull.size(); i++) {
        for(int j=0; j<(int)defects.size(); j++) {
            CvPoint cvp1 = *(defects.at(j).start);
//            CvPoint cvpd = *(defects.at(j).depth_point);
            Point p1(cvp1.x, cvp1.y);
            Point pdp = Utils::getRectCenter(contenedor);//tendria q ser la distancia maxima de la palma al punto mas lejana
            if(cvxHull.at(i) == p1){
                //Si la distancia formada por start-->depth_point es menor q un factor X
                double d = Utils::euclideanDistance(p1, pdp);//si la diferencia es de menos de un 20% la long del otro entonces ok
//                printf("distancia %f\n",d);
                if(d >= DISTANCE_THRESHOLD && cvxHull.at(i).y < cutoff){//los q estan fuera del circulo contenedor
                    fingerTips.push_back(defects.at(j));
                }
            }
        }
        //        int idx = j; // corner index
//        int pdx = idx == 0 ? (int)cvxHull.size() - 1 : idx - 1; //  predecessor of idx
//        int sdx = idx ==(int) cvxHull.size() - 1 ? 0 : idx + 1; // successor of idx
//        Point v1 = Point(cvxHull[sdx].x - cvxHull[idx].x, cvxHull[sdx].y - cvxHull[idx].y);//vector director
//        Point v2 = Point(cvxHull[pdx].x - cvxHull[idx].x, cvxHull[pdx].y - cvxHull[idx].y);
//        float angle = acos( (v1.x*v2.x + v1.y*v2.y) / (norm(v1) * norm(v2)) )* 180 / M_PI;
//        // low interior angle + within upper 90% of region -> we got a finger
//        //printf("angulo %f",angle);
//        //Los dedos abiertos forman un angulo mas o menos igual o superior a este
//        if (angle > 60 && cvxHull[idx].y < cutoff) {//angle > 0.5 &&
//            int u = cvxHull[idx].x;
//            int v = cvxHull[idx].y;
//            fingerTips.push_back(Point2i(u,v));
//            cv::circle(color, cvxHull[idx], 10, Scalar(200,200,200), 5, -1);
//        }
    }
}

int HandDetection::getFingersCount(int hand) const{
    int fingers = 0;
    if(hand == LEFT_HAND)
        fingers = m_left_hand->getFingers();
    else if(hand == RIGHT_HAND)
        fingers = m_right_hand->getFingers();
    return fingers;
}

