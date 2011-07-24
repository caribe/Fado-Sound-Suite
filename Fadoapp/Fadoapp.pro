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
    core/updates.cpp \
    core/encoder.cpp \
    core/core.cpp \
    core/config.cpp \
    ext/param.cpp \
    ext/machine.cpp \
    gears/master.cpp \
    gears/lineinput.cpp \
    gears/fileinput.cpp \
    gui/pattern/patterntabledata.cpp \
    gui/pattern/patterntable.cpp \
    gui/pattern/pattern.cpp \
    gui/route/route.cpp \
    gui/route/machinebox.cpp \
    gui/route/linkbox.cpp \
    gui/track/tracks.cpp \
    gui/track/tracksmodel.cpp \
    gui/analyze/analyze.cpp \
    gui/analyze/timeanalyze.cpp \
    gui/analyze/freqanalyze.cpp

HEADERS  += mainwindow.h \
    core/updates.h \
    core/encoder.h \
    core/core.h \
    core/config.h \
    ext/param.h \
    ext/machine.h \
    gears/master.h \
    gears/lineinput.h \
    gears/fileinput.h \
    gui/pattern/patterntabledata.h \
    gui/pattern/patterntable.h \
    gui/pattern/pattern.h \
    gui/route/route.h \
    gui/route/machinebox.h \
    gui/route/linkbox.h \
    gui/track/tracks.h \
    gui/track/tracksmodel.h \
    gui/analyze/analyze.h \
    gui/analyze/timeanalyze.h \
    gui/analyze/freqanalyze.h

RESOURCES += \
    icons.qrc

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
    resources/001_08.png \
    resources/table--plus.png \
    resources/table--pencil.png \
    resources/table--minus.png \
    resources/question.png \
    resources/minus.png \
    resources/plus.png \
    resources/fado-logo.svg \
    resources/slash.png \
    resources/folder-open-document.png \
    resources/document.png \
    resources/disk.png \
    resources/disk--pencil.png \
    resources/cross-button.png \
    resources/control.png \
    resources/control-stop.png \
    resources/control-stop-square.png \
    resources/control-stop-180.png \
    resources/control-record.png \
    resources/user.png \
    resources/system-monitor.png \
    resources/cross.png \
    resources/pencil.png \
    resources/microphone.png \
    resources/document-music.png \
    resources/funnel.png \
    icons/user.png \
    icons/table.png \
    icons/table--plus.png \
    icons/table--pencil.png \
    icons/table--minus.png \
    icons/system-monitor.png \
    icons/splashscreen.jpg \
    icons/speaker.png \
    icons/slash.png \
    icons/question.png \
    icons/plus.png \
    icons/pencil.png \
    icons/node.png \
    icons/minus.png \
    icons/microphone.png \
    icons/gear.png \
    icons/funnel.png \
    icons/folder-open-document.png \
    icons/fado-logo.svg \
    icons/document.png \
    icons/document-music.png \
    icons/disk.png \
    icons/disk--pencil.png \
    icons/cross.png \
    icons/cross-button.png \
    icons/control.png \
    icons/control-stop.png \
    icons/control-stop-square.png \
    icons/control-stop-180.png \
    icons/control-record.png \
    icons/block.png \
    icons/speaker-volume-none.png \
    icons/speaker-volume-control-mute.png \
    icons/broom.png \
    icons/table-insert-row.png \
    icons/table-delete-row.png \
    icons/gear-small.png \
    icons/funnel-small.png
