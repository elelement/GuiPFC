#include "mykinect.hpp"


/*
 * Constructores y metodos
 * El constructor está modificado para poder emplear una función de normalización de los datos
 * más precisa que la que venía con el driver.
 * Se calcula de la siguienet forma const float k1 = 1.1863;
                                    const float k2 = 2842.5;
                                    const float k3 = 0.1236;
                                    const float depth = k3 * tanf(i/k2 + k1);
 */
MyFreenectDevice::MyFreenectDevice(freenect_context *_ctx, int _index)
: Freenect::FreenectDevice(_ctx, _index), m_buffer_depth(FREENECT_DEPTH_11BIT_SIZE), m_buffer_rgb(FREENECT_VIDEO_RGB_SIZE), m_gamma(2048), m_new_rgb_frame(false), m_new_depth_frame(false),
m_depth_mat(Size(640, 480), CV_16UC1), m_rgb_mat(Size(640, 480), CV_8UC3, Scalar(0)), m_own_mat(Size(640, 480), CV_8UC3, Scalar(0)) {
    for (size_t i = 0; i < 2048; i++) {
        const float k1 = 1.1863;
        const float k2 = 2842.5;
        const float k3 = 0.1236;
        const float depth = k3 * tanf(i/k2 + k1);
        m_gamma[i] = depth;
//        float v = i / 2048.0;
//        v = std::pow(v, 3)* 7;
//        m_gamma[i] = v * 6 * 256;
    }
    m_degrees = this->getState().getTiltDegs();
}
// Do not call directly even in child

MyFreenectDevice::~MyFreenectDevice(){
}

void MyFreenectDevice::VideoCallback(void* _rgb, uint32_t timestamp) {
    //        std::cout << "RGB callback" << std::endl;
    (void)timestamp;//ignora el "unused warning", que no es problema nuestro, sino de la biblioteca libfreenect
    m_rgb_mutex.lock();
    uint8_t* rgb = static_cast<uint8_t*> (_rgb);
    m_rgb_mat.data = rgb;
    m_new_rgb_frame = true;
    m_rgb_mutex.unlock();
}
// Do not call directly even in child

void MyFreenectDevice::DepthCallback(void* _depth, uint32_t timestamp) {
    //        std::cout << "Depth callback" << std::endl;
    (void)timestamp;
    m_depth_mutex.lock();
    uint16_t* depth = static_cast<uint16_t*> (_depth);
    m_depth_mat.data = (uchar*) depth;
    m_new_depth_frame = true;
    m_depth_mutex.unlock();
}

bool MyFreenectDevice::getVideo(Mat& output) {
    m_rgb_mutex.lock();
    if (m_new_rgb_frame) {
        cv::cvtColor(m_rgb_mat, output, CV_RGB2BGR);
        m_new_rgb_frame = false;
        m_rgb_mutex.unlock();
        return true;
    } else {
        m_rgb_mutex.unlock();
        return false;
    }
}

bool MyFreenectDevice::getDepth(Mat& output) {
    m_depth_mutex.lock();
    if (m_new_depth_frame) {
        m_depth_mat.copyTo(output);
        m_new_depth_frame = false;
        m_depth_mutex.unlock();
        return true;
    } else {
        m_depth_mutex.unlock();
        return false;
    }
}

/*
 * Los mutex
 */
Mutex::Mutex() {
    pthread_mutex_init(&m_mutex, NULL);
}

void Mutex::lock() {
    pthread_mutex_lock(&m_mutex);
}

void Mutex::unlock() {
    pthread_mutex_unlock(&m_mutex);
}
