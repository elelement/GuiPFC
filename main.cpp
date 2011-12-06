#include "libfreenect.hpp"
#include "btcomm.hpp"
#include "mykinect.hpp"
#include "handdetection.hpp"
#include "guiapp.hpp"
#include <QApplication>

using namespace std;
using namespace cv;

int main(int argc, char **argv) {

    QApplication a(argc, argv);
    GuiApp w;
    w.show();

    return a.exec();
}
