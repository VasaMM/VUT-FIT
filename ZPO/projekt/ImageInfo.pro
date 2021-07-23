#-------------------------------------------------
#
# Project created by QtCreator 2019-03-30T07:38:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageInfo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11
CONFIG += staticlib

SOURCES += \
        main.cpp \
        ImageInfo.cpp \
    BasicInfo.cpp \
    ImageArea.cpp \
    Histograms.cpp \
    exif.cpp \
    Tab.cpp \
    Fraction.cpp \
    ColorCounter.cpp \
    Image.cpp \
    Tresholding.cpp \
    Filtering.cpp \
    Noise.cpp

HEADERS += \
        ImageInfo.hpp \
    BasicInfo.hpp \
    ImageArea.hpp \
    Histograms.hpp \
    exif.h \
    Tab.hpp \
    Fraction.hpp \
    ColorCounter.hpp \
    Image.hpp \
    Tresholding.hpp \
    Filtering.hpp \
    Noise.hpp

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES +=
