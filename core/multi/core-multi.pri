QT += core

SOURCES += \
    $$PWD/metaserverclient.cpp \
    $$PWD/util/serverentry.cpp \
    $$PWD/util/serverlist.cpp \
    $$PWD/util/global.cpp \
    $$PWD/util/player.cpp \
    $$PWD/util/roominfo.cpp \
    $$PWD/message/message.cpp \
    $$PWD/roompropertiesutility.cpp \
    $$PWD/message/basemessage.cpp \
    $$PWD/message/chatmessage.cpp \
    $$PWD/message/abstractmessage.cpp \
    $$PWD/message/playermessage.cpp \
    $$PWD/message/playerremovemessage.cpp \
    $$PWD/message/heartbeatmessage.cpp \
    $$PWD/message/setidmessage.cpp \
    $$PWD/message/kickmessage.cpp \
    $$PWD/message/assignrolemessage.cpp \
    $$PWD/message/givestatusmessage.cpp \
    $$PWD/message/roomsmessage.cpp \
    $$PWD/message/joinroommessage.cpp \
    $$PWD/message/createroommessage.cpp \
    $$PWD/message/joinedroommessage.cpp \
    $$PWD/message/roomvariablemessage.cpp \
    $$PWD/message/roomresourcemessage.cpp \
    $$PWD/message/unknownmessage.cpp \
    $$PWD/version.cpp


HEADERS += \
    $$PWD/metaserverclient.h \
    $$PWD/util/serverentry.h \
    $$PWD/util/serverlist.h \
    $$PWD/util/global.h \
    $$PWD/util/player.h \
    $$PWD/util/roominfo.h \
    $$PWD/message/message.h \
    $$PWD/roompropertiesutility.h \
    $$PWD/message/basemessage.h \
    $$PWD/message/chatmessage.h \
    $$PWD/message/abstractmessage.h \
    $$PWD/message/playermessage.h \
    $$PWD/message/playerremovemessage.h \
    $$PWD/message/heartbeatmessage.h \
    $$PWD/message/setidmessage.h \
    $$PWD/message/kickmessage.h \
    $$PWD/message/assignrolemessage.h \
    $$PWD/message/givestatusmessage.h \
    $$PWD/message/roomsmessage.h \
    $$PWD/message/joinroommessage.h \
    $$PWD/message/createroommessage.h \
    $$PWD/message/joinedroommessage.h \
    $$PWD/message/roomvariablemessage.h \
    $$PWD/message/roomresourcemessage.h \
    $$PWD/message/unknownmessage.h \
    $$PWD/version.h

include(roomresources/resources.pri)
include(roomvariables/variables.pri)

