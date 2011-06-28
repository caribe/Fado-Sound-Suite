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
    core/encoder.cpp \
    core/core.cpp \
    core/config.cpp \
    ext/param.cpp \
    ext/machine.cpp \
    core/store.cpp \
    gears/master.cpp \
    gears/lineinput.cpp \
    gears/fileinput.cpp

HEADERS  += mainwindow.h \
    gui/tracktabledata.h \
    gui/tracktable.h \
    gui/track.h \
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
    core/encoder.h \
    core/core.h \
    core/config.h \
    ext/param.h \
    ext/machine.h \
    core/store.h \
    gears/master.h \
    gears/lineinput.h \
    gears/fileinput.h

RESOURCES += \
    resources/resources.qrc

unix|win32: LIBS += -lfftw3
unix|win32: LIBS += -ljack
unix|win32: LIBS += -lsndfile

OTHER_FILES += \
    resources/table.png \
    resources/splashscreen.jpg \
    resources/speaker.png \
    resources/playback.png \
    resources/node.png \
    resources/gear.png \
    resources/block.png \
    resources/001_60.png \
    resources/001_59.png \
    resources/001_58.png \
    resources/001_57.png \
    resources/001_56.png \
    resources/001_55.png \
    resources/001_54.png \
    resources/001_53.png \
    resources/001_52.png \
    resources/001_51.png \
    resources/001_50.png \
    resources/001_49.png \
    resources/001_48.png \
    resources/001_47.png \
    resources/001_46.png \
    resources/001_45.png \
    resources/001_44.png \
    resources/001_43.png \
    resources/001_42.png \
    resources/001_41.png \
    resources/001_40.png \
    resources/001_39.png \
    resources/001_38.png \
    resources/001_37.png \
    resources/001_36.png \
    resources/001_35.png \
    resources/001_34.png \
    resources/001_33.png \
    resources/001_32.png \
    resources/001_31.png \
    resources/001_30.png \
    resources/001_29.png \
    resources/001_28.png \
    resources/001_27.png \
    resources/001_26.png \
    resources/001_25.png \
    resources/001_24.png \
    resources/001_23.png \
    resources/001_22.png \
    resources/001_21.png \
    resources/001_20.png \
    resources/001_19.png \
    resources/001_18.png \
    resources/001_17.png \
    resources/001_16.png \
    resources/001_15.png \
    resources/001_14.png \
    resources/001_13.png \
    resources/001_12.png \
    resources/001_11.png \
    resources/001_10.png \
    resources/001_09.png \
    resources/001_08.png \
    resources/001_07.png \
    resources/001_06.png \
    resources/001_05.png \
    resources/001_04.png \
    resources/001_03.png \
    resources/001_02.png \
    resources/001_01.png
