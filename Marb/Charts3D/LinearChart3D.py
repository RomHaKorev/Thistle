from ..Global import Color, Render, Type
from ..Charts.Chart import ChartStyle
from ..Charts.Delegates import PointDelegate, BarDelegate

from PySide.QtGui import QColor, QPainter, QPainterPath, QPolygonF, QStyleOptionViewItem, QTransform, QMatrix4x4, QVector3D, QPen, QBrush 
from PySide.QtCore import QRectF, QRect, QPoint, QPointF, Qt, QLineF

from ..Charts.LinearChart import LinearChart

import math

class LinearChart3D( LinearChart ):
	def __init__( self, parent = None ):
		super(LinearChart3D, self).__init__( parent )
		self._depth = 100


	def itemRect(self, index ):	
		'''Overloaded method.
		'''
		r = QRect()
		t = self.columnType( index.column() )
		value = index.data()
		if value == None:
			return QRect()
		
		y = self.valueToPx(value)
		x = self._origin.x() + index.row() *  self._x	
		
		space = self._x * 0.2
		orderedColumns = self._calculateOrderedColumn()
		
		if t == Type.Bar:
			bars = self._barStyleColumns()
			w = float(self._x) / len(bars)
			x += w * orderedColumns.index( index.column() )
			tl = QPointF( x + space/2, y )             # top left
			br = QPointF( x + w, self._origin.y() )    # bottom right
			r = QRectF( tl, br )
			if value < 0:
				r.translate( 0, 1 )
			else:
				r.translate( 0, -2 )
		else:
			r = QRectF( -5, -5, 10 ,10 ).translated( x + self._x/2, y ) 
		return r.normalized()


	def paintChart(self, painter):
		'''Overloaded method.
		'''
		painter.setRenderHints( QPainter.Antialiasing | QPainter.TextAntialiasing )
		for c in range(0, self.model().columnCount()):
			self._paintValues( painter, c )
			
	
	def _3DPoints(self, point, level ):
		m = level * ( self._depth + 20 )
		center = QVector3D(point.x(), point.y(), 0)
		p1 = QVector3D(point.x(), point.y(), -self._depth/2 + m )
		p2 = QVector3D(point.x(), point.y(), self._depth/2 + m )
		
		m = QMatrix4x4()
		m.rotate(20, 0, 1.0, 0);
		m.rotate(10, 1.0, 0, 0);
				
		p1 -= center
		p1 *= m
		p1 += center
		p2 -= center
		p2 *= m
		p2 += center
		
		return (p1.toPointF(), p2.toPointF())
		
		
	def _calculatePoint(self, column):
		rows = self.model().rowCount()

		frontPoints = []
		backPoints = []
		for r in range( 0, rows ):
			index = self.model().index( r, column )
			p =  self.itemRect( index ).center()
			(p1, p2) = self._3DPoints(p, column)
			frontPoints.append( p1 )
			backPoints.append( p2 )
		
		return (frontPoints, backPoints)
	
	def rectToCube(self, rect):
		
		p1 = rect.topLeft()
		p2 = rect.topRight()
		p3 = rect.bottomLeft()
		p4 = rect.bottomRight()
		
		pTL_F = QVector3D( p1.x(), p1.y(), self._depth )
		pTR_F = QVector3D( p2.x(), p2.y(), self._depth )
		pBL_F = QVector3D( p3.x(), p3.y(), self._depth )
		pBR_F = QVector3D( p4.x(), p4.y(), self._depth )
		
		pTL_B = QVector3D( p1.x(), p1.y(), -self._depth )
		pTR_B = QVector3D( p2.x(), p2.y(), -self._depth )
		pBL_B = QVector3D( p3.x(), p3.y(), -self._depth )
		pBR_B = QVector3D( p4.x(), p4.y(), -self._depth )		
		
		return ( pTL_F, pTR_F, pBL_F, pBR_F, pTL_B, pTR_B, pBL_B, pBR_B )
	
	def _2DTo3D(self, points):
		pts3D_front = []
		for pt in points:
			pt3D = QVector3D( pt.x(), pt.y(), self._depth )
			pts3D_front.append(pt3D)
			
		pts3D_back = []
		for pt in points:
			pt3D = QVector3D( pt.x(), pt.y(), self._depth )
			pts3D_back.append(pt3D)
		return (pts3D_front, pts3D_back)
	
	def _paintValues(self, painter, column ):		
		center = QVector3D( self._chartRect.center().x(), self._chartRect.center().y(), 0 )
		
		def boundedLine( bound1, bound2, line ):
			_, intersectPoint = bound1.intersect( line )
			p1 = intersectPoint
			
			_, intersectPoint = bound2.intersect( line )
			p2 = intersectPoint
			return QLineF( p1, p2 )
		
		def rotate( p, rev = False, scale = None ):
			m = QMatrix4x4()
			if scale != None:
				m.scale( scale, scale, scale )						
			if rev == True:
				m.rotate(-20, 0, 1, 0)
				m.rotate(-20, 0, 0, -1)
				m.rotate(-45, 1, 0, 0)				
			else:
				m.rotate(45, 1.0, 0, 0)
				m.rotate(20, 0, 0, -1.0)
				m.rotate(20, 0, 1.0, 0)
			p -= center
			p *= m
			p += center
			return p
			
		#( pTL_F, pTR_F, pBL_F, pBR_F, pTL_B, pTR_B, pBL_B, pBR_B ) = self.rectToCube(self._chartRect)	
		( _, _, _, pBR_F, pTL_B, _, _, _ ) = self.rectToCube(self._chartRect)
		
		lT = QLineF( self._chartRect.topLeft(),
					self._chartRect.topRight() )
		
		lB = QLineF( self._chartRect.bottomLeft(),
					self._chartRect.bottomRight() )
		
#		lL = QLineF( lT.p1(), lB.p1() )
#		
#		lR = QLineF( lT.p2(), lB.p2() )
#		
#		flatD1 = QLineF( self._chartRect.topLeft(),
#						self._chartRect.bottomRight(), ) 
		
		
		d1 = QLineF( rotate( pTL_B ).toPointF(),
					 rotate( pBR_F ).toPointF() ) 
		
		boundedD1 = boundedLine(lT, lB, d1)
		
#		painter.setPen( Qt.blue )
#		painter.drawLine( lT )
#		painter.drawLine( lB )
#		painter.drawLine( lL )
#		painter.drawLine( lR )
#		painter.setPen( Qt.red )
#		painter.drawLine( d1 )
#		
#		painter.setPen( Qt.green )
#		painter.drawLine( d1 )
#		
#		painter.setPen( QPen( Qt.green, 3 ) )
#		painter.drawLine( boundedD1 )
#		
#		painter.setPen( Qt.black )
#		painter.drawLine( flatD1 )
		
		scale = float( boundedD1.length() / d1.length() )
		( pTL_F, pTR_F, pBL_F, pBR_F, pTL_B, pTR_B, pBL_B, pBR_B ) = self.rectToCube(self._chartRect)
		l = []
		for pt in ( pTL_F, pTR_F, pBL_F, pBR_F, pTL_B, pTR_B, pBL_B, pBR_B ):
			l.append( rotate(QVector3D(pt), False, scale) )
		( _pTL_F, _pTR_F, _pBL_F, _pBR_F, _pTL_B, _pTR_B, _pBL_B, _pBR_B ) = l
		
		painter.setBrush( Qt.gray )
		polygon = QPolygonF()
		polygon.append( _pTL_B.toPointF() )
		polygon.append( _pTR_B.toPointF() )
		polygon.append( _pBR_B.toPointF() )
		polygon.append( _pBL_B.toPointF() )
		painter.drawPolygon( polygon )
		
		painter.setBrush( Qt.lightGray )
		polygon = QPolygonF()
		polygon.append( _pTL_F.toPointF() )
		polygon.append( _pTR_F.toPointF() )
		polygon.append( _pBR_F.toPointF() )
		polygon.append( _pBL_F.toPointF() )
		painter.drawPolygon( polygon )
		
#		
#		_, newCenter = boundedD1.intersect( boundedD2 )
#		
#		newCenter = QVector3D( newCenter.x(), newCenter.y(), 0 )
#		
#		painter.setPen( Qt.blue )
#		painter.drawLine( lT )
#		painter.drawLine( lB )
#		painter.drawLine( lL )
#		painter.drawLine( lR )
#		
#		painter.setPen( Qt.red )
#		painter.drawLine( boundedD1 )
#		painter.drawLine( boundedD2 )
#		painter.setPen( QPen( Qt.green, 5 ) )
#		painter.drawPoint( newCenter.toPointF() )
#		painter.setPen( QPen( Qt.black, 5 ) )
#		painter.drawPoint( center.toPointF() )
#		
#		
#		p1 = rotate( QVector3D( boundedD1.p1().x(), boundedD1.p1().y(), -50 ), True, newCenter ).toPointF()
#		p2 = rotate( QVector3D( boundedD1.p2().x(), boundedD1.p2().y(), 50 ), True, newCenter ).toPointF()
#		
#		p3 = rotate( QVector3D( boundedD2.p1().x(), boundedD2.p1().y(), -50 ), True, newCenter ).toPointF()
#		p4 = rotate( QVector3D( boundedD2.p2().x(), boundedD2.p2().y(), -50 ), True, newCenter ).toPointF()
#		
#		painter.setPen( QPen( Qt.blue, 5 ) )
#		painter.drawPoint( p1 )
#		painter.drawPoint( p2 )
#		painter.drawPoint( p3 )
#		painter.drawPoint( p4 )
#		
#		painter.setPen( QPen( Qt.red, 5 ) )
#		painter.drawPoint( QVector3D( boundedD1.p1().x(), boundedD1.p1().y(), -50 ).toPointF() )
#		painter.drawPoint( QVector3D( boundedD1.p2().x(), boundedD1.p2().y(), 50 ).toPointF() )
#		painter.drawPoint( QVector3D( boundedD2.p1().x(), boundedD2.p1().y(), -50 ).toPointF() )
#		painter.drawPoint( QVector3D( boundedD2.p2().x(), boundedD2.p2().y(), -50 ).toPointF() )
##
#		
#		pts = self.rectToCube(rect)
#		l = []
#		for pt in pts:
#			l.append( rotate( pt ) )
#
#		( pTL_F, pTR_F, pBL_F, pBR_F, pTL_B, pTR_B, pBL_B, pBR_B ) = l
#		
#		painter.setBrush( Qt.blue )
#		polygon = QPolygonF()
#		polygon.append( pTL_B.toPointF() )
#		polygon.append( pTR_B.toPointF() )
#		polygon.append( pBR_B.toPointF() )
#		polygon.append( pBL_B.toPointF() )
#		painter.drawPolygon( polygon )
#		
#		painter.setBrush( Qt.red )
#		polygon = QPolygonF()
#		polygon.append( pTL_F.toPointF() )
#		polygon.append( pTR_F.toPointF() )
#		polygon.append( pBR_F.toPointF() )
#		polygon.append( pBL_F.toPointF() )
#		painter.drawPolygon( polygon )
#		
#		painter.setBrush( Qt.NoBrush )
#		painter.drawRect( self._chartRect )
		
#		painter.setPen( Qt.NoPen )
#		p1 = QVector3D( self._chartRect.topLeft().x(), self._chartRect.topLeft().y(), 50 )
#		p2 = QVector3D( self._chartRect.topRight().x(), self._chartRect.topRight().y(), 50 )
#		
#		p3 = QVector3D( self._chartRect.bottomLeft().x(), self._chartRect.bottomLeft().y(), 50 )
#		p4 = QVector3D( self._chartRect.bottomRight().x(), self._chartRect.bottomRight().y(), 50 )
#		
#		p5 = QVector3D( self._chartRect.topLeft().x(), self._chartRect.topLeft().y(), -50 )
#		p6 = QVector3D( self._chartRect.topRight().x(), self._chartRect.topRight().y(), -50 )
#		
#		p7 = QVector3D( self._chartRect.bottomLeft().x(), self._chartRect.bottomLeft().y(), -50 )
#		p8 = QVector3D( self._chartRect.bottomRight().x(), self._chartRect.bottomRight().y(), -50 )
#		
#		center = QVector3D( self._chartRect.center().x(), self._chartRect.center().y(), 0 )
#		
#		m = QMatrix4x4()
#		m.rotate(45, 1.0, 0, 0);
#		m.rotate(20, 0, 0, -1.0);
#		m.rotate(20, 0, 1.0, 0);
#		
#		p1 -= center
#		p1 *= m
#		p1 += center
#		p2 -= center
#		p2 *= m
#		p2 += center
#		p3 -= center
#		p3 *= m
#		p3 += center
#		p4 -= center
#		p4 *= m
#		p4 += center
#		
#		p5 -= center
#		p5 *= m
#		p5 += center
#		p6 -= center
#		p6 *= m
#		p6 += center
#		p7 -= center
#		p7 *= m
#		p7 += center
#		p8 -= center
#		p8 *= m
#		p8 += center
#				
#		painter.setBrush( Qt.blue )
#		polygon = QPolygonF()
#		polygon.append( p5.toPointF() )
#		polygon.append( p6.toPointF() )
#		polygon.append( p8.toPointF() )
#		polygon.append( p7.toPointF() )
#		painter.drawPolygon( polygon )
#		
#		painter.setBrush( Qt.red )
#		polygon = QPolygonF()
#		polygon.append( p1.toPointF() )
#		polygon.append( p2.toPointF() )
#		polygon.append( p4.toPointF() )
#		polygon.append( p3.toPointF() )
#		painter.drawPolygon( polygon )
#		
#		
#		m2 = QMatrix4x4()
#		m2.rotate(-20, 0, 1.0, 0);
#		m2.rotate(-20, 0, 0, -1.0);
#		m2.rotate(-45, 1.0, 0, 0);
#		
#		p1 -= center
#		p1 *= m2
#		p1 += center
#		p2 -= center
#		p2 *= m2
#		p2 += center
#		p3 -= center
#		p3 *= m2
#		p3 += center
#		p4 -= center
#		p4 *= m2
#		p4 += center
#		
#		p5 -= center
#		p5 *= m2
#		p5 += center
#		p6 -= center
#		p6 *= m2
#		p6 += center
#		p7 -= center
#		p7 *= m2
#		p7 += center
#		p8 -= center
#		p8 *= m2
#		p8 += center
#		
#		
#		
#		painter.setBrush( Qt.green )
#		polygon = QPolygonF()
#		polygon.append( p5.toPointF() )
#		polygon.append( p6.toPointF() )
#		polygon.append( p8.toPointF() )
#		polygon.append( p7.toPointF() )
#		painter.drawPolygon( polygon )
#		
#		painter.setBrush( Qt.yellow )
#		polygon = QPolygonF()
#		polygon.append( p1.toPointF() )
#		polygon.append( p2.toPointF() )
#		polygon.append( p4.toPointF() )
#		polygon.append( p3.toPointF() )
#		painter.drawPolygon( polygon )
#		
#		return None
#		painter.save()
#		style = self.columnStyle( column )
#		backBrush = QBrush(style.brush())
#		frontBrush = QBrush( style.brush() )
#		backBrush.setColor( backBrush.color().darker(120) )
#		
#		painter.setPen( Qt.NoPen )
#		
#		( frontPoint, backPoint ) = self._calculatePoint( column )
#		
#		for  i in range( len(frontPoint) - 1):
#			p1 = frontPoint[i]
#			p2 = frontPoint[i + 1]
#			p3 = backPoint[i]
#			p4 = backPoint[i + 1]
#			
#			polygon = QPolygonF()
#			polygon.append( p1 )
#			polygon.append( p2 )
#			polygon.append( p4 )
#			polygon.append( p3 )
#			
#			if p1.y() > p2.y(): #Up, so bottom
#				painter.setBrush( backBrush )
#				painter.setPen( QPen(backBrush, 2) )
#			else:
#				painter.setBrush( style.brush() )
#				painter.setPen( QPen(style.brush(), 2) )
#				
#			painter.drawPolygon( polygon )
#			
#
##		painter.drawPolygon( polygon )
#		
#		painter.restore()
		
		
			

#	def _paintValues(self, painter, column):
#		t = self.columnType(column)
#
#		delegate = None
#		
#		if t | Type.Point == t:
#			delegate = self._pointDelegate
#		elif t == Type.Bar:
#			delegate = self._barDelegate
#		
#		rows = self.model().rowCount()
#		
#		painter.save()
#		style = self.columnStyle( column )
#		painter.setBrush( style.brush() )
#		painter.setPen( style.pen() )
#
#		for r in range(0, rows):
#			index = self.model().index( r, column )
#			option = QStyleOptionViewItem()
#			value = index.data()
#			if value < 0:
#				option.decorationPosition = QStyleOptionViewItem.Bottom
#			else:
#				option.decorationPosition = QStyleOptionViewItem.Top
#
#			option.rect = self.itemRect( index )
#
#			if t | Type.Line == t:
#				if r < (rows - 1):
#					p1 = option.rect.center()
#					p2 = self.itemRect( self.model().index( r + 1, column ) ).center()
#					painter.drawLine( p1, p2 )
#
#			if delegate != None:
#				delegate.paint( painter, option, index )
#		painter.restore()