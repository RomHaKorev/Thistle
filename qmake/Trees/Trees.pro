QT       += widgets
TEMPLATE = lib

release:TARGET = Trees
debug:TARGET = Treesd

DEFINES += THISTLE_TREES_LIBRARY

QMAKE_CXXFLAGS += -Wno-unused-function -Wall

DESTDIR = ../

unix:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../bin/libs/gcc/ -lKernel
unix:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../bin/libs/gcc/ -lKerneld

win32:CONFIG(release, debug|release):: LIBS += -L$$PWD/../../../bin/libs/mingw32 -lKernel
win32:CONFIG(debug, debug|release):LIBS += -L$$PWD/../../../bin/libs/mingw32 -lKerneld


INCLUDEPATH += $$PWD/../../../
DEPENDPATH += $$PWD/../../../

QMAKE_CXXFLAGS += -Wno-unused-function


HEADERS += ../../Thistle/Trees/radialtreelayout.h \
	../../Thistle/Trees/private/radialtreelayout_p.h \
	../../Thistle/Trees/private/treelayout_p.h \
	../../Thistle/Trees/private/treeview_p.h \
	../../Thistle/Trees/treeview.h \
	../../Thistle/Trees/treelayout.h
SOURCES += ../../Thistle/Trees/radialtreelayout.cpp \
	../../Thistle/Trees/private/radialtreelayout_p.cpp \
	../../Thistle/Trees/treelayout.cpp \
	../../Thistle/Trees/private/treelayout_p.cpp \
	../../Thistle/Trees/treeview.cpp \
	../../Thistle/Trees/private/treeview_p.cpp
