from PySide.QtGui import QApplication, QStandardItemModel, QStandardItem, QPen, QColor
from PySide.QtCore import QRect
import sys

import Marb

#"Root" 
#"Item1" 
#"Item2" 
#"Item3" 
#"Item4" 
#"Item5" 
#"Item6" 
#"Item7" 
#"Item8"

#Item8
#Item7
#Item6
#Item5
#Item4
#Item3
#Item2
#Item1
#Root

if __name__ == "__main__":
    
    app = QApplication(sys.argv)
    model = QStandardItemModel()
    it0 = QStandardItem( "Root" )
    
    it1 = QStandardItem( "Item1" )
    it2 = QStandardItem( "Item2" )
    it1.appendRow( it2 )
    it2 = QStandardItem( "Item3" )
    it1.appendRow( it2 )
    it3 = QStandardItem( "Item4" )
    it2.appendRow( it3 )
    it0.appendRow( it1 )
    it1 = QStandardItem( "Item5" )
    it2 = QStandardItem( "Item6" )
    it1.appendRow( it2 )
    it2 = QStandardItem( "Item7" )
    it1.appendRow( it2 )
    it3 = QStandardItem( "Item8" )
    it2.appendRow( it3 )
    it0.appendRow( it1 )
#    it1 = QStandardItem( "Item5" )
#    it2 = QStandardItem( "Item7" )
#    it1.appendRow( it2 )
#    it2 = QStandardItem( "Item8" )
#    it1.appendRow( it2 )
#    it3 = QStandardItem( "Item10" )
#    it2.appendRow( it3 )
#    it0.appendRow( it1 )
#    it1 = QStandardItem( "Item5" )
#    it2 = QStandardItem( "Item7" )
#    it1.appendRow( it2 )
#    it2 = QStandardItem( "Item8" )
#    it1.appendRow( it2 )
#    it3 = QStandardItem( "Item10" )
#    it2.appendRow( it3 )
#    it0.appendRow( it1 )
#    it1 = QStandardItem( "Item5" )
#    it2 = QStandardItem( "Item7" )
#    it1.appendRow( it2 )
#    it2 = QStandardItem( "Item8" )
#    it1.appendRow( it2 )
#    it3 = QStandardItem( "Item10" )
#    it2.appendRow( it3 )
#    it0.appendRow( it1 )
#    it1 = QStandardItem( "Item5" )
#    it2 = QStandardItem( "Item7" )
#    it1.appendRow( it2 )
#    it2 = QStandardItem( "Item8" )
#    it1.appendRow( it2 )
#    it3 = QStandardItem( "Item10" )
#    it2.appendRow( it3 )
#    it0.appendRow( it1 )
    model.appendRow( it0 )

    style = Marb.ItemStyle()
    style.setShape( Marb.Shape.Ellipse )
#    style.setDisplayText(False)
    
    r = Marb.HorizontalTree()
    delegate = r.itemDelegate()
    delegate.setItemStyle(style)

    r.setConnectionPen( QPen( QColor(Marb.Color.Blue), 1 ) )
    r.setModel(model)
    r.show()
    r.move( 50, 50 )
#    r.setItemRect(QRect( -5, -5, 10, 10 ))
#    r.setSpacing( 30, 10 )
    
    print r.save( "test.png" )
    
    app.exec_()


    