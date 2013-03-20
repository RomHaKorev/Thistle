from VerticalTree import VerticalTree

from PySide.QtCore import QPointF, QSize
class HorizontalTree(VerticalTree):
    _max = 0
    _min = 0
    def __init__(self, parent=None):
        super(HorizontalTree, self).__init__( parent )
        self.setDirectConnection(True)
        
        
    def setScrollBarValues(self):
        s = self.realSize + QSize(50, 50) # Margins + scrollbar width/height
        dw = max( 0, ( s.width() - self.width() )  )
        dh = max ( 0, ( s.height() - self.height() )/2 )
        self.horizontalScrollBar().setRange( 0, dw )
        self.verticalScrollBar().setRange( 0, dh )
        
        index = self.model().index(0, 0)
        if self.model() != None and index in self.itemPos and dh != 0:
            p = self.itemPos[ index ]
            self.itemOffset = QPointF( 10, self.height() / 2 - p.y() )        

    def positionsInView(self):
        self.realSize.setWidth( ( self.depth ) * ( self.xDistance + self.rect.width() ) + self.rect.width() )
        self.realSize.setHeight( self.left * ( self.yDistance + self.rect.height() ) + self.rect.height() )

        self.setScrollBarValues()
        
        offset = QPointF( self.rect.width()/2, self.rect.height()/2 )
        for index in self.itemTreePos.keys():
            p = self.itemTreePos[ index ]
            self.itemPos[index] = QPointF( p.y() * ( self.xDistance + self.rect.width() ), p.x() * ( self.yDistance + self.rect.height() ) ) + offset