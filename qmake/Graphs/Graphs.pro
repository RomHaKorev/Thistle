QT       += widgets
TEMPLATE = lib

release:TARGET = Graphs
debug:TARGET = Graphsd

DEFINES += THISTLE_GRAPHS_LIBRARY

QMAKE_CXXFLAGS += -Wno-unused-function

DESTDIR = ../

unix:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../bin/libs/gcc/ -lKernel
unix:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../bin/libs/gcc/ -lKerneld

win32:CONFIG(release, debug|release):: LIBS += -L$$PWD/../../../bin/libs/mingw32 -lKernel
win32:CONFIG(debug, debug|release):LIBS += -L$$PWD/../../../bin/libs/mingw32 -lKerneld


INCLUDEPATH += $$PWD/../../../
DEPENDPATH += $$PWD/../../../

QMAKE_CXXFLAGS += -Wno-unused-function -Wall


HEADERS += ../../Thistle/Graphs/edge.h \
	../../Thistle/Graphs/node.h \
	../../Thistle/Graphs/graphview.h \
	../../Thistle/Graphs/graphmodel.h \
	../../Thistle/Graphs/abstractgraphalgorithm.h \
	../../Thistle/Graphs/graphalgorithm.h \
	../../Thistle/Graphs/private/graphmodel_p.h \
	../../Thistle/Graphs/private/graphview_p.h \
	../../Thistle/Graphs/private/node_p.h \
	../../Thistle/Graphs/private/node_p.h

SOURCES += ../../Thistle/Graphs/abstractgraphalgorithm.cpp \
	../../Thistle/Graphs/graphalgorithm.cpp \
	../../Thistle/Graphs/edge.cpp \
	../../Thistle/Graphs/graphmodel.cpp \
	../../Thistle/Graphs/graphview.cpp \
	../../Thistle/Graphs/node.cpp \
	../../Thistle/Graphs/private/graphalgorithm_p.cpp \
	../../Thistle/Graphs/private/graphmodel_p.cpp \
	../../Thistle/Graphs/private/graphview_p.cpp \
	../../Thistle/Graphs/private/abstractgraphalgorithm_p.cpp \
	../../Thistle/Graphs/private/node_p.cpp
