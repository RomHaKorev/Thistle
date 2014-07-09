#-------------------------------------------------
#
# Project created by QtCreator 2014-07-05T17:16:51
#
#-------------------------------------------------

QT       += widgets

TARGET = Kernel
TEMPLATE = lib

DEFINES += KERNEL_LIBRARY

SOURCES += \
	../../Thistle/kernel/abstractitemview.cpp \
	../../Thistle/kernel/abstractitemview_p.cpp \
	../../Thistle/kernel/itemdelegate.cpp \
	../../Thistle/kernel/itemdelegate_p.cpp \
	../../Thistle/kernel/itemstyle.cpp \
	../../Thistle/kernel/itemstyle_p.cpp

HEADERS += \
	../../Thistle/kernel/abstractitemview.h \
	../../Thistle/kernel/abstractitemview_p.h \
	../../Thistle/kernel/global.h \
	../../Thistle/kernel/itemdelegate.h \
	../../Thistle/kernel/itemdelegate_p.h \
	../../Thistle/kernel/itemstyle.h \
	../../Thistle/kernel/itemstyle_p.h


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
