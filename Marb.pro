#-------------------------------------------------
#
# Project created by QtCreator 2013-06-29T09:54:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Marb
TEMPLATE = app


SOURCES += main.cpp\
    Trees/verticaltree.cpp \
    Trees/tree.cpp \
    Trees/radialtree.cpp \
    Trees/horizontaltree.cpp \
    Charts/radialchart.cpp \
    Charts/piechart3D.cpp \
    Charts/piechart.cpp \
    Charts/linearchart.cpp \
    Charts/delegates.cpp \
    Charts/chartstyle.cpp \
    Charts/chart.cpp \
    Graphs/graph.cpp \
	Graphs/node.cpp \
    marbitemdelegate.cpp \
    marbabstractitemview.cpp

HEADERS  += Marb.h \
    Trees/verticaltree.h \
    Trees/tree.h \
    Trees/radialtree.h \
    Trees/horizontaltree.h \
    Charts/radialchart.h \
    Charts/piechart3D.h \
    Charts/piechart.h \
    Charts/linearchart.h \
    Charts/delegates.h \
    Charts/chartstyle.h \
    Charts/chart.h \
    Graphs/graph.h \
	Graphs/node.h \
    marbitemdelegate.h \
    marbabstractitemview.h
