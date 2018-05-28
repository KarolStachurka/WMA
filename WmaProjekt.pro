#-------------------------------------------------
#
# Project created by QtCreator 2018-05-20T17:46:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WmaProjekt
TEMPLATE = app
CONFIG += console c++11
INCLUDEPATH += /usr/local/include/opencv

LIBS += -L/usr/local/lib \
-lopencv_core \
-lopencv_imgproc \
-lopencv_imgcodecs \
-lopencv_highgui \
-lopencv_ml \
-lopencv_video \
-lopencv_videoio \
-lopencv_features2d \
-lopencv_calib3d \
-lopencv_objdetect \
-lopencv_contrib \
-lopencv_legacy \
-lopencv_flann \
-lopencv_face \
-lopencv_bgsegm \

SOURCES += main.cpp\
        mainwindow.cpp \
    cardetection.cpp \
    car.cpp

HEADERS  += mainwindow.h \
    cardetection.h \
    car.h

FORMS    += mainwindow.ui
