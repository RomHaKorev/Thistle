from VerticalTree import VerticalTree
from PySide.QtCore import QPoint, QPointF


class HierarchicalTree(VerticalTree):
    _max = 0
    _min = 0
    def __init__(self, parent=None):
        super(HierarchicalTree, self).__init__( parent )

        
    def setScrollBarValues(self):
        dw = max( 0, ( self.realSize.width() - self.width() )/2  )
        dh = max ( 0, ( self.realSize.height() - self.height() )/2 )
        self.horizontalScrollBar().setRange( 0, dw )
        self.verticalScrollBar().setRange( 0, dh )
        self.central_itemPos = QPointF( self.width() / 2, self.height() / 2 )
        
        
    def scan(self, index, _left, _depth):
        rows = self.model().rowCount( index )
        self.setX( index, _left )
        if not index.isValid():
            return None
        elif rows == 0:
            self.setY(index, _depth)
            return (_left + 1 , 1)
        
        _left += 1
        child_depth = 0
        for r in range( 0, self.model().rowCount( index ) ):
            child = index.child( r, 0 )
            (_left, d) = self.scan( child, _left, _depth + 1 )
            child_depth = max( child_depth, d )
        
        self.setY(index, _depth)
        return ( _left, child_depth + 1 )
   
    
    def paintConnectionsFor(self, painter, index, offset):
        for index in self._itemPos.keys():
            parent = self.model().parent( index )
            if parent.isValid():
                pItem = self.itemRect(index).center()
                if index.row() == self.model().rowCount( parent ) - 1:
                    pParent = self.itemRect( parent ).center()
                    jointure = QPoint( pParent.x(), pItem.y() )
                    painter.drawLine( pItem, jointure )
                    painter.drawLine( pParent, jointure )