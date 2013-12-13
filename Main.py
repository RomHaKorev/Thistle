import Marb
from Marb.Charts import *

from PySide.QtGui import *
from PySide.QtCore import *
from PySide import QtCore
import sys


app = QApplication(sys.argv)

model = QStandardItemModel( 10, 2 )
for i in range( 0, model.rowCount() ):
	for j in range( 0, model.columnCount() ):
		model.setData( model.index( i, j ), 1 + j )

# model.setHeaderData( i, Qt.Vertical, "test plus long" )
# chart = LinearChart()
# chart.setModel( model )
# chart.show()

chart1 = LinearChart()
chart1.setModel( model )
chart1.show()

app.exec_()