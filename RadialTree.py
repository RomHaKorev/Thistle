from Tree import Tree

import math
from PySide.QtGui import QPainter, QStyleOptionViewItem, QPainterPath, QPainterPathStroker
from PySide.QtCore import QRect, QPoint, QPointF, Qt

class RadialTree(Tree):
    _max = 0
    _min = 0
    def __init__(self, parent=None):
        super(RadialTree, self).__init__( parent )
        
     
    def setScrollBarValues(self):
        dw = max( [0, (self.realSize.width() - self.width())] )
        dh = max( [0, (self.realSize.height() - self.height()) ] )
        self.horizontalScrollBar().setRange( 0, dw )
        self.verticalScrollBar().setRange( 0, dh )

            
    def treeDepth(self):
        nb = 0
        for r in range( 0, self.model().rowCount() ):
            d = self.itemDepth( model.index(r, 0 ) )
            if d > nb:
                nb = d
        return nb
    
    def treeWidth(self):
        for r in range( 0, self.model().rowCount() ):
            self.itemWidth( self.model().index( r, 0 ) )
    
    def itemWidth(self, index):
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
                self.setX( index, (right + left) / 2 )
        else:
            self.setX( index, left )        
    
    
    def itemDepth(self, index, depth=0):
        if not index.isValid():
            return 0
        if self.model().rowCount( index ) == 0:
            self.setY( index, depth + 1 )
            return 1
        
        childDepth = 0
        for r in range( 0, self.model().rowCount( index ) ):
            child = index.child( r, 0 )
            d = self.itemDepth( child, depth + 1 )
            if d > childDepth:
                childDepth = d
        
        self.setY( index, depth + 1 )
        
        return childDepth + 1
        
        
        
    def setX(self, index, x):
        if not self.itemPos.has_key( index ):
            self.itemPos[ index ] = QPointF()
        self.itemPos[index].setX(x)
    
    
    def setY(self, index, y):
        if not self.itemPos.has_key( index ):
            self.itemPos[ index ] = QPointF()
            
        self.itemPos[index].setY(y)
    
        
    def resolvePositions(self):
        self.left = 0
        self.treeWidth()
        self.depth = self.treeDepth()
        self.left += 1
        self.realSize.setWidth( ( self.left ) * ( self.xDistance + self.rect.width() ) );
        self.realSize.setHeight( self.depth * ( self.yDistance + self.rect.height() ) + 40 )
        self.setScrollBarValues()
        for index in self.itemPos.keys():
            p = self.itemPos[ index ]
            self.itemPos[index] = QPointF( ( 1 + p.x() ) * ( self.xDistance + self.rect.width() ), p.y() * ( self.yDistance + self.rect.height() ) )

    
    def itemRect(self, index):
        if not index.isValid():
            return QRect()
        
        p  = self.itemPos[ index ] - QPointF( self.horizontalOffset(), self.verticalOffset() )
        return self.rect.translated( p.x(), p.y() )
    
        
    def paintEvent(self, event):
        painter = QPainter()
        painter.begin( self.viewport() )
        painter.setClipRect( event.rect() )
        painter.setRenderHint( QPainter.Antialiasing )
        for index in self.itemPos.keys():
            self.paintConnections(painter, index)
        
        for index in self.itemPos.keys():
            option = QStyleOptionViewItem()
            option.rect = self.itemRect( index )
            self.itemDelegate().paint( painter, option, index )
        
        painter.drawRect( QRect(QPoint(0,0), self.realSize) )
        painter.end()
    
    
    def paintConnections(self, painter, index):
        painter.save()
        painter.setPen( self._palette.link() )
        rows = self.model().rowCount( index )
        r = self.itemRect(index)
        if rows > 1:
            child1 = self.itemRect( model.index( 0, 0, index ) )
            child2 = self.itemRect( model.index( rows - 1, 0, index ) )
            p1 = QPointF( child1.center().x(), child1.top() - self.yDistance / 2 );
            p2 = QPointF( child2.center().x(), child2.top() - self.yDistance / 2 );
            painter.drawLine( p1, p2 )
            
            p1 = QPointF( r.center().x(), r.bottom() + 1 );
            p2 = QPointF( r.center().x(), r.bottom() + self.yDistance / 2 );
            painter.drawLine( p1, p2 )
            for i in range( 0, self.model().rowCount(index)):
                r = self.itemRect( self.model().index( i, 0, index ) )
                p1 = QPointF( r.center().x(), r.top() );
                p2 = QPointF( r.center().x(), r.top() - self.yDistance / 2 );
                painter.drawLine( p1, p2 );
                
        elif rows == 1:
            p1 = QPointF( r.center().x(), r.bottom() + 1 );
            p2 = QPointF( p1.x(), p1.y() + self.yDistance - 2 );
            painter.drawLine( p1, p2 );
             
        painter.restore()
    
    
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
    t.show()
    app.exec_()