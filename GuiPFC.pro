#-------------------------------------------------
#
# Project created by QtCreator 2011-06-20T19:39:20
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = GuiPFC
TEMPLATE = app


SOURCES +=\
    main.cpp \
    guiapp.cpp \
    mykinect.cpp \
    btcomm.cpp \
    guicontroller.cpp \
    handdetection.cpp \
    utils.cpp \
    hand.cpp \
    steeringwheel.cpp \
    trackobject.cpp \
    glwidget.cpp

HEADERS  += guiapp.hpp \
    mykinect.hpp \
    libfreenect.hpp \
    btcomm.hpp \
    guicontroller.hpp \
    handdetection.hpp \
    utils.hpp \
    guiappheaders.hpp \
    hand.hpp \
    steeringwheel.hpp \
    trackobject.hpp \
    glwidget.hpp

FORMS    += guiapp.ui

LIBS += -lfreenect -lGLU -lGL -lSM -lICE -lX11 -lXext -lglut -lXmu -lXi -lpthread -lm -lcv -lcvaux -lrt -lcxcore -lhighgui -ljpeg -lml -lbluetooth

RESOURCES += \
    fuentes.qrc

OTHER_FILES +=
