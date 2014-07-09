#-------------------------------------------------
#
# Project created by QtCreator 2014-07-06T20:42:08
#
#-------------------------------------------------

QT       += widgets

TARGET = Trees
TEMPLATE = lib

DEFINES += TREES_LIBRARY


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

win32:CONFIG(release, debug|release): LIBS += -L$$DESTDIR/ -lKernel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$DESTDIR/ -lKernel
else:unix: LIBS += -L$$DESTDIR/ -lKernel

INCLUDEPATH += $$PWD/../../Thistle/kernel
DEPENDPATH += $$PWD/../../Thistle/kernel
