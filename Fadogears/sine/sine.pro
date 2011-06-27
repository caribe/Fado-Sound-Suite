#-------------------------------------------------
#
# Project created by QtCreator 2011-06-26T21:11:42
#
#-------------------------------------------------

QT       -= core gui

TARGET = sine
TEMPLATE = lib

DEFINES += SINE_LIBRARY

SOURCES += sine.cpp

HEADERS += sine.h\
        sine_global.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE45BD793
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = sine.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
