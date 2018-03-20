INCLUDEPATH += $$PWD
QT += remoteobjects

HEADERS += \
    $$PWD/sslremoteobjects.h \
    $$PWD/rosslclientio.h \
    $$PWD/rosslserverio.h

SOURCES += \
    $$PWD/sslremoteobjects.cpp \
    $$PWD/rosslclientio.cpp \
    $$PWD/rosslserverio.cpp
