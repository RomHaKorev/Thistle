TEMPLATE = subdirs

SUBDIRS += Core\
	Charts \
	Trees \
	Graphs \
	Sandbox \

QMAKE_CXXFLAGS += -Wno-unused-function -Wall
