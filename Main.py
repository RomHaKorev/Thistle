import Marb

from PySide.QtGui import QApplication, QStandardItemModel, QStandardItem, QPen, QColor
from PySide.QtCore import QSize, Qt
import sys
import random

if __name__ == "__main__":
		
	app = QApplication(sys.argv)
	model = QStandardItemModel( 6, 1 )
	for r in range(model.rowCount()):
		v = random.randint(0, 20 )
		model.setData(model.index(r,0), v, Qt.DisplayRole )
	#model = QStandardItemModel()
	#it0 = QStandardItem( "Root" )
	
	#it1 = QStandardItem( "Item1" )
	#it2 = QStandardItem( "Item2" )
	#it1.appendRow( it2 )
	#it2 = QStandardItem( "Item3" )
	#it1.appendRow( it2 )
	#it3 = QStandardItem( "Item4" )
	#it2.appendRow( it3 )
	#it0.appendRow( it1 )
	#it1 = QStandardItem( "Item5" )
	#it2 = QStandardItem( "Item6" )
	#it1.appendRow( it2 )
	#it2 = QStandardItem( "Item7" )
	#it1.appendRow( it2 )
	#it3 = QStandardItem( "Item8" )
	#it2.appendRow( it3 )
	#it0.appendRow( it1 )
	#it1 = QStandardItem( "Item5" )
	#it2 = QStandardItem( "Item7" )
	#it1.appendRow( it2 )
	#it2 = QStandardItem( "Item8" )
	#it1.appendRow( it2 )
	#it3 = QStandardItem( "Item10" )
	#it2.appendRow( it3 )
	#it0.appendRow( it1 )
	#it1 = QStandardItem( "Item5" )
	#it2 = QStandardItem( "Item7" )
	#it1.appendRow( it2 )
	#it2 = QStandardItem( "Item8" )
	#it1.appendRow( it2 )
	#it3 = QStandardItem( "Item10" )
	#it2.appendRow( it3 )
	#it0.appendRow( it1 )
	#it1 = QStandardItem( "Item5" )
	#it2 = QStandardItem( "Item7" )
	#it1.appendRow( it2 )
	#it2 = QStandardItem( "Item8" )
	#it1.appendRow( it2 )
	#it3 = QStandardItem( "Item10" )
	#it2.appendRow( it3 )
	#it0.appendRow( it1 )
	#it1 = QStandardItem( "Item5" )
	#it2 = QStandardItem( "Item7" )
	#it1.appendRow( it2 )
	#it2 = QStandardItem( "Item8" )
	#it1.appendRow( it2 )
	#it3 = QStandardItem( "Item10" )
	#it2.appendRow( it3 )
	#it0.appendRow( it1 )
	
	#model.appendRow( it0 )

	#style = Marb.TreeStyle()
	#style.setShape( Marb.Shape.Ellipse )
	#style.setDisplayText( False )
	#h = Marb.RadialTree()
##		h.setDirectConnection()
	#delegate = h.itemDelegate()
	#delegate.setTreeStyle(style)
	#h.setConnectionPen( QPen( QColor(Marb.Color.Blue), 2 ) )
	#h.setModel(model)
	#h.show()
	#h.move( 50, 50 )
	#h.setItemSize( QSize( 10, 10 ) )
	#h.setSpacing( 50, 50 )
	
	#h.save("test.png")
	
	t = Marb.RadialChart()
	t.setModel( model ) 
	t.show()
	
	app.exec_()


	
