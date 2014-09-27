#-------------------------------------------------
#
# Project created by QtCreator 2014-07-06T20:42:08
#
#-------------------------------------------------

QT       += widgets

Release:TARGET = Trees
Debug:TARGET = Treesd
TEMPLATE = lib

DEFINES += TREES_LIBRARY


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
	../../Thistle/Trees/abstracttree.h \
	../../Thistle/Trees/abstracttree_p.h \
	../../Thistle/Trees/horizontaltree.h \
	../../Thistle/Trees/horizontaltree_p.h \
	../../Thistle/Trees/radialtree.h \
	../../Thistle/Trees/radialtree_p.h \
	../../Thistle/Trees/verticaltree.h \
	../../Thistle/Trees/verticaltree_p.h

SOURCES += \
	../../Thistle/Trees/abstracttree.cpp \
	../../Thistle/Trees/abstracttree_p.cpp \
	../../Thistle/Trees/horizontaltree.cpp \
	../../Thistle/Trees/horizontaltree_p.cpp \
	../../Thistle/Trees/radialtree.cpp \
	../../Thistle/Trees/radialtree_p.cpp \
	../../Thistle/Trees/verticaltree.cpp \
	../../Thistle/Trees/verticaltree_p.cpp

CONFIG(release): LIBS += -L$$DESTDIR/ -lKernel
else:CONFIG(debug) LIBS += -L$$DESTDIR/ -lKerneld

INCLUDEPATH += $$PWD/../../Thistle/kernel
DEPENDPATH += $$PWD/../../Thistle/kernel
