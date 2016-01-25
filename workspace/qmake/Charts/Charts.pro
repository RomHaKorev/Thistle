QT += widgets
TEMPLATE = lib

CONFIG(debug, debug|release) {
TARGET = ThChartsd
}

CONFIG(release, debug|release) {
TARGET = ThCharts
}


DEFINES += THISTLE_CHARTS_LIBRARY

QMAKE_CXXFLAGS += -Wno-unused-function -Wall

unix:DESTDIR = $$PWD/../../../../bin/libs/gcc/
win32:DESTDIR = $$PWD/../../../../bin/libs/mingw32/

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../bin/libs/mingw32/ -lThCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../bin/libs/mingw32/ -lThCored
else:unix:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../bin/libs/gcc/ -lThCored
else:unix:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../bin/libs/gcc/ -lThCore


INCLUDEPATH += $$PWD/../../../
DEPENDPATH += $$PWD/../../../


include( base.pri )
include( linechart.pri )
include( piechart.pri )
include( kiviatchart.pri )
include( scatterchart.pri )

