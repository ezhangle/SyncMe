#-------------------------------------------------
#
# Definiciones generales y dependencias
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SyncMe
TEMPLATE = app

CONFIG += C++11


SOURCES += main.cpp\
        mainwindow.cpp \
    API/Sources/Dropbox/SM_qdropbox.cpp

HEADERS  += mainwindow.h \
    API/Headers/Dropbox/SM_dropbox.h \
    API/Headers/Dropbox/SM_dropbox_global.h

FORMS    += mainwindow.ui
