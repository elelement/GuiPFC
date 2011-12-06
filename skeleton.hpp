#ifndef SKELETON_HPP
#define SKELETON_HPP

class Skeleton
{
private:
    vector<Point>* m_skeleton;
    vector<Point>* m_joints;

    /*
     * Métodos para la extracci&oacute;n del
     * esqueleto a partir de una imagen.
     */
    void pavlidis(Mat& image);//Imagen como parámetro
    void zhangSuen(Mat& image);
    void voronoi(Mat& image);
    void holt(Mat& image);

public:
    Skeleton();
    Skeleton(string method);
    virtual ~Skeleton();
};

#endif // SKELETON_HPP
