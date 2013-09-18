import Marb
from Marb.Charts3D.LinearChart3D import LinearChart3D

from PySide.QtGui import QApplication, QStandardItemModel, QStandardItem, QTableView, QPen, QColor, QSlider, QFormLayout, QWidget
from PySide.QtCore import QSize, Qt, QObject, SIGNAL
import sys


from random import randint

chart = None


app = QApplication(sys.argv)

model = QStandardItemModel()
root0 = QStandardItem( "Root0" )

it1 = QStandardItem( "ItemA" )
it2 = QStandardItem( "ItemB" )
it1.appendRow( it2 )
it2 = QStandardItem( "ItemC" )
it1.appendRow( it2 )
it3 = QStandardItem( "ItemD" )
it2.appendRow( it3 )
root0.appendRow( it1 )

root1 = QStandardItem( "Root1" )

it1 = QStandardItem( "Item4" )
it2 = QStandardItem( "Item5" )
it1.appendRow( it2 )
it2 = QStandardItem( "Item6" )
it1.appendRow( it2 )
it3 = QStandardItem( "Item7" )
it1.appendRow( it3 )
root1.appendRow( it1 )

root2 = QStandardItem( "Root2" )
it1 = QStandardItem( "Item8" )
it1.appendRow( QStandardItem( "Item9" ) )
it1.appendRow( QStandardItem( "Item10" ) )
root2.appendRow( it1 )
it1 = QStandardItem( "Item11" )
it2 = QStandardItem( "Item12" )
it2.appendRow( QStandardItem( "Item13" ) )
it1.appendRow( it2 )
root2.appendRow( it1 )

root3 = QStandardItem( "Root2" )
it1 = QStandardItem( "Item14" )
it1.appendRow( QStandardItem( "Item15" ) )
it1.appendRow( QStandardItem( "Item16" ) )
root3.appendRow( it1 )
it1 = QStandardItem( "Item17" )
it2 = QStandardItem( "Item18" )
it2.appendRow( QStandardItem( "Item19" ) )
it1.appendRow( it2 )
root3.appendRow( it1 )

model.appendRow( root0 )
model.appendRow( root1 )
model.appendRow( root2 )
model.appendRow( root3 )

#	tree = Marb.HorizontalTree()
#	tree.setModel( model )
#	tree.show()

color = QColor( 0x0829A1 )
model2 = QStandardItemModel( 10, 3 )
for i in range( model2.rowCount() ):
	for j in range( model2.columnCount() ):
		model2.setData( model2.index( i, j ), randint( 0, 10 ) )
		#model2.setData( model2.index( i, j ), 10 )
		color = color.lighter( 120 )

#model2.setData( model2.index( 1, 0 ), 5 )
#model2.setData( model2.index( 2, 0 ), 5 )

chart = LinearChart3D()
#style = chart.columnStyle( 1 )
#style.setType( Marb.Type.Bar )
#chart.setColumnStyle( 1, style )
#chart.setSplitted( True )
chart.setModel( model2 )
chart.setTitle( "Test legende sous le graphique" )
s = chart.columnStyle(0)
s.setType( Marb.Type.Line )
chart.setColumnStyle( 0, s )
s = chart.columnStyle(2)
s.setType( Marb.Type.Bar )
chart.setColumnStyle( 2, s )
chart.setAngles( 0,0,0 )
chart.show()

#l = QTableView()
#l.setModel( model2 )
#l.show()

#l.setSelectionModel( chart.selectionModel() )

w = QWidget()
form = QFormLayout( w )
sliderX = QSlider()
sliderX.setMinimum( 0 )
sliderX.setMaximum( 90 )
sliderY = QSlider()
sliderY.setMinimum( 0 )
sliderY.setMaximum( 90 )
sliderZ = QSlider()
sliderZ.setMinimum( 0 )
sliderZ.setMaximum( 90 )

form.addRow( "X", sliderX )
form.addRow( "Y", sliderY )
form.addRow( "Z", sliderZ )

#QObject.connect( sliderX, SIGNAL('valueChanged()'), chart.test )

def test():
	chart.setAngles( sliderX.value(), sliderY.value(), sliderZ.value() )

sliderX.valueChanged.connect(test)
sliderY.valueChanged.connect(test)
sliderZ.valueChanged.connect(test)

w.show()

#	t = Marb.Graph()
#	t.setItemSize( QSize( 10, 10 ) )
#	t.setWeight( 3 )
#	t.setConnectionPen( QPen( QColor( Marb.Color.DarkBlue ), 1 ) )
#	
#	t.setModel( model2 )
#	for r1 in range(50):
#		n = randint( 1, 4 )
#		for i in range( n ):
#			r2 = randint( 0, 49 )
#			print( r1, r2 )
#			t.addEdge( r1,0,r2,0 )
#	t.addEdge( 0,0,1,0 )
#	t.addEdge( 1,0,2,0 )
#	t.addEdge( 2,0,3,0 )
#	t.addEdge( 3,0,4,0 )
#	t.addEdge( 3,0,0,0 )
#	t.show()

app.exec_()
	
