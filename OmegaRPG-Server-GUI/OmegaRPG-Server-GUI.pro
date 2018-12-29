#-------------------------------------------------
#
# Project created by QtCreator 2016-11-13T13:30:12
#
#-------------------------------------------------

RC_FILE = OmegaRPG.rc
QT       += core gui network xml

include(../version.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

TARGET = OmegaRPG-Server-GUI
TEMPLATE = app
#unix: INCLUDEPATH += /usr/include/openssl-1.0/
LIBS += -lcrypto -lssl
#unix: LIBS += -l:libcrypto.so.1.0.0

INCLUDEPATH += ..

#win32: INCLUDEPATH += C:\OpenSSL-Win32\include

SOURCES += \
    main.cpp \
    omegarpgservermainwindow.cpp


DEPENDPATH = ..

RESOURCES += ../icons/omegarpg.png

HEADERS  += \
    omegarpgservermainwindow.h



# include sub-projects
include(../gui/util/gui-util.pri)
include(../gui/network/gui-network.pri)
include(../core/server/core-server.pri)
include(../core/multi/core-multi.pri)
include(../util/util.pri)
