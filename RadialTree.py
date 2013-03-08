from VerticalTree import VerticalTree

import math
from PySide.QtGui import QPainter, QStyleOptionViewItem, QPainterPath
from PySide.QtCore import QRect, QPoint, QPointF

class RadialTree(VerticalTree):
    _max = 0
    _min = 0
    def __init__(self, parent=None):
        super(RadialTree, self).__init__( parent )
        self._rotateText = False
        self.orderedIndexes = []
        self.test = []
        self.rect = QRect( -20, -20, 40, 40 )
        
    def setScrollBarValues(self):
        dw = max( 0, ( self.realSize.width() - self.width() )/2 + self.verticalScrollBar().width() + 50 );
        dh = max ( 0, ( self.realSize.height() - self.height() )/2 + self.horizontalScrollBar().height() + 50 );
        self.horizontalScrollBar().setRange( -dw, dw );
        self.verticalScrollBar().setRange( -dh, dh );
        self.centralItemPos = QPointF( self.width() / 2, self.height() / 2 );
         
    def setRotateText(self, rotate = True):
        self._rotateText = rotate
        
    def updatePerimeter(self):
        self.diagonal = math.sqrt( self.rect.width() * self.rect.width() + self.rect.height() * self.rect.height() )
        self.perimeter = 0.0
        factor = 1
        for index in self.itemPos.keys():
            if self.model().rowCount( index ) == 0:
                factor = self.depth / self.itemPos[index].y()
                self.perimeter += self.diagonal * factor
        
        self.radius = self.perimeter / ( 2 * math.pi )
        if self.radius * 2 < self.diagonal * self.depth:
            self.radius = (self.diagonal * self.depth)/2 * 1.5
            
        
    def treeWidth(self):
        for r in range( 0, self.model().rowCount() ):
            self.itemWidth( self.model().index( r, 0 ) )
    
    
    def itemWidth(self, index):
        print "order ", index.model().data( index )
        self.orderedIndexes.append( index )
        rows = self.model().rowCount( index )
        if not index.isValid():
            return None
        elif rows == 0:
            self.setX( index, self.left )
            self.left += 1
            return None
        
        for r in range(0, rows):
            child = index.child( r, 0 )
            self.itemWidth( child )
        
        left = self.itemPos[ self.model().index(0, index.column(), index) ].x()
        if rows >= 2:
            if rows % 2 == 1:
                r = math.floor( rows/2 ) + 1
                v = self.itemPos[ self.model().index( r - 1, index.column(), index ) ].x()
                self.setX( index, v )
            else:
                right =  self.itemPos[ self.model().index( rows - 1, index.column(), index ) ].x();
                self.setX( index, float(right + left + 1) / 2.0 )
        else:
            self.setX( index, left )
    
    
    
    def resolvePositions(self):
        self.test= []
        
        self.itemPos = {}
        self.orderedIndexes = []
                
        self.left = 0
        self.treeWidth()
        self.depth = self.treeDepth()
        self.left += 1
        
        self.realSize.setWidth( ( self.left ) * ( self.xDistance + self.rect.width() ) );
        self.realSize.setHeight( self.depth * ( self.yDistance + self.rect.height() ) + 40 )
        self.setScrollBarValues()
        
        self.updatePerimeter()
        
        path = QPainterPath();
        path.addEllipse( self.centralItemPos + self.itemRect( self.model().index( 0,0 ) ).center(), self.radius, self.radius );
                
        l = 0
        factor = 0
        for index in self.orderedIndexes:
            if self.model().rowCount( index ) != 0:
                continue
            
            factor = float(self.depth) / float(len(self.itemPos.keys()))
            
            if self.model().rowCount( index ) == 0:
                l +=  (float(self.diagonal)/2.0) * float(factor)
            
            percent = path.percentAtLength( l )
            print "debugA", index.model().data( index), percent
            self.itemPos[index].setX( percent )

            if self.model().rowCount( index ) == 0:
                l +=  (float(self.diagonal)/2.0) * float(factor)
        
        
        for index in self.itemPos.keys():
            if self.model().rowCount( index ) == 0:
                continue
            left = self.model().index( 0,0, index )
            right = self.model().index( self.model().rowCount(index) - 1,0, index )
            self.itemPos[index].setX( float(self.itemPos[left].x() + self.itemPos[right].x()) / 2.0 )
            print "Debug C : ", index.model().data(index), float(self.itemPos[left].x() + self.itemPos[right].x()) / 2.0
        
        self.itemPos[self.model().index(0,0)] = QPointF(0, 0)
        for index in self.orderedIndexes:
            if self.depth == self.itemPos[index].y():
                factor = 1.0
            else:
                if self.itemPos[index].y() != 0:
                    factor = float(self.depth) / float(self.itemPos[index].y())
                else:
                    factor = 1.0

            circle = QPainterPath()
            circle.addEllipse( self.rect.center(), float(self.radius) / float(factor), float(self.radius) / float(factor) )
            self.test.append( circle )
            print "debug 2",index.row(), index.column(), self.itemPos[index].x()
            self.itemPos[index] = circle.pointAtPercent( self.itemPos[index].x() )
        self.itemPos[self.model().index(0,0)] = QPointF(0, 0)
 
    def paintEvent(self, event):
        painter = QPainter()
        painter.begin( self.viewport() )
        painter.save()
        painter.setClipRect( event.rect() )
        painter.setRenderHint( QPainter.Antialiasing )
        
        for index in self.itemPos.keys():
            option = QStyleOptionViewItem()
            option.rect = self.itemRect( index ).translated( self.centralItemPos.x(), self.centralItemPos.y() )
            self.itemDelegate().paint( painter, option, index )
        
        painter.restore()
        p = QPainterPath()
        p.addEllipse( self.centralItemPos, self.radius, self.radius )      
        painter.drawPath( p )  
        painter.end()
    
    
from PySide.QtGui import QApplication, QStandardItemModel, QStandardItem
import sys

if __name__ == "__main__":
    app = QApplication(sys.argv)
    model = QStandardItemModel()
    it0 = QStandardItem( "Item0" )
    it1 = QStandardItem( "Item1" )
    it0.appendRow( it1 )
    it2 = QStandardItem( "Item2" )
    it0.appendRow( it2 )
    it2 = QStandardItem( "Item3" )
    it0.appendRow( it2 )
    it2 = QStandardItem( "Item4" )
    it0.appendRow( it2 )
    
    it1 = QStandardItem( "Item5" )
    it2 = QStandardItem( "Item7" )
    it1.appendRow( it2 )
    it2 = QStandardItem( "Item8" )
    it1.appendRow( it2 )
    it3 = QStandardItem( "Item10" )
    it2.appendRow( it3 )
    it0.appendRow( it1 )
    
    it1 = QStandardItem( "Item6" )
    it0.appendRow( it1 )
    model.appendRow( it0 )
    

    t = RadialTree()
    t.setModel(model)
    t.resolvePositions()
#    for index in t.itemPos:
#        print index.model().data(index), "->", t.itemPos[index].x(), t.itemPos[index].y()
    t.show()
    
    test = VerticalTree()
    test.setModel(model)
    test.resolvePositions()
    test.show()
    test.move( 800, 50 )
    app.exec_()
