QT       += widgets
TEMPLATE = lib

release:TARGET = Charts
debug:TARGET = Chartsd

DEFINES += THISTLE_CHARTS_LIBRARY

QMAKE_CXXFLAGS += -Wno-unused-function -Wall

DESTDIR = ../

unix:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../bin/libs/gcc/ -lKernel
unix:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../bin/libs/gcc/ -lKerneld

win32:CONFIG(release, debug|release):: LIBS += -L$$PWD/../../../bin/libs/mingw32 -lKernel
win32:CONFIG(debug, debug|release):LIBS += -L$$PWD/../../../bin/libs/mingw32 -lKerneld


INCLUDEPATH += $$PWD/../../../
DEPENDPATH += $$PWD/../../../

HEADERS += ../../Thistle/Charts/base/linearaxis.h \
	../../Thistle/Charts/linear/linearchartcoordinateplaneview.h \
	../../Thistle/Charts/linear/linearlegendview.h \
	../../Thistle/Charts/linear/linearchart.h \
	../../Thistle/Charts/linear/linearchartwidget.h \
	../../Thistle/Charts/base/private/linearaxis_p.h \
	../../Thistle/Charts/linear/private/linearchartwidget_p.h \
	../../Thistle/Charts/linear/private/linearchart_p.h \
	../../Thistle/Charts/linear/private/linearlegendview_p.h \
	../../Thistle/Charts/base/private/cartesiancoordinateplaneview_p.h \
	../../Thistle/Charts/base/private/cartesiancoordinateplane_p.h \
	../../Thistle/Charts/base/private/abstractaxis_p.h \
	../../Thistle/Charts/base/private/abstractchart_p.h \
	../../Thistle/Charts/base/private/abstractcoordinatesystem_p.h \
	../../Thistle/Charts/base/private/abstractcoordinatesystemview_p.h \
	../../Thistle/Charts/base/private/abstractlegendview_p.h \
	../../Thistle/Charts/base/private/seriechart_p.h \
	../../Thistle/Charts/base/private/serieformat_p.h \
	../../Thistle/Charts/base/private/linearaxisdelegate_p.h \
	../../Thistle/Charts/base/abstractchartlegend.h \
	../../Thistle/Charts/base/cartesiancoordinateplane.h \
	../../Thistle/Charts/base/cartesiancoordinateplaneview.h \
	../../Thistle/Charts/base/seriedelegates.h \
	../../Thistle/Charts/base/abstractaxis.h \
	../../Thistle/Charts/base/abstractchart.h \
	../../Thistle/Charts/base/abstractcoordinatesystem.h \
	../../Thistle/Charts/base/abstractcoordinatesystemview.h \
	../../Thistle/Charts/base/serieformat.h \
	../../Thistle/Charts/base/linearaxisdelegate.h \
	../../Thistle/Charts/base/seriechart.h \
	../../Thistle/Charts/base/abstractlegendview.h \
	../../Thistle/Charts/scatter/scatterchart.h \
	../../Thistle/Charts/scatter/scatterchartcoordinateplaneview.h \
	../../Thistle/Charts/scatter/private/scatterchart_p.h \
	../../Thistle/Charts/kiviat/kiviatchart.h \
	../../Thistle/Charts/kiviat/kiviatcoordinatesystem.h \
	../../Thistle/Charts/kiviat/kiviatcoordinatesystemview.h \
	../../Thistle/Charts/kiviat/private/kiviatchart_p.h \
	../../Thistle/Charts/kiviat/private/kiviatcoordinatesystem_p.h \
	../../Thistle/Charts/pie/pielegendview.h \
	../../Thistle/Charts/pie/piechart.h \
	../../Thistle/Charts/pie/piechart3d.h \
	../../Thistle/Charts/pie/private/piechart3d_p.h \
	../../Thistle/Charts/pie/private/pielegendview_p.h \
	../../Thistle/Charts/pie/private/piechart_p.h \
    ../../Thistle/Charts/kiviat/kiviatchartwidget.h \
    ../../Thistle/Charts/kiviat/private/kiviatchartwidget_p.h \
    ../../Thistle/Charts/scatter/scatterchartwidget.h \
    ../../Thistle/Charts/scatter/private/scatterchartwidget_p.h
SOURCES += ../../Thistle/Charts/base/linearaxis.cpp \
	../../Thistle/Charts/linear/linearchart.cpp \
	../../Thistle/Charts/linear/linearchartcoordinateplaneview.cpp \
	../../Thistle/Charts/linear/linearchartwidget.cpp \
	../../Thistle/Charts/linear/linearlegendview.cpp \
	../../Thistle/Charts/base/private/linearaxis_p.cpp \
	../../Thistle/Charts/linear/private/linearchartwidget_p.cpp \
	../../Thistle/Charts/linear/private/linearchart_p.cpp \
	../../Thistle/Charts/linear/private/linearlegendview_p.cpp \
	../../Thistle/Charts/base/private/abstractaxis_p.cpp \
	../../Thistle/Charts/base/private/abstractchart_p.cpp \
	../../Thistle/Charts/base/private/abstractcoordinatesystem_p.cpp \
	../../Thistle/Charts/base/private/abstractcoordinatesystemview_p.cpp \
	../../Thistle/Charts/base/private/abstractlegendview_p.cpp \
	../../Thistle/Charts/base/private/seriechart_p.cpp \
	../../Thistle/Charts/base/private/linearaxisdelegate_p.cpp \
	../../Thistle/Charts/base/private/cartesiancoordinateplaneview_p.cpp \
	../../Thistle/Charts/base/private/cartesiancoordinateplane_p.cpp \
	../../Thistle/Charts/base/abstractaxis.cpp \
	../../Thistle/Charts/base/abstractchart.cpp \
	../../Thistle/Charts/base/abstractchartlegend.cpp \
	../../Thistle/Charts/base/abstractcoordinatesystem.cpp \
	../../Thistle/Charts/base/abstractcoordinatesystemview.cpp \
	../../Thistle/Charts/base/abstractlegendview.cpp \
	../../Thistle/Charts/base/seriechart.cpp \
	../../Thistle/Charts/base/linearaxisdelegate.cpp \
	../../Thistle/Charts/base/cartesiancoordinateplane.cpp \
	../../Thistle/Charts/base/cartesiancoordinateplaneview.cpp \
	../../Thistle/Charts/base/seriedelegates.cpp \
	../../Thistle/Charts/base/serieformat.cpp \
	../../Thistle/Charts/scatter/scatterchart.cpp \
	../../Thistle/Charts/scatter/scatterchartcoordinateplaneview.cpp \
	../../Thistle/Charts/scatter/private/scatterchart_p.cpp \
	../../Thistle/Charts/kiviat/kiviatchart.cpp \
	../../Thistle/Charts/kiviat/kiviatcoordinatesystem.cpp \
	../../Thistle/Charts/kiviat/kiviatcoordinatesystemview.cpp \
	../../Thistle/Charts/kiviat/private/kiviatchart_p.cpp \
	../../Thistle/Charts/kiviat/private/kiviatcoordinatesystem_p.cpp \
	../../Thistle/Charts/pie/piechart.cpp \
	../../Thistle/Charts/pie/piechart3d.cpp \
	../../Thistle/Charts/pie/pielegendview.cpp \
	../../Thistle/Charts/pie/private/piechart3d_p.cpp \
	../../Thistle/Charts/pie/private/pielegendview_p.cpp \
	../../Thistle/Charts/pie/private/piechart_p.cpp \
    ../../Thistle/Charts/kiviat/kiviatchartwidget.cpp \
    ../../Thistle/Charts/kiviat/private/kiviatchartwidget_p.cpp \
    ../../Thistle/Charts/scatter/private/scatterchartwidget_p.cpp \
    ../../Thistle/Charts/scatter/scatterchartwidget.cpp \
    ../../Thistle/Charts/pie/piechart3Dwidget.cpp
