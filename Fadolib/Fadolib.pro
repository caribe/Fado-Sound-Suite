#-------------------------------------------------
#
# Project created by QtCreator 2011-06-26T21:09:58
#
#-------------------------------------------------

QT       += core

TARGET = Fadolib
TEMPLATE = lib

DEFINES += FADOLIB_LIBRARY

SOURCES += \
    store.cpp \
    param.cpp \
    machine.cpp

HEADERS +=\
    store.h \
    param.h \
    machine.h

unix {
	target.path = /usr/lib
	INSTALLS += target
}
