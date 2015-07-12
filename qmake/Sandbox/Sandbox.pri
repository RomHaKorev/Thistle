QT       += widgets
TEMPLATE = app
TARGET = Sandbox

SOURCES += \
	main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../bin/mingw32/ -lKernel -lCharts
win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../bin/mingw32/ -lKerneld -lChartsd

unix: LIBS += -L$$PWD/../../../bin/linux/ -lKernel -lCharts

INCLUDEPATH += $$PWD/../../
DEPENDPATH += $$PWD/../../
