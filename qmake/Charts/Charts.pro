#-------------------------------------------------
#
# Project created by QtCreator 2014-07-06T20:40:01
#
#-------------------------------------------------

QT       += widgets

TARGET = Charts
TEMPLATE = lib

DEFINES += CHARTS_LIBRARY


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
	../../Thistle/Charts/abstractchart.h \
	../../Thistle/Charts/abstractchart_p.h \
	../../Thistle/Charts/axischart.h \
	../../Thistle/Charts/axischart_p.h \
	../../Thistle/Charts/delegates.h \
	../../Thistle/Charts/linearchart.h \
	../../Thistle/Charts/linearchart_p.h \
	../../Thistle/Charts/piechart.h \
	../../Thistle/Charts/piechart_p.h \
	../../Thistle/Charts/piechart3d.h \
	../../Thistle/Charts/piechart3d_p.h \
	../../Thistle/Charts/radialchart.h \
	../../Thistle/Charts/radialchart_p.h \
	../../Thistle/Charts/serieformat.h \
	../../Thistle/Charts/serieformat_p.h \
	../../Thistle/Charts/Axis/abstractaxis.h \
	../../Thistle/Charts/Axis/abstractaxis_p.h \
	../../Thistle/Charts/Axis/orthogonalaxis.h \
	../../Thistle/Charts/Axis/orthogonalaxis_p.h \
	../../Thistle/Charts/Axis/radialaxis.h \
	../../Thistle/Charts/Axis/radialaxis_p.h \
	../../Thistle/Charts/Legends/chartlegend.h \
	../../Thistle/Charts/Legends/linearchartlegend.h \
	../../Thistle/Charts/Legends/piechartlegend.h \
	../../Thistle/Charts/Legends/RadialChartLegend.h

SOURCES += \
	../../Thistle/Charts/abstractchart.cpp \
	../../Thistle/Charts/abstractchart_p.cpp \
	../../Thistle/Charts/axischart.cpp \
	../../Thistle/Charts/axischart_p.cpp \
	../../Thistle/Charts/delegates.cpp \
	../../Thistle/Charts/linearchart.cpp \
	../../Thistle/Charts/linearchart_p.cpp \
	../../Thistle/Charts/piechart.cpp \
	../../Thistle/Charts/piechart_p.cpp \
	../../Thistle/Charts/piechart3d.cpp \
	../../Thistle/Charts/piechart3d_p.cpp \
	../../Thistle/Charts/radialchart.cpp \
	../../Thistle/Charts/radialchart_p.cpp \
	../../Thistle/Charts/serieformat.cpp \
	../../Thistle/Charts/Axis/abstractaxis.cpp \
	../../Thistle/Charts/Axis/abstractaxis_p.cpp \
	../../Thistle/Charts/Axis/orthogonalaxis.cpp \
	../../Thistle/Charts/Axis/orthogonalaxis_p.cpp \
	../../Thistle/Charts/Axis/radialaxis.cpp \
	../../Thistle/Charts/Axis/radialaxis_p.cpp \
	../../Thistle/Charts/Legends/chartlegend.cpp \
	../../Thistle/Charts/Legends/linearchartlegend.cpp \
	../../Thistle/Charts/Legends/piechartlegend.cpp \
	../../Thistle/Charts/Legends/RadialChartLegend.cpp


win32:CONFIG(release, debug|release): LIBS += -L$$DESTDIR/ -lKernel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$DESTDIR/ -lKernel
else:unix: LIBS += -L$$DESTDIR/ -lKernel

INCLUDEPATH += $$PWD/../../Thistle/kernel
DEPENDPATH += $$PWD/../../Thistle/kernel
