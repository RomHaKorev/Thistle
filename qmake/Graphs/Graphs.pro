#-------------------------------------------------
#
# Project created by QtCreator 2014-07-06T20:43:06
#
#-------------------------------------------------

QT       += widgets

Release:TARGET = Graphs
Debug:TARGET = Graphsd
TEMPLATE = lib

DEFINES += GRAPHS_LIBRARY


Release:DESTDIR_TARGET = ../../../bin/mingw32
Release:DESTDIR = ../../../bin/mingw32
Release:OBJECTS_DIR = ../../../bin/mingw32/generatedFiles
Release:MOC_DIR = ../../../bin/mingw32/generatedFiles
Release:RCC_DIR = ../../../bin/mingw32/generatedFiles
Release:UI_DIR = ../../../bin/mingw32/generatedFiles

Debug:DESTDIR_TARGET = ../../../bin/mingw32
Debug:DESTDIR = ../../../bin/mingw32
Debug:OBJECTS_DIR = ../../../bin/mingw32/generatedFiles
Debug:MOC_DIR = ../../../bin/mingw32/generatedFiles
Debug:RCC_DIR = ../../../bin/mingw32/generatedFiles
Debug:UI_DIR = ../../../bin/mingw32/generatedFiles


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


CONFIG(release): LIBS += -L$$DESTDIR/ -lKernel
else:CONFIG(debug) LIBS += -L$$DESTDIR/ -lKerneld

INCLUDEPATH += $$PWD/../../Thistle/kernel
DEPENDPATH += $$PWD/../../Thistle/kernel
