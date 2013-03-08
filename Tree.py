from Marb import enum
import Marb

from PySide.QtGui import QAbstractItemView, QPen, QStyledItemDelegate, QBrush, QFont, QPolygon, \
QLineEdit, QRegion, QColor
from PySide.QtCore import QSize, QRect, QPoint, Qt, QModelIndex

class TreeStyle:
    ItemShape = enum(Rectangle = 0, Ellipse   = 1, RoundedRect = 2, Diamond = 3, Triangle = 4, ReversedTriangle = 5 )
    def __init__(self):
        self._link= QPen( QColor(Marb.Color.Gray), 2 )
        self._brush = QBrush( Marb.Color.Blue )
        self._border = QPen( QColor(Marb.Color.DarkBlue), 1.5 )
        self._font = QFont()
        self._textColor = QColor( Marb.Color.White )
        self._shape = TreeStyle.ItemShape.Rectangle
        
    def setLink(self, pen):
        self._link = pen
    
    def setBackground(self, brush):
        self._brush = brush
    
    def setBorder(self, pen):
        self._border = pen
    
    def setFont(self, font):
        self._font = font
        
    def setTextColor(self, color):
        self._textColor = color
    
    def setShape(self, shape):
        self._shape = shape
        
    def link(self):
        return self._link
    
    def background(self):
        return self._brush
    
    def border(self):
        return self._border
    
    def font(self):
        return self._font
        
    def textColor(self):
        return self._textColor
    
    def shape(self):
        return self._shape
        
        
class TreeItemDelegate(QStyledItemDelegate):
    def __init__(self, parent=None):
        super(TreeItemDelegate, self).__init__(parent)
        self._textVisible = True
        self.palette = TreeStyle()
        
            
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
        
        if self.palette.shape() == TreeStyle.ItemShape.Ellipse:
            painter.drawEllipse( r )
        elif self.palette.shape() == TreeStyle.ItemShape.RoundedRect:
            painter.drawRoundedRect( r, 5, 5 )
        elif self.palette.shape() == TreeStyle.ItemShape.Diamond:
            painter.drawPolygon( self.createDiamond( r ) )
        elif self.palette.shape() == TreeStyle.ItemShape.Triangle:
            poly = QPolygon()
            poly.append( r.topLeft() + QPoint( r.width()/2, 0 ) )
            poly.append( r.bottomLeft() )
            poly.append( r.bottomRight() )
            painter.drawPolygon( poly )
        elif self.palette.shape() == TreeStyle.ItemShape.ReversedTriangle:
            poly = QPolygon()
            poly.append( r.bottomLeft() + QPoint( r.width()/2, 0 ) )
            poly.append( r.topLeft() )
            poly.append( r.topRight() )
            painter.drawPolygon( poly )
        else:
            painter.drawRect( r )
        painter.restore()
        
        if self._textVisible == True:
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
        self.centralItemPos = QPoint()
        self.delegate = TreeItemDelegate( self )
        self.setItemDelegate( self.delegate )
        self.itemPos = {}
        self._palette = TreeStyle()
        
    def setTreeStyle(self, palette):
        self._palette = TreeStyle()
        if isinstance(self.itemDelegate(), TreeItemDelegate):
            self.itemDelegate().palette = palette
        
    
    def treeStyle(self):
        return self._palette
    
    def setScrollBarValues(self):
        None 
    
    def resizeEvent(self, ev ):
        self.setScrollBarValues()
        return QAbstractItemView.resizeEvent(self, ev )
    
    def moveCursor(self, cursorAction, modifiers):
        return QModelIndex()
    
    def itemRect(self, index):
        raise "Should be implemented"
    
    def horizontalOffset(self):
        return self.horizontalScrollBar().value()
    
    def verticalOffset(self):
        return self.verticalScrollBar().value()
        
    def indexAt(self, point):
        p = point - self.centralItemPos - QPoint( self.horizontalOffset(), self.verticalOffset() )
        for index in self.itemPos.keys():
            r = self.itemRect(index)
            if r.contains( p ):
                print index.model().data(index)
#                return index
            
        return QModelIndex()
    
    def visualRect(self, index):
        print "visualRect"
        r = self.itemRect(index).translated( self.centralItemPos )
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
       