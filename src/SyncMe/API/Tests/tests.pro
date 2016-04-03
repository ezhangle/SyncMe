#-------------------------------------------------
#
# SyncMe Dropbox API Tests QT Project
#
#-------------------------------------------------

QT       += network testlib xml gui

TARGET = SM_Dropbox_Tests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    SM_SyncMe_Dropbox_Test.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    SM_SyncMe_Dropbox_Test.h \
    SM_Dropbox_Keys_Data.h

INCLUDEPATH += ../Sources/Dropbox

target.path = lib/
INSTALLS += target
