from VerticalTree import VerticalTree
from Global import Shape
from Tree import TreeStyle
from PySide.QtCore import QRect, QPoint, QPointF


class HierarchicalTree(VerticalTree):
    _max = 0
    _min = 0
    def __init__(self, parent=None):
        super(HierarchicalTree, self).__init__( parent )
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
        self.horizontalScrollBar().setRange( 0, dw )
        self.verticalScrollBar().setRange( 0, dh )
        self.centralItemPos = QPointF( self.width() / 2, self.height() / 2 )
        
        
    def scan(self, index, left, depth):
        rows = self.model().rowCount( index )
        self.setX( index, left )
        if not index.isValid():
            return None
        elif rows == 0:
            self.setY(index, depth)
            return (left + 1 , 1)
        
        left += 1
        childDepth = 0
        for r in range( 0, self.model().rowCount( index ) ):
            child = index.child( r, 0 )
            (left, d) = self.scan( child, left, depth + 1 )
            childDepth = max( childDepth, d )
        
        self.setY(index, depth)
        return ( left, childDepth + 1 )
    
         
    def itemWidth(self, index):
        rows = self.model().rowCount( index )
        self.setX( index, self.left )
        self.left += 1
        for r in range( 0, rows ):
            child = index.child( r, 0 )
            self.itemWidth(child)
    
    
    def paintConnectionsFor(self, painter, index):
        for index in self.itemPos.keys():
            parent = self.model().parent( index )
            if parent.isValid():
                pItem = self.itemRect(index).center()
                if index.row() == self.model().rowCount( parent ) - 1:
                    pParent = self.itemRect( parent ).center()
                    jointure = QPoint( pParent.x(), pItem.y() )
                    painter.drawLine( pItem, jointure )
                    painter.drawLine( pParent, jointure )