QT += widgets
TEMPLATE = lib

release:TARGET = Kernel
debug:TARGET = Kerneld

DEFINES += THISTLE_KERNEL_LIBRARY

DESTDIR = ../

QMAKE_CXXFLAGS += -Wno-unused-function

SOURCES += \
	../../Thistle/kernel/itemstyle_p.cpp \
	../../Thistle/kernel/itemstyle.cpp \
	../../Thistle/kernel/itemdelegate_p.cpp \
	../../Thistle/kernel/itemdelegate.cpp \
	../../Thistle/kernel/abstractitemview_p.cpp \
	../../Thistle/kernel/abstractitemview.cpp

HEADERS += \
	../../Thistle/kernel/itemstyle_p.h \
	../../Thistle/kernel/itemstyle.h \
	../../Thistle/kernel/itemdelegate_p.h \
	../../Thistle/kernel/itemdelegate.h \
	../../Thistle/kernel/global.h \
	../../Thistle/kernel/abstractitemview_p.h \
	../../Thistle/kernel/abstractitemview.h
