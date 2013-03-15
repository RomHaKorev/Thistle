from Global import Color, Shape

from PySide.QtGui import QAbstractItemView, QPen, QStyledItemDelegate, QBrush, QFont, QPolygon, \
QLineEdit, QRegion, QColor, QPainter, QStyleOptionViewItem, QPainterPath
from PySide.QtCore import QSize, QRect, QPoint, QPointF, Qt, QModelIndex

class TreeStyle:
    def __init__(self):
        self._connection= QPen( QColor(Color.Gray), 2 )
        self._brush = QBrush( Color.Blue )
        self._border = QPen( QColor( Color.DarkBlue ), 1.5 )
        self._font = QFont()
        self._textColor = QColor( Color.White )
        self._displayText = True
        self._shape = Shape.Rectangle
        
    def setConnection(self, pen):
        self._connection = pen
    
    def setBackground(self, brush):
        self._brush = brush
    
    def setBorder(self, pen):
        self._border = pen
    
    def setFont(self, font):
        self._font = font
        
    def setTextColor(self, color):
        self._textColor = color
    
    def setDisplayText(self, b = True):
        self._displayText = b
    
    def setShape(self, shape):
        self._shape = shape
        
    def connection(self):
        return self._connection
    
    def background(self):
        return self._brush
    
    def border(self):
        return self._border
    
    def font(self):
        return self._font
        
    def textColor(self):
        return self._textColor
    
    def DisplayText(self):
        return self._displayText
    
    def shape(self):
        return self._shape
        
        
class TreeItemDelegate(QStyledItemDelegate):
    def __init__(self, parent=None):
        super(TreeItemDelegate, self).__init__(parent)
        self._textVisible = True
        self.style = TreeStyle()
        
            
    def _createDiamond(self, rect):
        poly = QPolygon()
        poly.append( rect.topLeft() + QPoint(rect.width()/2, 0) )
        poly.append( rect.topRight() + QPoint(0, rect.height()/2) )
        poly.append(rect.bottomLeft() + QPoint( rect.width()/2, 0 ))
        poly.append(rect.topLeft() + QPoint( 0, rect.height()/2 ))
        return poly
    
    
    def createEditor(self, parent, option, index):
        editor = QLineEdit()
        editor.setGeometry( option.rect )
        return editor
    
    
    def setEditorData(self, editor, index):
        if isinstance( editor , QLineEdit):
            editor.setText( index.model().data( index ) )
    
        
    def setModelData(self, editor, model, index):
        if isinstance( editor, QLineEdit):
            model.setData( index, editor.text(), Qt.DisplayRole )
    
    
    def paint(self, painter, option, index):
        painter.save()
        painter.setBrush( self.palette.background() )
        painter.setPen( self.palette.border() )
        r = option.rect
        
        if self.palette.shape() == Shape.Ellipse:
            painter.drawEllipse( r )
        elif self.palette.shape() == Shape.RoundedRect:
            painter.drawRoundedRect( r, 5, 5 )
        elif self.palette.shape() == Shape.Diamond:
            painter.drawPolygon( self.createDiamond( r ) )
        elif self.palette.shape() == Shape.Triangle:
            poly = QPolygon()
            poly.append( r.topLeft() + QPoint( r.width()/2, 0 ) )
            poly.append( r.bottomLeft() )
            poly.append( r.bottomRight() )
            painter.drawPolygon( poly )
        elif self.palette.shape() == Shape.ReversedTriangle:
            poly = QPolygon()
            poly.append( r.bottomLeft() + QPoint( r.width()/2, 0 ) )
            poly.append( r.topLeft() )
            poly.append( r.topRight() )
            painter.drawPolygon( poly )
        else:
            painter.drawRect( r )
        painter.restore()
        
        if self.style.DisplayText() == True:
            painter.setPen( QPen( self.palette.textColor(), 1 ) )
            painter.drawText( r, Qt.AlignCenter, index.model().data(index) )
            
    def updateEditorGeometry(self, editor, option, index):
        None
    

class Tree(QAbstractItemView):
            
    def __init__(self, parent=None):
        print "init tree"
        super(Tree, self).__init__( parent )
        self.realSize = QSize( 200, 200)
        self.rect = QRect( -40, -20, 80, 40 );
        self.yDistance = 50
        self.xDistance = 20
        self.depth = 0
        self.left = 0
        self.connectionPen = QPen()
        self.itemOffset = QPoint()
        self.delegate = TreeItemDelegate( self )
        self.setItemDelegate( self.delegate )
        self.itemPos = {}
        
    def setTreeStyle(self, style):
        self._connectionPen = style.connection()
        if isinstance(self.itemDelegate(), TreeItemDelegate):
            self.itemDelegate().palette = style
    
    def scan(self, index, left, depth):
        raise "Must be implemented. Should resolve the left and depth for each node."
    
    def resolvePositions(self):
        raise "Must be implemented; Should resolve the position (x,y) in px for each node."
    
    def treeStyle(self):
        if isinstance(self.itemDelegate(), TreeItemDelegate):
            return self.itemDelegate().style
        return None
        
    
    def setScrollBarValues(self):
        None 
    
    def resizeEvent(self, ev ):
        self.setScrollBarValues()
        return QAbstractItemView.resizeEvent(self, ev )
    
    def moveCursor(self, cursorAction, modifiers):
        return QModelIndex()
    
    def itemRect(self, index):
        if not index.isValid():
            return QRect()
        
        p  = self.itemPos[ index ] - QPointF( self.horizontalOffset(), self.verticalOffset() )
        return self.rect.translated( p.x(), p.y() )
    
    def horizontalOffset(self):
        return self.horizontalScrollBar().value()
    
    def verticalOffset(self):
        return self.verticalScrollBar().value()
        
    def indexAt(self, point):
        return QModelIndex() # BUG IN PySide
        p = point - self.itemOffset.toPoint() - QPoint( self.horizontalOffset(), self.verticalOffset() )
        for index in self.itemPos.keys():
            r = self.itemRect(index)
            if r.contains( p ):
                None
                print index.model().data(index)
                return index
            
    
    def visualRect(self, index):
        print "visualRect"
        r = self.itemRect(index).translated( self.itemOffset.x(), self.itemOffset.y() )
        return r
    
    def visualRegionForSelection(self, selection):
        print "visualRegionForSelection"
        ranges = selection.count()
        region = QRegion()
        for i in range(0, ranges):
            ran = self.selection.at(i)
            for row in range( ran.top(), ran.bottom() ):
                index = self.model().index( row, 0, self.rootIndex() )
                region += self.visualRect( index )
    
    def rows(self, index):
        return self.model().rowCount( self.model().parent(index))
    
    
    def dataChanged(self, topLeft, bottomRight ):
        QAbstractItemView.dataChanged(self, topLeft, bottomRight)
        self.update( self.model().index(0,0) )
    
        
    def rowsAboutToBeRemoved(self, parent, start, end):
        QAbstractItemView.rowsAboutToBeRemoved(self, parent, start, end)
        self.viewport().update()
        self.setScrollBarValues()
    
    
    def rowsInserted(self, parent, start, end):
        QAbstractItemView.rowsInserted(self, parent, start, end)
        self.viewport().update()
        self.setScrollBarValues()
    
    def setItemRect(self, r):
        self.rect = r 
                
    def setX(self, index, x):
        if not self.itemPos.has_key( index ):
            self.itemPos[ index ] = QPointF()
        self.itemPos[index].setX(x)
    
    
    def setY(self, index, y):
        if not self.itemPos.has_key( index ):
            self.itemPos[ index ] = QPointF()
            
        self.itemPos[index].setY(y)
        
    def paintEvent(self, event):
        painter = QPainter( self.viewport() )
#        painter.setClipRect( event.rect() )
        painter.setRenderHint( QPainter.Antialiasing )
        self.paintConnections(painter)
        self.paintItems(painter)
        
#        circle = QPainterPath()
#        circle.addEllipse( self.rect.center(), float(self.radius), float(self.radius) )
#        r = QRect( 0, 0, self.radius * 2, self.radius * 2 )
#        circle.moveTo( self.radius * 2, self.radius )
#        circle.arcTo( r, 0, -180 )
#        painter.setPen( Qt.black )
#        painter.drawPath( circle.translated( self.itemOffset.x() - self.radius, self.itemOffset.y()  - self.radius ) )
#        painter.drawRect( r.translated( self.itemOffset.x(), self.itemOffset.y() ) )
        
        
    def paintItems(self, painter):
        for index in self.itemPos.keys():
            option = QStyleOptionViewItem()
            option.rect = self.itemRect( index ).translated( self.itemOffset.x(), self.itemOffset.y() )
            self.itemDelegate().paint( painter, option, index )
    
    def paintConnections(self, painter):
        painter.save()
        painter.setPen( self._connectionPen )
        for index in self.itemPos.keys():
            self.paintConnectionsFor(painter, index)
        painter.restore()
    
    def paintConnectionsFor(self, painter, index):
        parent = self.model().parent( index )
        if parent.isValid():
            p1 = self.itemRect(index).translated( self.itemOffset.x(), self.itemOffset.y() ).center()
            p2 = self.itemRect(parent).translated( self.itemOffset.x(), self.itemOffset.y() ).center()
            painter.drawLine( p1, p2 )