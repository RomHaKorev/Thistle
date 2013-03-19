from VerticalTree import VerticalTree

from PySide.QtCore import QPointF, Qt, QPoint, QRect
from PySide.QtGui import QPixmap, QPainter
class HorizontalTree(VerticalTree):
    _max = 0
    _min = 0
    def __init__(self, parent=None):
        super(HorizontalTree, self).__init__( parent )
        self.setDirectConnection(True)
        
        
    def setScrollBarValues(self):
        dw = max( 0, ( self.realSize.width() - self.width() )  )
        dh = max ( 0, ( self.realSize.height() - self.height() )/2 )
        self.horizontalScrollBar().setRange( 0, dw )
        self.verticalScrollBar().setRange( -dh/2, dh/2 )
        
        index = self.model().index(0, 0)
        if self.model() != None and index in self.itemPos:
            p = self.itemPos[ index ]
            self.itemOffset = QPointF( -self.rect.x() + 20, self.height() / 2 - p.y() )
        
#        def positionsInView(self):
#        self.realSize.setWidth( ( self.left + 1 ) * ( self.xDistance + self.rect.width() ) + 40 );
#        self.realSize.setHeight( self.depth * ( self.yDistance + self.rect.height() ) + 40 )
#        
#        self.setScrollBarValues()
#        
#        for index in self.itemTreePos.keys():
#            p = self.itemTreePos[ index ]
#            self.itemPos[index] = QPointF( ( 1 + p.x() ) * ( self.xDistance + self.rect.width() ), p.y() * ( self.yDistance + self.rect.height() ) )
         

    def positionsInView(self):
        self.realSize.setWidth( ( self.depth + 1.5 ) * ( self.xDistance + self.rect.width() ) );
        self.realSize.setHeight( ( self.left +1.5 ) * ( self.yDistance + self.rect.height() ) + 40 )
        self.setScrollBarValues()
        
        for index in self.itemTreePos.keys():
            p = self.itemTreePos[ index ]
            self.itemPos[index] = QPointF( p.y() * ( self.xDistance + self.rect.width() ), p.x() * ( self.yDistance + self.rect.height() ) )
            
    
    def save(self, filename ):
        pix = QPixmap( self.realSize )
        pix.fill( Qt.transparent )
        painter = QPainter( pix )
        painter.setRenderHint( QPainter.Antialiasing )
        painter.drawRect( QRect(QPoint(0,0), self.realSize) )
        x = self.realSize.width() / 2
        y = 0
        self.paintConnections( painter, QPointF( x, y ) )
        self.paintItems(painter , QPointF( x, y ) )
        painter.end()
        return pix.save( filename )
