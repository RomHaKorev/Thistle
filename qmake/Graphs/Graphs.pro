#-------------------------------------------------
#
# Project created by QtCreator 2014-07-06T20:43:06
#
#-------------------------------------------------

QT       += widgets

TARGET = Graphs
TEMPLATE = lib

DEFINES += GRAPHS_LIBRARY


Release:DESTDIR = ../../../bin/mingw32/release
Release:OBJECTS_DIR = ../../../bin/mingw32/release/generatedFiles
Release:MOC_DIR = ../../../bin/mingw32/release/generatedFiles
Release:RCC_DIR = ../../../bin/mingw32/release/generatedFiles
Release:UI_DIR = ../../../bin/mingw32/release/generatedFiles

Debug:DESTDIR = ../../../bin/mingw32/debug
Debug:OBJECTS_DIR = ../../../bin/mingw32/debug/generatedFiles
Debug:MOC_DIR = ../../../bin/mingw32/debug/generatedFiles
Debug:RCC_DIR = ../../../bin/mingw32/debug/generatedFiles
Debug:UI_DIR = ../../../bin/mingw32/debug/generatedFiles

HEADERS += \
	../../Thistle/Graphs/abstractgraphalgorithm.h \
	../../Thistle/Graphs/abstractgraphalgorithm_p.h \
	../../Thistle/Graphs/edge.h \
	../../Thistle/Graphs/graphalgorithm.h \
	../../Thistle/Graphs/graphalgorithm_p.h \
	../../Thistle/Graphs/graphmodel.h \
	../../Thistle/Graphs/graphmodel_p.h \
	../../Thistle/Graphs/graphview.h \
	../../Thistle/Graphs/graphview_p.h \
	../../Thistle/Graphs/node.h \
	../../Thistle/Graphs/node_p.h

SOURCES += \
	../../Thistle/Graphs/abstractgraphalgorithm.cpp \
	../../Thistle/Graphs/abstractgraphalgorithm_p.cpp \
	../../Thistle/Graphs/edge.cpp \
	../../Thistle/Graphs/graphalgorithm.cpp \
	../../Thistle/Graphs/graphalgorithm_p.cpp \
	../../Thistle/Graphs/graphmodel.cpp \
	../../Thistle/Graphs/graphmodel_p.cpp \
	../../Thistle/Graphs/graphview.cpp \
	../../Thistle/Graphs/graphview_p.cpp \
	../../Thistle/Graphs/node.cpp \
	../../Thistle/Graphs/node_p.cpp


win32:CONFIG(release, debug|release): LIBS += -L$$DESTDIR/ -lKernel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$DESTDIR/ -lKernel
else:unix: LIBS += -L$$DESTDIR/ -lKernel

INCLUDEPATH += $$PWD/../../Thistle/kernel
DEPENDPATH += $$PWD/../../Thistle/kernel
