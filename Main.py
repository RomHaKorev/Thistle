import Marb

from PySide.QtGui import QApplication, QStandardItemModel, QWidget, QHBoxLayout, QPainterPath, QPushButton, QRegion, QLabel, QPalette, QColor, QTableWidget, QTableWidgetItem
from PySide.QtCore import QSize, Qt, QRect
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

	model = QStandardItemModel( 14, 2)
	l = ("<center><b>1976</b></center>High Voltage","<center><b>1976</b></center>Dirty Deeds Done Dirt Cheap","<center><b>1977</b></center>Let There Be Rock",
"<center><b>1978</b></center>Powerage","<center><b>1979</b></center>Highway to Hell","<center><b>1980</b></center>Back in Black","<center><b>1981</b></center>For Those About to Rock We Salute You",
"<center><b>1983</b></center>Flick of the Switch","<center><b>1985</b></center>Fly on the Wall","<center><b>1988</b></center>Blow Up Your Video","<center><b>1990</b></center>The Razors Edge",
"<center><b>1995</b></center>Ballbreaker","<center><b>2000</b></center>Stiff Upper Lip","<center><b>2008</b></center>Black Ice")
	for i in range( 14 ):
		model.setData( model.index( i, 0 ), l[i] )
	


	path = QPainterPath()
	
	path.moveTo( 20, 50 )
	path.lineTo( 45, 200 )
	
	itemPath = path.translated( 0, -40 )	
	t = Marb.Timeline()
	t.setItemSize( QRect( -32, -40, 64, 80 ) )
	t.setDistancePolicy( Marb.DistancePolicy.KeepDistance )
	t.setPath( path )
	t.setItemPath( itemPath )
	t.setModel( model )
	t.show()
	
	table = QTableWidget( 1, 2)

	
	
	app.exec_()


	
