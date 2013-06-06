import Marb

from PySide.QtGui import QApplication, QStandardItemModel, QWidget, QHBoxLayout, QPainterPath
from PySide.QtCore import QSize, Qt
import sys
import random

import re

if __name__ == "__main__":
	app = QApplication(sys.argv)
	model = QStandardItemModel( 6, 2 )
	for r in range(model.columnCount()):
		model.setHeaderData(r, Qt.Horizontal, "Column "+str(r) )
	for r in range(model.rowCount()):
		model.setHeaderData(r, Qt.Vertical, "Row "+str(r) )
	
	
	i = 0
	for v in (768,1399,82,1902,1575,266):
		model.setData(model.index(i,1), v, Qt.DisplayRole )
		i += 1
	i = 0
	for v in (831,1220,608,1555,1093,1188):
		model.setData(model.index(i,0), v, Qt.DisplayRole )
		i += 1
	

	
	for r in range(model.rowCount()):
		model.setData(model.index(r,0), r, Qt.DisplayRole )
		#print v
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
	
#	w = QWidget()
#	l = QHBoxLayout( w )
#		
#	lc1 = Marb.LinearChart()
#
#	lc1.setModel( model )
#	style = lc1.columnStyle(0)
#	style.setType( Marb.Type.Bar )
#	lc1.setColumnStyle( 0,style )
#	style = lc1.columnStyle(1)
#	style.setType( Marb.Type.Bar )
#	lc1.setColumnStyle( 1,style )
#	l.addWidget( lc1 )
#	
#	lc2 = Marb.LinearChart()
#
#	lc2.setModel( model )
#	l.addWidget( lc2 )
#	
#	lc3 = Marb.LinearChart()
#
#	lc3.setModel( model )
#	style = lc3.columnStyle(0)
#	style.setType( Marb.Type.Bar )
#	lc3.setColumnStyle( 0,style )
#	l.addWidget( lc3 )
#	
#	rc = Marb.RadialChart()
#
#	rc.setModel( model )
#	l.addWidget( rc )
#
#	
#	w.show()

	path = QPainterPath()
	#path.addEllipse( -10, -10, 20, 20 )
	path.moveTo(200, 100);
	path.lineTo( 250, 100 )

	itemPath = QPainterPath()
	itemPath.moveTo(200, 80);
	itemPath.lineTo( 250, 80 )

	t = Marb.Timeline()
	t.setPath( path )
	t.setItemPath( itemPath )
	t.setModel( model )
	t.show()
	
	
	app.exec_()


	
