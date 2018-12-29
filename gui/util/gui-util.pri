QT += core widgets
INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/widgets/folderselectwidget.cpp \
    $$PWD/widgets/squarepushbutton.cpp \
    $$PWD/widgets/colorselectbutton.cpp \
    $$PWD/widgets/textedithtmlinsert.cpp \
    $$PWD/widgets/searchabletextbrowser.cpp \
    $$PWD/widgets/searchbar.cpp \
    $$PWD/widgets/fileselectwidget.cpp \
    $$PWD/widgets/treewidgetextended.cpp \
    $$PWD/widgets/treewidgetitemextended.cpp \
    $$PWD/globalgui.cpp \
    $$PWD/widgets/imageurlselection.cpp \
    $$PWD/filehashurltable.cpp

HEADERS += \
    $$PWD/widgets/folderselectwidget.h \
    $$PWD/widgets/squarepushbutton.h \
    $$PWD/widgets/colorselectbutton.h \
    $$PWD/widgets/textedithtmlinsert.h \
    $$PWD/widgets/searchabletextbrowser.h \
    $$PWD/widgets/searchbar.h \
    $$PWD/widgets/fileselectwidget.h \
    $$PWD/widgets/treewidgetextended.h \
    $$PWD/widgets/treewidgetitemextended.h \
    $$PWD/globalgui.h \
    $$PWD/widgets/imageurlselection.h \
    $$PWD/filehashurltable.h

include(../../3rdParty/QImgurUploader/QImgurUploader.pri)
