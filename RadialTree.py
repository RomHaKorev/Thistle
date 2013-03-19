from Tree import Tree, TreeItemDelegate
from Tree import ItemStyle
from Global import Shape

import math
from PySide.QtGui import QPainterPath, QPainter, QPen, QPixmap
from PySide.QtCore import QRect, QPointF


class RadialTree(Tree):
    def __init__(self, parent=None):
        super(RadialTree, self).__init__( parent )
        self._rotateText = False
        self.orderedIndexes = []
        self.rect = QRect( -20, -20, 40, 40 )
        style = ItemStyle()
        style.setShape( Shape.Ellipse )
        
        delegate = TreeItemDelegate( self )
        delegate.setItemStyle( style )
        self.setItemDelegate( delegate )
        
        self.yDistance = 40
        self.xDistance = 40
        
        self.radius = 1
        self.diagonal = 1
        
        self._displayCircle = True
        
    
    def displayCicrcle(self, display = True ):
        self._displayCircle = display
        
        
    def setScrollBarValues(self):
        dw = max( 0, ( self.realSize.width() - self.width() )/2  )
        dh = max ( 0, ( self.realSize.height() - self.height() )/2 )
        self.horizontalScrollBar().setRange( -dw/2, dw/2 )
        self.verticalScrollBar().setRange( -dh, dh )
        self.itemOffset = QPointF( self.width() / 2, self.height() / 2 )
        
        
    def updatePerimeter(self):
        w = self.rect.width() + self.xDistance
        h = self.rect.height() + self.yDistance
        self.diagonal = math.sqrt( w * w + h * h )
        self.perimeter = 0.0
        factor = 1
        for index in self.itemTreePos.keys():
            if self.model().rowCount( index ) == 0:
                factor = self.depth / self.itemTreePos[index].y()
                self.perimeter += self.diagonal * factor
        
        self.radius = self.perimeter / ( 2 * math.pi )
        if self.radius * 2 < self.diagonal * self.depth:
            self.radius = (self.diagonal * self.depth)/2 * 1.5
    
    
    def scan(self, index, left, depth):
        self.orderedIndexes.append( index )
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
        right =  self.itemTreePos[ self.model().index( rows - 1, index.column(), index ) ].x();
        if rows >= 2:
            if rows % 2 == 1:
                r = math.floor( rows/2 ) + 1
                v = self.itemTreePos[ self.model().index( r - 1, index.column(), index ) ].x()
                self.setX( index, v )
            else:
                self.setX( index, float(right + left + 1) / 2.0 )
        else:
            self.setX( index, left )

        self.setY(index, depth )
        return ( right + 1 , childDepth + 1 ) 
        
    
    def positionsInTree(self):
        self.itemTreePos = {}
        self.orderedIndexes = []
        
        (self.left, self.depth) = self.scan( self.model().index(0,0) , 0, 0)
        self.left += 1

        self.positionsInView()
        
        
    def positionsInView(self):
        if len(self.itemTreePos.keys()) == 0:
            return None
        
        self.updatePerimeter()
        
        self.realSize.setWidth( ( self.rect.width() + self.radius ) * 2 )
        self.realSize.setHeight( ( self.rect.height() + self.radius ) * 2 )
        self.setScrollBarValues()
        
        self.itemPos = {}
        
        for index in self.itemTreePos.keys():
            p = self.itemTreePos[ index ]
            self.itemPos[ index ] = QPointF( p.x(), p.y() )
        
        path = QPainterPath()
        path.addEllipse( self.itemOffset + self.rect.center(), self.radius, self.radius )
        
        l = 0
        factor = 0
        for index in self.orderedIndexes:
            if self.model().rowCount( index ) != 0:
                continue
            
            factor = float(self.depth) / float(self.itemPos[index].y())
            
            if self.model().rowCount( index ) == 0:
                l +=  (float(self.diagonal)/2.0) * float(factor)
            
            percent = path.percentAtLength( l )
            self.itemPos[index] = QPointF( percent, self.itemPos[index].y() )

            if self.model().rowCount( index ) == 0:
                l +=  (float(self.diagonal)/2.0) * float(factor)
                       
        self.orderedIndexes.reverse()
        for index in self.orderedIndexes:
            if self.model().rowCount( index ) == 0:
                continue
            left = self.model().index( 0,0, index )
            right = self.model().index( self.model().rowCount(index) - 1,0, index )
            self.itemPos[index] = QPointF( float(self.itemPos[left].x() + self.itemPos[right].x()) / 2.0, self.itemPos[index].y() )        
        
        self.itemPos[self.model().index(0,0)] = QPointF(0, 0)
        
        self.orderedIndexes.reverse()
        
        for index in self.orderedIndexes:
            if self.depth == self.itemPos[index].y():
                factor = 1.0
            else:
                if self.itemPos[index].y() != 0:
                    factor = float(self.depth) / float(self.itemPos[index].y())
                else:
                    factor = 1.0

            radius = float(self.radius) / float(factor)
            circle = QPainterPath()
            circle.addEllipse( self.rect.center(), radius, radius )
            
            self.itemPos[index] = circle.pointAtPercent( self.itemPos[index].x() )
            
        self.itemPos[self.model().index(0,0)] = QPointF(0, 0)

        
    def paintConnections(self, painter, offset):
        if self._displayCircle == True:
            self.paintCircles(painter, offset)

        Tree.paintConnections(self, painter, offset)

        
    def paintCircles(self, painter, offset):
        c = self._connectionPen.color()
        c.setAlpha( c.alpha() * 0.15 )
        pen = QPen(self._connectionPen)
        pen.setColor( c )
        pen.setWidth( pen.width() * 2 )
        painter.setPen( pen )
        painter.setRenderHint( QPainter.Antialiasing )
        index = self.model().index(0,0)
        center = self.itemRect(index).translated( offset.x(), offset.y() ).center()
        for rd in range( 1, int(self.depth) + 1 ):
            r = float(self.radius) * rd / self.depth
            painter.drawEllipse( center, r, r )
            
    def save(self, filename ):
        pix = QPixmap( self.realSize )
        pix.fill( Qt.transparent )
        painter = QPainter( pix )
        painter.setRenderHint( QPainter.Antialiasing )
        painter.drawRect( QRect(QPoint(0,0), self.realSize) )
        x = self.realSize.width() / 2
        y = self.realSize.height() / 2
        self.paintConnections( painter, QPointF( x, y ) )
        self.paintItems(painter , QPointF( x, y ) )
        painter.end()
        return pix.save( filename )