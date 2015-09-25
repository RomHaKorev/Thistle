#-------------------------------------------------
#
# Project created by QtCreator 2015-07-10T22:40:35
#
#-------------------------------------------------

QT       += core gui declarative quick

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sandbox
TEMPLATE = app


SOURCES += main.cpp \
	thistleproxywidget.cpp

HEADERS  += \
	thistleproxywidget.h

DISTFILES += \
	Sandbox.qml \
	ItemTypeView.qml

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../bin/libs/mingw32/ -lKernel -lCharts
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../bin/libs/mingw32/ -lKerneld -lChartsd
else:unix: LIBS += -L$$PWD/../../../bin/libs/mingw32/ -lKernel -lChartsd

INCLUDEPATH += $$PWD/../../Thistle
DEPENDPATH += $$PWD/../../Thistle
