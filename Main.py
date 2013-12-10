import Marb
from Marb.Charts import *

from PySide.QtGui import *
from PySide.QtCore import *
from PySide import QtCore
import sys

app = QApplication(sys.argv)

model = QStandardItemModel( 10, 1 )
for i in range( 0, model.rowCount() ):
	model.setData( model.index( i, 0 ), i )

model.setHeaderData( i, Qt.Vertical, "test plus long" )
chart = RadialChart()
chart.setModel( model )
chart.show()

app.exec_()
	
