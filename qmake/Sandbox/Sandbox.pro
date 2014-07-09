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


win32:CONFIG(release, debug|release): LIBS += -L$$DESTDIR/ -lKernel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$DESTDIR/ -lKernel
else:unix: LIBS += -L$$DESTDIR/ -lKernel

win32:CONFIG(release, debug|release): LIBS += -L$$DESTDIR/ -lCharts
else:win32:CONFIG(debug, debug|release): LIBS += -L$$DESTDIR/ -lCharts
else:unix: LIBS += -L$$DESTDIR/ -lCharts

win32:CONFIG(release, debug|release): LIBS += -L$$DESTDIR/ -lTrees
else:win32:CONFIG(debug, debug|release): LIBS += -L$$DESTDIR/ -lTrees
else:unix: LIBS += -L$$DESTDIR/ -lTrees

win32:CONFIG(release, debug|release): LIBS += -L$$DESTDIR/ -lGraphs
else:win32:CONFIG(debug, debug|release): LIBS += -L$$DESTDIR/ -lGraphs
else:unix: LIBS += -L$$DESTDIR/ -lGraphs
