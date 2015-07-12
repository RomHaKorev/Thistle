TEMPLATE = subdirs

SUBDIRS += Charts \
	Sandbox \
	Trees \
	Graphs \
	Kernel

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../bin/mingw32/ -lCharts
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../bin/mingw32/ -lChartsd
else:unix: LIBS += -L$$PWD/../../bin/mingw32/ -lCharts

INCLUDEPATH += $$PWD/../Thistle/Charts
DEPENDPATH += $$PWD/../Thistle/Charts
