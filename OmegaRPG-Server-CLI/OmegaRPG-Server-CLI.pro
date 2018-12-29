QT += core network xml
QT -= gui
RC_FILE = OmegaRPG.rc

include(../version.pri)

CONFIG += c++14
TARGET = OmegaRPG-Server-CLI
CONFIG += console
CONFIG -= app_bundle
#unix: INCLUDEPATH += /usr/include/openssl-1.0/
LIBS += -lcrypto -lssl
#unix: LIBS += -l:libcrypto.so.1.0.0

TEMPLATE = app

INCLUDEPATH += ..

#win32: INCLUDEPATH += C:\OpenSSL-Win32\include

SOURCES += \
    main.cpp \
    consoleinterface.cpp


HEADERS += \
    consoleinterface.h


# include sub-projects
include(../core/server/core-server.pri)
include(../core/multi/core-multi.pri)
include(../util/util.pri)
