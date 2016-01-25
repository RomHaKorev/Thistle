QT       += widgets
TEMPLATE = lib

CONFIG(debug, debug|release) {
TARGET = ThGraphsd
}

CONFIG(release, debug|release) {
TARGET = ThGraphs
}


DEFINES += THISTLE_GRAPHS_LIBRARY

QMAKE_CXXFLAGS += -Wno-unused-function -Wall -Werror

unix:DESTDIR = $$PWD/../../../../bin/libs/gcc/
win32:DESTDIR = $$PWD/../../../../bin/libs/mingw32/

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../bin/libs/mingw32/ -lThCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../bin/libs/mingw32/ -lThCored
else:unix:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../bin/libs/gcc/ -lThCored
else:unix:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../bin/libs/gcc/ -lThCore


INCLUDEPATH += $$PWD/../../../
DEPENDPATH += $$PWD/../../../


HEADERS +=  ../../../Thistle/Graphs/edge.h \
			../../../Thistle/Graphs/node.h \
			../../../Thistle/Graphs/graphview.h \
			../../../Thistle/Graphs/graphmodel.h \
			../../../Thistle/Graphs/abstractgraphalgorithm.h \
			../../../Thistle/Graphs/graphalgorithm.h

SOURCES +=  ../../../Thistle/Graphs/abstractgraphalgorithm.cpp \
			../../../Thistle/Graphs/graphalgorithm.cpp \
			../../../Thistle/Graphs/edge.cpp \
			../../../Thistle/Graphs/graphmodel.cpp \
			../../../Thistle/Graphs/graphview.cpp \
			../../../Thistle/Graphs/node.cpp


HEADERS +=  ../../../Thistle/Graphs/private/graphmodel_p.h \
			../../../Thistle/Graphs/private/graphview_p.h \
			../../../Thistle/Graphs/private/node_p.h \
			../../../Thistle/Graphs/private/node_p.h

SOURCES +=  ../../../Thistle/Graphs/private/graphalgorithm_p.cpp \
			../../../Thistle/Graphs/private/graphmodel_p.cpp \
			../../../Thistle/Graphs/private/graphview_p.cpp \
			../../../Thistle/Graphs/private/abstractgraphalgorithm_p.cpp \
			../../../Thistle/Graphs/private/node_p.cpp
