from Tree import Tree
from Tree import TreeStyle
from Global import Shape

import math
from PySide.QtGui import QPainterPath
from PySide.QtCore import QRect, QPointF


class RadialTree(Tree):
    _max = 0
    _min = 0
    def __init__(self, parent=None):
        super(RadialTree, self).__init__( parent )
        self._rotateText = False
        self.orderedIndexes = []
        self.rect = QRect( -20, -20, 40, 40 )
        style = TreeStyle()
        style.setShape( Shape.Ellipse )
        self.setTreeStyle( style )
        self.yDistance = 0
        self.xDistance = 40
        
        
    def setScrollBarValues(self):
        dw = max( 0, ( self.realSize.width() - self.width() )/2  )
        dh = max ( 0, ( self.realSize.height() - self.height() )/2 )
        self.horizontalScrollBar().setRange( -dw, dw )
        self.verticalScrollBar().setRange( -dh, dh )
        self.itemOffset = QPointF( self.width() / 2, self.height() / 2 )
         
    def setRotateText(self, rotate = True):
        self._rotateText = rotate
        
    def updatePerimeter(self):
        self.diagonal = math.sqrt( self.rect.width() * self.rect.width() + self.rect.height() * self.rect.height() )
        self.diagonal += max( self.yDistance, self.xDistance )
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
        self.itemPos = {}
        self.orderedIndexes = []
                
        self.left = 0
        self.treeWidth()
        self.depth = self.treeDepth()
        self.left += 1
        
        dist = max(self.xDistance, self.yDistance )
        w = ( self.left ) * ( dist + self.rect.width() )
        self.realSize.setWidth( w );
        self.realSize.setHeight( w )
        self.setScrollBarValues()
        
        self.updatePerimeter()
        
        path = QPainterPath();
        path.addEllipse( self.itemOffset + self.itemRect( self.model().index( 0,0 ) ).center(), self.radius, self.radius );
                
        l = 0
        factor = 0
        for index in self.orderedIndexes:
            if self.model().rowCount( index ) != 0:
                continue
            
            factor = float(self.depth) / float(self.itemPos[index].y())
            
            if self.model().rowCount( index ) == 0:
                l +=  (float(self.diagonal)/2.0) * float(factor)
            
            percent = path.percentAtLength( l )
            self.itemPos[index].setX( percent )

            if self.model().rowCount( index ) == 0:
                l +=  (float(self.diagonal)/2.0) * float(factor)
        
        
        reversedIndexes = self.orderedIndexes
        reversedIndexes.reverse()
        for index in reversedIndexes:
            if self.model().rowCount( index ) == 0:
                continue
            left = self.model().index( 0,0, index )
            right = self.model().index( self.model().rowCount(index) - 1,0, index )
            self.itemPos[index].setX( float(self.itemPos[left].x() + self.itemPos[right].x()) / 2.0 )        
        
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

            self.itemPos[index] = circle.pointAtPercent( self.itemPos[index].x() )
        self.itemPos[self.model().index(0,0)] = QPointF(0, 0)   

