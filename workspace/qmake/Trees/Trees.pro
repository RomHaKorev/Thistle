QT       += widgets
TEMPLATE = lib

CONFIG(debug, debug|release) {
TARGET = ThTreesd
}

CONFIG(release, debug|release) {
TARGET = ThTrees
}


DEFINES += THISTLE_TREES_LIBRARY

QMAKE_CXXFLAGS += -Wno-unused-function -Wall -Werror

unix:DESTDIR = $$PWD/../../../../bin/libs/gcc/
win32:DESTDIR = $$PWD/../../../../bin/libs/mingw32/

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../bin/libs/mingw32/ -lThCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../bin/libs/mingw32/ -lThCored
else:unix:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../bin/libs/gcc/ -lThCored
else:unix:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../bin/libs/gcc/ -lThCore


INCLUDEPATH += $$PWD/../../../
DEPENDPATH += $$PWD/../../../

HEADERS +=  ../../../Thistle/Trees/radialtreelayout.h \
			../../../Thistle/Trees/treeview.h \
			../../../Thistle/Trees/treelayout.h

SOURCES +=  ../../../Thistle/Trees/radialtreelayout.cpp \
			../../../Thistle/Trees/treelayout.cpp \
			../../../Thistle/Trees/treeview.cpp


HEADERS +=  ../../../Thistle/Trees/private/radialtreelayout_p.h \
			../../../Thistle/Trees/private/treelayout_p.h \
		   ../../../Thistle/Trees/private/treeview_p.h

SOURCES +=  ../../../Thistle/Trees/private/radialtreelayout_p.cpp \
			../../../Thistle/Trees/private/treelayout_p.cpp \
			../../../Thistle/Trees/private/treeview_p.cpp
