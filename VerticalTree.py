from Tree import Tree

import math
from PySide.QtCore import QPointF, Qt, QPoint, QRect
from PySide.QtGui import QPixmap, QPainter

class VerticalTree(Tree):
    _max = 0
    _min = 0
    _directLink = False
    
    def __init__(self, parent=None):
        super(VerticalTree, self).__init__( parent )
        
                
    def setDirectConnection(self, direct = True):
        self._directLink = direct
     
     
    def setScrollBarValues(self):
        dw = max( [0, (self.realSize.width() - self.width())] )
        dh = max( [0, (self.realSize.height() - self.height()) ] )
        self.horizontalScrollBar().setRange( -dw/2, dw/2 )
        self.verticalScrollBar().setRange( 0, dh )
        
        if self.model() == None:
            return None
        
        index = self.model().index(0,0)
        if index in self.itemPos:
            p = self.itemPos[ index ]
            self.itemOffset = QPointF( self.width() / 2 - p.x(), 0 )


    def scan(self, index, left, depth):
        rows = self.model().rowCount( index )
        if not index.isValid():
            return 0
        elif rows == 0:
            self.setX( index, left )
            self.setY(index, depth + 1)
            return (left + 1, 1)
        
        childDepth = 0
        for r in range( 0, self.model().rowCount( index ) ):
            child = index.child( r, 0 )
            (left, d) = self.scan( child, left, depth + 1 )
            childDepth = max( childDepth, d )
        
        left = self.itemTreePos[ self.model().index(0, index.column(), index) ].x()
        right = self.itemTreePos[ self.model().index( rows - 1, index.column(), index ) ].x()
        if rows >= 2:
            if rows % 2 == 1:
                r = math.floor( rows/2 ) + 1
                v = self.itemTreePos[ self.model().index( r - 1, index.column(), index ) ].x()
                self.setX( index, v )
            else:
                self.setX( index, (right + left) / 2 )
        else:
            self.setX( index, left )
            
        self.setY(index, depth)
        return ( right + 1 , childDepth + 1 ) 
            
        
    def positionsInTree(self):
        self.itemTreePos = {}
        (self.left, self.depth) = self.scan( self.model().index(0,0) , 0, 0)
            
        self.positionsInView()
        
    def positionsInView(self):
        self.realSize.setWidth( ( self.left + 1 ) * ( self.xDistance + self.rect.width() ) + 40 );
        self.realSize.setHeight( self.depth * ( self.yDistance + self.rect.height() ) + 40 )
        
        self.setScrollBarValues()
        
        for index in self.itemTreePos.keys():
            p = self.itemTreePos[ index ]
            self.itemPos[index] = QPointF( ( 1 + p.x() ) * ( self.xDistance + self.rect.width() ), p.y() * ( self.yDistance + self.rect.height() ) )
    
    
    def paintConnectionsFor(self, painter, index, offset ):
        painter.save()
        painter.setPen( self._connectionPen )
        if self._directLink == True:
            super(VerticalTree, self).paintConnectionsFor( painter, index, offset )
        else:
            rows = self.model().rowCount( index )
            r = self.itemRect(index).translated( offset.x(), offset.y() )
            if rows > 1:
                child1 = self.itemRect( self.model().index( 0, 0, index ) ).translated( offset.x(), offset.y() )
                child2 = self.itemRect( self.model().index( rows - 1, 0, index ) ).translated( offset.x(), offset.y() )
                p1 = QPointF( child1.center().x(), child1.top() - self.yDistance / 2 )
                p2 = QPointF( child2.center().x(), child2.top() - self.yDistance / 2 )
                painter.drawLine( p1, p2 )
                
                p1 = QPointF( r.center().x(), r.bottom() + 1 )
                p2 = QPointF( r.center().x(), r.bottom() + self.yDistance / 2 )
                painter.drawLine( p1, p2 )
                for i in range( 0, self.model().rowCount(index)):
                    r = self.itemRect( self.model().index( i, 0, index ) ).translated( offset.x(), offset.y() )
                    p1 = QPointF( r.center().x(), r.top() )
                    p2 = QPointF( r.center().x(), r.top() - self.yDistance / 2 )
                    painter.drawLine( p1, p2 )
                    
            elif rows == 1:
                p1 = QPointF( r.center().x(), r.bottom() + 1 )
                p2 = QPointF( p1.x(), p1.y() + self.yDistance - 2 )
                painter.drawLine( p1, p2 )
        painter.restore()
        
    def save(self, filename ):
        pix = QPixmap( self.realSize )
        pix.fill( Qt.transparent )
        painter = QPainter( pix )
        painter.setRenderHint( QPainter.Antialiasing )
        self.paintConnections( painter, QPointF( 0, 0 ) )
        self.paintItems(painter , QPointF( 0, 0 ) )
        painter.end()
        return pix.save( filename )
