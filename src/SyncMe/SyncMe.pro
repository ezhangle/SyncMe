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
    API/Sources/Dropbox/SM_qdropbox.cpp \
    API/Sources/Dropbox/SM_dropbox_account.cpp \
    API/Sources/Dropbox/SM_dropbox_deltaresponse.cpp \
    API/Sources/Dropbox/SM_dropbox_fileinfo.cpp \
    API/Sources/Dropbox/SM_dropbox_file.cpp \
    API/Sources/Dropbox/SM_dropbox_json.cpp \
    API/Tests/qtdropboxtest.cpp

HEADERS  += mainwindow.h \
    API/Headers/Dropbox/SM_dropbox.h \
    API/Headers/Dropbox/SM_dropbox_global.h \
    API/Headers/Dropbox/SM_dropbox_account.h \
    API/Headers/Dropbox/SM_dropbox_deltaresponse.h \
    API/Headers/Dropbox/SM_dropbox_file.h \
    API/Headers/Dropbox/SM_dropbox_fileinfo.h \
    API/Headers/Dropbox/SM_dropbox_json.h \
    API/Headers/Dropbox/SM_qdropbox.h

FORMS    += mainwindow.ui
