#-------------------------------------------------
#
# Project created by QtCreator 2014-07-06T23:40:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sandbox
TEMPLATE = app

SOURCES += \
	main.cpp

INCLUDEPATH += $$PWD/../../

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



CONFIG(release): LIBS += -L$$DESTDIR/ -lKernel
else:CONFIG(debug) LIBS += -L$$DESTDIR/ -lKerneld

CONFIG(release): LIBS += -L$$DESTDIR/ -lCharts
else:CONFIG(debug) LIBS += -L$$DESTDIR/ -lChartsd

CONFIG(release): LIBS += -L$$DESTDIR/ -lTrees
else:CONFIG(debug) LIBS += -L$$DESTDIR/ -lTreesd

CONFIG(release): LIBS += -L$$DESTDIR/ -lGraphs
else:CONFIG(debug) LIBS += -L$$DESTDIR/ -lGraphsd
