QT += widgets gui
TEMPLATE = lib


CONFIG(debug, debug|release) {
TARGET = ThCored
}

CONFIG(release, debug|release) {
TARGET = ThCore
}


DEFINES += THISTLE_CORE_LIBRARY

QMAKE_CXXFLAGS += -Wno-unused-function -Wall

unix:DESTDIR = $$PWD/../../../../bin/libs/gcc/
win32:DESTDIR = $$PWD/../../../../bin/libs/mingw32/

SOURCES += \
	../../../Thistle/Core/private/itemstyle_p.cpp \
	../../../Thistle/Core/itemstyle.cpp \
	../../../Thistle/Core/private/itemdelegate_p.cpp \
	../../../Thistle/Core/itemdelegate.cpp \
	../../../Thistle/Core/private/abstractitemview_p.cpp \
	../../../Thistle/Core/abstractitemview.cpp

HEADERS += \
	../../../Thistle/Core/private/itemstyle_p.h \
	../../../Thistle/Core/itemstyle.h \
	../../../Thistle/Core/private/itemdelegate_p.h \
	../../../Thistle/Core/itemdelegate.h \
	../../../Thistle/Core/global.h \
	../../../Thistle/Core/private/abstractitemview_p.h \
	../../../Thistle/Core/abstractitemview.h
