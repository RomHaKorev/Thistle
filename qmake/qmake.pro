TEMPLATE = subdirs

CONFIG += debug_and_release


SUBDIRS += \
	Kernel \
	Charts \
	Trees \
	Graphs \
	Sandbox

Charts.depends = Kernel
Graphs.depends = Kernel
Trees.depends = Kernel

Sandbox.depends = Kernel Charts Trees Graphs
