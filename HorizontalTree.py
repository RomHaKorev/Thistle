from VerticalTree import VerticalTree

from PySide.QtCore import QPointF, QPoint, QRect


class HorizontalTree(VerticalTree):
    _max = 0
    _min = 0
    def __init__(self, parent=None):
        super(HorizontalTree, self).__init__( parent )
        self.setDirectConnection(True)
        
        
    def setScrollBarValues(self):
        dw = max( 0, ( self.realSize.width() - self.width() )/2  )
        dh = max ( 0, ( self.realSize.height() - self.height() )/2 )
        self.horizontalScrollBar().setRange( 0, dw )
        self.verticalScrollBar().setRange( -dh/2, dh/2 )
        p = self.itemPos[ self.model().index(0, 0) ]
        self.itemOffset = QPointF( 0, self.height() / 2 - p.y() )
         
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
            self.itemPos[index] = QPointF( ( 1 + p.y() ) * ( self.xDistance + self.rect.width() ), p.x() * ( self.yDistance + self.rect.height() ) )