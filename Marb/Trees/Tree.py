
# This file is part of Marb.
# 
#     Marb is free software: you can redistribute it and/or modify
#     it under the terms of the Lesser GNU General Public License as published by
#     the Free Software Foundation, either version 3 of the License.
# 
#     Marb is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#     Lesser GNU General Public License for more details.
# 
#     You should have received a copy of the Lesser GNU General Public License
#     along with Marb.  If not, see <http://www.gnu.org/licenses/>.
# 
# Marb  Copyright (C) 2013  Dimitry Ernot



from ..Global import Color, Shape

from PySide.QtGui import QAbstractItemView, QPen, QStyledItemDelegate, QBrush, QFont, QPolygon, \
QLineEdit, QRegion, QColor, QPainter, QStyleOptionViewItem
from PySide.QtCore import QSize, QRect, QPoint, QPointF, Qt, QModelIndex

class TreeStyle:
		def __init__(self):
				self._brush = QBrush( Color.Blue )
				self._border = QPen( QColor( Color.DarkBlue ), 1.5 )
				self._font = QFont()
				self._textColor = QColor( Qt.white )
				self._displayText = True
				self._shape = Shape.Rectangle
				self.itemOffset = QPointF(0,0)
		
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
		
					 
		def background(self):
				return self._brush
		
		
		def border(self):
				return self._border
		
		
		def font(self):
				return self._font
		
				
		def textColor(self):
				return self._textColor
		
		
		def displayText(self):
				return self._displayText
		
		
		def shape(self):
				return self._shape
				
				
class TreeItemDelegate(QStyledItemDelegate):
		def __init__(self, parent=None):
				super(TreeItemDelegate, self).__init__(parent)
				self._textVisible = True
				self.TreeStyle = TreeStyle()
		
		
		def setTreeStyle(self, style):
				self.TreeStyle = style
		
						
		def _createDiamond(self, rect):
				poly = QPolygon()
				poly.append( rect.top_left() + QPoint(rect.width()/2, 0) )
				poly.append( rect.topRight() + QPoint(0, rect.height()/2) )
				poly.append(rect.bottom_left() + QPoint( rect.width()/2, 0 ))
				poly.append(rect.top_left() + QPoint( 0, rect.height()/2 ))
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
				painter.setBrush( self.TreeStyle.background() )
				painter.setPen( self.TreeStyle.border() )
				r = option.rect
				
				if self.TreeStyle.shape() == Shape.Ellipse:
						painter.drawEllipse( r )
				elif self.TreeStyle.shape() == Shape.RoundedRect:
						painter.drawRoundedRect( r, 5, 5 )
				elif self.TreeStyle.shape() == Shape.Diamond:
						painter.drawPolygon( self.createDiamond( r ) )
				elif self.TreeStyle.shape() == Shape.Triangle:
						poly = QPolygon()
						poly.append( r.topLeft() + QPoint( r.width()/2, 0 ) )
						poly.append( r.bottomLeft() )
						poly.append( r.bottomRight() )
						painter.drawPolygon( poly )
				elif self.TreeStyle.shape() == Shape.ReversedTriangle:
						poly = QPolygon()
						poly.append( r.bottomLeft() + QPoint( r.width()/2, 0 ) )
						poly.append( r.topLeft() )
						poly.append( r.topRight() )
						painter.drawPolygon( poly )
				else:
						painter.drawRect( r )
				
				if self.TreeStyle.displayText() == True:
						painter.setPen( QPen( self.TreeStyle.textColor(), 1 ) )
						flags = Qt.AlignCenter
						if self.TreeStyle.shape() == Shape.Triangle:
								flags = Qt.AlignBottom | Qt.AlignHCenter
						elif self.TreeStyle.shape() == Shape.ReversedTriangle:
								flags = Qt.AlignTop | Qt.AlignHCenter
						
						painter.drawText( r, flags, index.model().data(index) )
				
				painter.restore()
			
						
		def updateEditorGeometry(self, editor, option, index):
				None
		

class Tree(QAbstractItemView):					
		def __init__(self, parent=None):
				super(Tree, self).__init__( parent )
				self._realSize = QSize( 200, 200)
				self._rect = QRect( -40, -20, 80, 40 );
				self._yDistance = 50
				self._xDistance = 20
				self._depth = 0
				self._left = 0
				self._connectionPen = QPen( QColor(Color.LightGray), 2 )
				self.itemOffset = QPoint()
				delegate = TreeItemDelegate( self )
				self.setItemDelegate( delegate )
				self._itemPos = {}
				self._itemTreePos = {}
				
				
		def setSpacing(self, horizontal, vertical ):
				self._xDistance = horizontal
				self._yDistance = vertical
				self._positionsInView()
				self.update()
				
						
		def setConnectionPen(self, pen ):
				self._connectionPen = pen
		
		
		def connectionPen(self):
				return self._connectionPen
		
		
		def scan(self, index, _left, _depth):
				raise( "Must be implemented. Should resolve the _left and _depth for each node." )
		
		
		def _positionsInTree(self):
				raise( "Must be implemented. Should resolve the position (x,y) in the tree for each node." )
		
		def _positionsInView(self):
				raise( "Must be implemented. Should resolve the position (x, y) in pixel in viewport for each node." )
				
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
				p	= self._itemPos[ index ] - QPointF( self.horizontalOffset(), self.verticalOffset() )
				return self._rect.translated( p.x(), p.y() )
		
		
		def horizontalOffset(self):
				return self.horizontalScrollBar().value()
		
		
		def verticalOffset(self):
				return self.verticalScrollBar().value()
			
				
		def indexAt(self, point):
				return QModelIndex() # BUG IN PySide
				p = point - self.itemOffset.toPoint() - QPoint( self.horizontalOffset(), self.verticalOffset() )
				for index in self._itemPos.keys():
						r = self.itemRect(index)
						if r.contains( p ):
								None
								return index
						
		
		def visualRect(self, index):
				r = self.itemRect(index).translated( self.itemOffset.x(), self.itemOffset.y() )
				return r
		
		
		def visualRegionForSelection(self, selection):
				ranges = selection.count()
				region = QRegion()
				for i in range(0, ranges):
						ran = self.selection.at(i)
						for row in range( ran.top(), ran.bottom() ):
								index = self.model().index( row, 0, self.rootIndex() )
								region += self.visualRect( index )
		
		
		def rows(self, index):
				return self.model().rowCount( self.model().parent(index))
		
		
		def setModel(self, *args, **kwargs):
				QAbstractItemView.setModel(self, *args, **kwargs)
				self._positionsInTree()
		
		
		def dataChanged(self, top_left, bottomRight ):
				QAbstractItemView.dataChanged(self, top_left, bottomRight)
				self._positionsInTree()
				self.update( self.model().index(0,0) )
		
				
		def rowsAboutToBeRemoved(self, parent, start, end):
				QAbstractItemView.rowsAboutToBeRemoved(self, parent, start, end)
				self._positionsInTree()
				self.viewport().update()
				self.setScrollBarValues()
		
		
		def rowsInserted(self, parent, start, end):
				QAbstractItemView.rowsInserted(self, parent, start, end)
				self._positionsInTree()
				self.viewport().update()
				self.setScrollBarValues()
		
		
		def setItemSize(self, size ):
				self._rect = QRect( -size.width()/2, -size.height()/2, size.width(), size.height() )
#				self._rect = r
				self._positionsInView()
				self.viewport().update()
		
								
		def setX(self, index, x):
				if not index in self._itemTreePos:
						self._itemTreePos[ index ] = QPointF()
				self._itemTreePos[index].setX(x)
		
		
		def setY(self, index, y):
				if not index in self._itemTreePos:
						self._itemTreePos[ index ] = QPointF()
						
				self._itemTreePos[index].setY(y)
				
				
		def paintEvent(self, event):
				painter = QPainter( self.viewport() )
				painter.setClipRect( event.rect() )
				painter.setRenderHint( QPainter.Antialiasing )
				self.paintConnections( painter, self.itemOffset )
				self.paintItems( painter, self.itemOffset )
				
		def paintItems( self, painter, offset ):
				for index in self._itemPos.keys():
						option = QStyleOptionViewItem()
						option.rect = self.itemRect( index ).translated( offset.x(), offset.y() )
						self.itemDelegate().paint( painter, option, index )
		
		
		def paintConnections( self, painter, offset ):
				painter.save()
				painter.setPen( self._connectionPen )
				for index in self._itemPos.keys():
						self.paintConnectionsFor( painter, index, offset )
				painter.restore()
		
		
		def paintConnectionsFor( self, painter, index, offset ):
				painter.save()
				parent = self.model().parent( index )
				if parent.isValid():
						p1 = self.itemRect(index).translated( offset.x(), offset.y() ).center()
						p2 = self.itemRect(parent).translated( offset.x(), offset.y() ).center()
						painter.drawLine( p1, p2 )
				painter.restore()
					

		def save(self, filename ):
				raise( "Must be implemented. Save the graph in a file named 'filename'" )
