# *******************************************************
#   Copyright (C) 2020 AMELLAL Oussama <ouss.amellal@gmail.com>
#
#  This file is part of 'CameraSurveillance' project.
#
#  'CameraSurveillance' project can not be copied and/or distributed without the express permission of AMELLAL Oussama
# *******************************************************


QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += C:\opencv\release\install\include

LIBS += C:\opencv\release\bin\libopencv_core440.dll
LIBS += C:\opencv\release\bin\libopencv_highgui440.dll
LIBS += C:\opencv\release\bin\libopencv_imgcodecs440.dll
LIBS += C:\opencv\release\bin\libopencv_imgproc440.dll
LIBS += C:\opencv\release\bin\libopencv_calib3d440.dll
LIBS += C:\opencv\release\bin\libopencv_dnn440.dll
LIBS += C:\opencv\release\bin\libopencv_features2d440.dll
LIBS += C:\opencv\release\bin\libopencv_flann440.dll
LIBS += C:\opencv\release\bin\libopencv_gapi440.dll
LIBS += C:\opencv\release\bin\libopencv_ml440.dll
LIBS += C:\opencv\release\bin\libopencv_objdetect440.dll
LIBS += C:\opencv\release\bin\libopencv_photo440.dll
LIBS += C:\opencv\release\bin\libopencv_stitching440.dll
LIBS += C:\opencv\release\bin\libopencv_video440.dll
LIBS += C:\opencv\release\bin\libopencv_videoio440.dll
LIBS += C:\opencv\release\bin\opencv_videoio_ffmpeg440.dll


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    objectcamvideo.cpp \
    outils.cpp

HEADERS += \
    mainwindow.h \
    objectcamvideo.h \
    outils.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
