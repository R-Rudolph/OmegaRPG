TEMPLATE = app

include(../version.pri)

RC_FILE = OmegaRPG.rc
QT += widgets network multimedia xml opengl quickwidgets svg
CONFIG += c++17
CONFIG += static
INCLUDEPATH += ..
# INCLUDEPATH += /usr/include/openssl-1.0/
LIBS += -lcrypto -lssl -lz
# unix: LIBS += -l:libcrypto.so.1.0.0

#QTPLUGIN += windows qtquick2plugin
# win32: INCLUDEPATH += C:\OpenSSL-Win32\include

SOURCES += \
    main.cpp \
    omegarpgmainwindow.cpp \
    omegarpgconnectionwindow.cpp \
    omegarpgsettingswindow.cpp

HEADERS += \
    omegarpgmainwindow.h \
    omegarpgconnectionwindow.h \
    omegarpgsettingswindow.h

DEPENDPATH = ..

RESOURCES += \
    ../icons/omegarpg.png \
    ../icons/pencil.png \
    ../icons/paintbrush.png \
    ../icons/tools.png \
    ../icons/typography.png \
    ../icons/rectangle_blue.png \
    ../icons/bright.png \
    ../icons/dark.png \
    ../icons/water.png \
    ../icons/circlecompass.png \
    ../icons/miniature.png \
    ../icons/cursor.png \
    ../icons/error.png \
    ../icons/loading.png \
    ../icons/d2.png \
    ../icons/d3.png \
    ../icons/d4.png \
    ../icons/d6.png \
    ../icons/d8.png \
    ../icons/d10.png \
    ../icons/d12.png \
    ../icons/d20.png \
    ../icons/d100.png \
    ../icons/sheet.png \
    ../icons/attention.png \
    ../sounds/quack.mp3 \
    ../icons/saddie.png \
    ../qml/ErrorMessage.qml \
    ../icons/misc/edit-delete.svg           \
    ../icons/misc/security-medium.svg       \
    ../icons/misc/view-sort-descending.svg  \
    ../icons/misc/list-add.svg              \
    ../icons/misc/security-low.svg          \
    ../icons/misc/edit-paste.svg            \
    ../icons/misc/edit-cut.svg              \
    ../icons/misc/security-high.svg         \
    ../icons/misc/folder.svg                \
    ../icons/misc/document-properties.svg   \
    ../icons/misc/document-close.svg        \
    ../icons/misc/document-open.svg         \
    ../icons/misc/edit-copy.svg             \
    ../icons/misc/edit.svg                  \
    ../icons/misc/zoom.svg                  \
    ../icons/misc/go-next.svg               \
    ../icons/misc/application-exit.svg      \
    ../icons/misc/view-sort-ascending.svg   \
    ../icons/misc/help-about.svg            \
    ../icons/misc/list-remove.svg           \
    ../icons/misc/document-save.svg

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

# include sub-projects)
include(../gui/map/gui-map.pri)
include(../gui/util/gui-util.pri)
include(../gui/nightmode/gui-nightmode.pri)
include(../gui/client/gui-client.pri)
include(../gui/network/gui-network.pri)
include(../core/client/core-client.pri)
include(../core/server/core-server.pri)
include(../core/multi/core-multi.pri)
include(../util/util.pri)

# include externals libs
include(../3rdParty/Qt-TabFormWidget/QTabFormWidget.pri)
DEFINES += QUAZIP_STATIC
include(../3rdParty/quazip/quazip.pri)
include(../3rdParty/QtUtils/QNetworkProxyWidget.pri)

DISTFILES += \
    QmlImports.qml


