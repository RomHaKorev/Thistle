QT       += widgets
TEMPLATE = lib

release:TARGET = Graphs
debug:TARGET = Graphsd

DEFINES += THISTLE_GRAPHS_LIBRARY

QMAKE_CXXFLAGS += -Wno-unused-function

DESTDIR = ../

unix:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../bin/gcc/ -lKernel
unix:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../bin/gcc/ -lKerneld

win32:CONFIG(release, debug|release):: LIBS += -L$$PWD/../../../bin/mingw32 -lKernel
win32:CONFIG(debug, debug|release):LIBS += -L$$PWD/../../../bin/mingw32 -lKerneld


INCLUDEPATH += $$PWD/../../../
DEPENDPATH += $$PWD/../../../

QMAKE_CXXFLAGS += -Wno-unused-function


HEADERS += ../../Thistle/Graphs/edge.h \
	../../Thistle/Graphs/graphmodel_p.h \
	../../Thistle/Graphs/graphview_p.h \
	../../Thistle/Graphs/node.h \
	../../Thistle/Graphs/node_p.h \
	../../Thistle/Graphs/graphview.h \
	../../Thistle/Graphs/graphmodel.h \
	../../Thistle/Graphs/abstractgraphalgorithm_p.h \
	../../Thistle/Graphs/graphalgorithm_p.h \
	../../Thistle/Graphs/abstractgraphalgorithm.h \
	../../Thistle/Graphs/graphalgorithm.h
SOURCES += ../../Thistle/Graphs/abstractgraphalgorithm.cpp \
	../../Thistle/Graphs/abstractgraphalgorithm_p.cpp \
	../../Thistle/Graphs/graphalgorithm.cpp \
	../../Thistle/Graphs/graphalgorithm_p.cpp \
	../../Thistle/Graphs/edge.cpp \
	../../Thistle/Graphs/graphmodel.cpp \
	../../Thistle/Graphs/graphmodel_p.cpp \
	../../Thistle/Graphs/graphview.cpp \
	../../Thistle/Graphs/graphview_p.cpp \
	../../Thistle/Graphs/node.cpp \
	../../Thistle/Graphs/node_p.cpp
