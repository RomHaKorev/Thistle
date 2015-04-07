QT       += widgets
TEMPLATE = lib

release:TARGET = Trees
debug:TARGET = Treesd

DEFINES += THISTLE_TREES_LIBRARY

QMAKE_CXXFLAGS += -Wno-unused-function

DESTDIR = ../

unix:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../bin/gcc/ -lKernel
unix:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../bin/gcc/ -lKerneld

win32:CONFIG(release, debug|release):: LIBS += -L$$PWD/../../../bin/mingw32 -lKernel
win32:CONFIG(debug, debug|release):LIBS += -L$$PWD/../../../bin/mingw32 -lKerneld


INCLUDEPATH += $$PWD/../../../
DEPENDPATH += $$PWD/../../../

QMAKE_CXXFLAGS += -Wno-unused-function


HEADERS += ../../Thistle/Trees/radialtreelayout.h \
	../../Thistle/Trees/radialtreelayout_p.h \
	../../Thistle/Trees/treelayout_p.h \
	../../Thistle/Trees/treeview_p.h \
	../../Thistle/Trees/treeview.h \
	../../Thistle/Trees/treelayout.h
SOURCES += ../../Thistle/Trees/radialtreelayout.cpp \
	../../Thistle/Trees/radialtreelayout_p.cpp \
	../../Thistle/Trees/treelayout.cpp \
	../../Thistle/Trees/treelayout_p.cpp \
	../../Thistle/Trees/treeview.cpp \
	../../Thistle/Trees/treeview_p.cpp
