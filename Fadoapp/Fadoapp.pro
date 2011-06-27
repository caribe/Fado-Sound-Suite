#-------------------------------------------------
#
# Project created by QtCreator 2011-06-26T21:08:27
#
#-------------------------------------------------

QT       += core gui network xml

TARGET = Fadoapp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gui/tracktabledata.cpp \
    gui/tracktable.cpp \
    gui/track.cpp \
    gui/routetree.cpp \
    gui/route.cpp \
    gui/playback.cpp \
    gui/patterntabledata.cpp \
    gui/patterntable.cpp \
    gui/pattern.cpp \
    gui/machinebox.cpp \
    gui/linkbox.cpp \
    gui/about.cpp \
    core/updates.cpp \
    core/settings.cpp \
    core/master.cpp \
    core/lineinput.cpp \
    core/fileinput.cpp \
    core/encoder.cpp \
    core/core.cpp \
    core/config.cpp

HEADERS  += mainwindow.h \
    gui/tracktabledata.h \
    gui/tracktable.h \
    gui/track.h \
    gui/routetree.h \
    gui/route.h \
    gui/playback.h \
    gui/patterntabledata.h \
    gui/patterntable.h \
    gui/pattern.h \
    gui/machinebox.h \
    gui/linkbox.h \
    gui/about.h \
    core/updates.h \
    core/settings.h \
    core/master.h \
    core/lineinput.h \
    core/fileinput.h \
    core/encoder.h \
    core/core.h \
    core/config.h

RESOURCES += \
    resources/resources.qrc

unix|win32: LIBS += -lfftw3

unix|win32: LIBS += -ljack

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Fadolib/release/ -lFadolib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Fadolib/debug/ -lFadolib
else:symbian: LIBS += -lFadolib
else:unix: LIBS += -L$$OUT_PWD/../Fadolib/ -lFadolib

INCLUDEPATH += $$PWD/../Fadolib
DEPENDPATH += $$PWD/../Fadolib

unix|win32: LIBS += -lsndfile
