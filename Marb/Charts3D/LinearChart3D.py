from ..Global import Color, Render, Type
from ..Charts.Chart import ChartStyle
from ..Charts.Delegates import PointDelegate, BarDelegate

from PySide.QtGui import QColor, QPainter, QPainterPath, QPolygonF, QStyleOptionViewItem, QTransform, QMatrix4x4, QVector3D, QPen, QBrush, QRadialGradient, QLinearGradient
from PySide.QtCore import QRectF, QRect, QPoint, QPointF, Qt, QLineF

from ..Charts.LinearChart import LinearChart

import math

class LinearChart3D( LinearChart ):
	def __init__( self, parent = None ):
		super(LinearChart3D, self).__init__( parent )
		self._depth = 200
		self.angleX = 45
		self.angleY = 10
		self.angleZ = 10 
		self.scale = 1


	def itemRect(self, index ):
		'''Overloaded method.
		'''
		r = QRect()
		t = self.columnType( index.column() )
		value = index.data()
		if value == None:
			return QRect()
		y = self.valueToPx(value)
		x = self._origin.x() + index.row() * self._x
		space = 0
		orderedColumns = self._calculateOrderedColumn()
		if t == Type.Bar:
			w = float(self._x)
			tl = QPoint( x + space/2, y )             # top left
			br = QPoint( x + w, self._origin.y() )    # bottom right
			r = QRect( tl, br )
			if value < 0:
				r.translate( 0, 1 )
			else:
				r.translate( 0, -2 )
		else:
			r = QRect( -5, -5, 10 ,10 ).translated( x + self._x/2, y ) 
		return r.normalized()


	def getScale(self):
		w = self._chartRect.width()
		h = self._chartRect.height()
		mW = self._chartRect.width() + self._depth
		mH = self._chartRect.height() + self._depth
		return min( h/mH, w/mW )

	def _paintXAxis(self, painter, front):
		( pTL_F, pTR_F, pBL_F, pBR_F, pTL_B, pTR_B, pBL_B, pBR_B ) = self.rectToCubePoints(self._chartRect, self._depth, 0)
		r = QRectF( self._origin.x(), self._origin.y(), self._chartRect.width(), 10 )
		face = self.rectToCubeFaces( r, self._depth, 0, True )[3]
		if front == False:
			painter.setPen( QPen( Qt.gray, 0 ) )
			painter.drawPolygon( face )
			
			painter.drawLine( face[2], face[3] )
		else:
			painter.drawLine( face[0], face[1] )
			painter.drawLine( face[1], face[2] )

	def _paintYAxis(self, painter):
		( pTL_F, pTR_F, pBL_F, pBR_F, pTL_B, pTR_B, pBL_B, pBR_B ) = self.rectToCubePoints(self._chartRect, self._depth, depthOffset=0, flatted=True )
		
		r = QRectF( self._chartRect )
		r.setX( self._origin.x() )
		face = self.rectToCubeFaces( r, self._depth, 0, True )[4]
		painter.drawPolygon( face )
		
# 	def _paintYAxis(self, painter):	
# 		p1 = QPoint( self._origin.x(), self._chartRect.y() )
# 		p2 = QPoint( self._origin.x(), self._chartRect.bottom() )
# 		painter.drawLine( p1, p2 )
# 		painter.save()
# 		c = painter.pen().color()
# 		c.setAlpha( 150 )
# 		painter.setPen( QPen( c , 1 ) )
# 		y = self._minBound
# 		while y <= self._maxBound:
# 			p1 = QPoint( self._origin.x(), self.valueToPx(y) )
# 			p2 = p1 + QPoint( self._valuesRect.width(), 0 )
# 			painter.drawLine( p1, p2 )
# 			y += self._tickSize
# 		painter.restore()


	def paintChart(self, painter):
		'''Overloaded method.
		'''
		self.scale = self.getScale()
		painter.save()
		
		self._paintYAxis( painter )
		self._paintXAxis(painter, False)
		
		painter.setRenderHints( QPainter.Antialiasing | QPainter.TextAntialiasing )

		for c in range(self.model().columnCount()-1, -1, -1):
			self._paintValues( painter, c )

		self._paintXAxis(painter, True)
		painter.restore()


	def _paintValues(self, painter, column ):
		depth = self._depth/(self.model().columnCount() + 1)
		depthOffset = -(depth * (column - self.model().columnCount()/2.0 +0.5 ))
		
		painter.save()
		
		style = self.columnStyle( column )
		t = style.type()
		painter.setPen( style.pen() )
		painter.setBrush( style.brush() )
		for r in range(0, self.model().rowCount() ):
			idx = self.model().index( r, column )
			if t | Type.Line == t:
				self._paintLine(painter, idx, depthOffset)
			if t | Type.Point == t:
				self._paintPoint( painter, idx, depthOffset )
			elif t | Type.Bar == t:
				self._paintBar(painter, idx, depth, depthOffset)
		
		painter.restore()
		painter.drawRect( self._chartRect )


	def _paintBar(self, painter, idx, depth, depthOffset):
		c = painter.brush().color()
		c.setAlpha( c.alpha() * 0.75 )
		painter.setBrush( c )
		rect = self.itemRect( idx )
		faces = self.rectToCubeFaces( rect, depth, depthOffset, True )
		for i in (5,2,4,6,3,1):
			f = faces[i-1]
			painter.drawPolygon( f )


	def _paintPoint(self, painter, idx, depth):
		c = painter.brush().color()
		rect = self.itemRect( idx )
		pt = self.transform2DPoint( rect.center(), depth )
		painter.save()
		painter.translate( pt.x(), pt.y() )
		gradient = QRadialGradient(-3, -3, 10)
		gradient.setCenter(3, 3)
		gradient.setFocalPoint(3, 3)
		gradient.setColorAt(1, c.lighter(120))
		gradient.setColorAt(0, c)
		painter.setBrush(gradient)
		painter.setPen(QPen(painter.pen().color(), 0))
		painter.drawEllipse(-10, -10, 20, 20)
		painter.restore()


	def _paintLine( self, painter, idx, depth):
		if not (idx.row() < self.model().rowCount() - 1):
			return None
		idx2 = self.model().index( idx.row() + 1, idx.column() )
		painter.save()
		painter.setPen( Qt.NoPen )
		pt1 = self.itemRect( idx ).center()
		pt2 = self.itemRect( idx2 ).center()

		pA = pt1 + QPoint( 0, 10 )
		pB = pt1 - QPoint( 0, 10 )
		pC = pt2 + QPoint( 0, 10 )
		pD = pt2 - QPoint( 0, 10 )

		p1 = self.transform2DPoint(pA, depth - 5)
		p2 = self.transform2DPoint(pB, depth - 5)
		p3 = self.transform2DPoint(pC, depth - 5)
		p4 = self.transform2DPoint(pD, depth - 5)
		p5 = self.transform2DPoint(pA, depth + 5)
		p6 = self.transform2DPoint(pB, depth + 5)
		p7 = self.transform2DPoint(pC, depth + 5)
		p8 = self.transform2DPoint(pD, depth + 5)
		
		l = QLineF( p1, p3 )
		l1 = QLineF( l.pointAt( 0.5 ), l.p1() ).normalVector()
		l1.setLength( 10 )
		
		c = painter.brush().color()
		gradient = QLinearGradient( l1.p1(), l1.p2() )
		gradient.setColorAt(1, c.lighter(140))
		gradient.setColorAt(0, c)
		painter.setBrush( gradient )
		#painter.setPen(QPen(painter.pen().color(), 0))

		#Faces : 5,2,4,6,3,1
		for pts in [ [p1, p2, p6, p5], [p5, p7, p8, p6], [p1, p5, p7, p3], [p7, p8, p4, p3], [p2, p6, p8, p4 ], [p1, p3, p4, p2] ]:
			polygon = QPolygonF()
			for pt in pts:
				polygon.append( pt )
			painter.drawPolygon( polygon )
		painter.restore()

# 	def _paintLine(self, painter, idx, depth):
# 		c = painter.brush().color()
# 		rect = self.itemRect( idx )
# 		pt = self.transform2DPoint( rect.center(), depth )
# 		painter.save()
# 		if idx.row() < self.model().rowCount() - 1:
# 			idx2 = self.model().index( idx.row() + 1, idx.column() )
# 			p1 = self.itemRect(idx).center()
# 			p1 = self.transform2DPoint(p1, depth)
# 			p2 = self.itemRect(idx2).center()
# 			p2 = self.transform2DPoint(p2, depth)
# 			p3 = p1 + QPoint( 0, 5 )
# 			p4 = p2 + QPoint( 0, 5 )
# 			p1 -= QPoint( 0, 5)
# 			p2 -= QPoint( 0, 5)
# 			polygon = QPolygonF()
# 			polygon.append( p1 )
# 			polygon.append( p2 )
# 			polygon.append( p4 )
# 			polygon.append( p3 )
# 			
# 			l = QLineF( p1, p2 )
# 			l.setLength( l.length() * 0.5 )
# 			l1 = QLineF( l.p2(), l.p1() ).normalVector()
# 			l1.setLength( 10 )
# 			
# 			gradient = QLinearGradient( l1.p1(), l1.p2() )
# 			gradient.setColorAt(1, c.lighter(140))
# 			gradient.setColorAt(0, c)
# 			painter.setBrush( gradient )
# 			painter.setPen(QPen(painter.pen().color(), 0))
# 			painter.drawPolygon( polygon )
# 		painter.restore()

	def rectToCubeFaces(self, rect, depth, depthOffset, rotate ):
		( tl_f, tr_f, bl_f, br_f, tl_b, tr_b, bl_b, br_b ) = self.rectToCubePoints( rect, depth * 0.8, depthOffset, rotate )
		f1 = QPolygonF()
		f2 = QPolygonF()
		f3 = QPolygonF()
		f4 = QPolygonF()
		f5 = QPolygonF()
		f6 = QPolygonF()
		for p in ( tl_f, tr_f, br_f, bl_f ):
			f1.append( p.toPointF() )
		for p in ( tl_b, tr_b, br_b, bl_b ):
			f2.append( p.toPointF() )
		for p in ( tl_f, tr_f, tr_b, tl_b ):
			f3.append( p.toPointF() )
		for p in ( bl_f, br_f, br_b, bl_b ):
			f4.append( p.toPointF() )
		for p in ( tl_f, tl_b, bl_b, bl_f ):
			f5.append( p.toPointF() )
		for p in ( tr_f, tr_b, br_b, br_f ):
			f6.append( p.toPointF() )

		return ( f1, f2, f3, f4, f5, f6 )


	def rectToCubePoints(self, rect, depth, depthOffset, rotate = True, flatted=False):
		p1 = rect.topLeft()
		p2 = rect.topRight()
		p3 = rect.bottomLeft()
		p4 = rect.bottomRight()
		
		pTL_F = QVector3D( p1.x(), p1.y(), depthOffset + depth/2.0 )
		pTR_F = QVector3D( p2.x(), p2.y(), depthOffset + depth/2.0 )
		pBL_F = QVector3D( p3.x(), p3.y(), depthOffset + depth/2.0 )
		pBR_F = QVector3D( p4.x(), p4.y(), depthOffset + depth/2.0 )
		
		pTL_B = QVector3D( p1.x(), p1.y(), depthOffset - depth/2.0 )
		pTR_B = QVector3D( p2.x(), p2.y(), depthOffset - depth/2.0 )
		pBL_B = QVector3D( p3.x(), p3.y(), depthOffset - depth/2.0 )
		pBR_B = QVector3D( p4.x(), p4.y(), depthOffset - depth/2.0 )
		if rotate == True:
			l = []
			for pt in ( pTL_F, pTR_F, pBL_F, pBR_F, pTL_B, pTR_B, pBL_B, pBR_B ):   		
				l.append( self.rotate3DPoint(QVector3D(pt), False, self.scale) )
			( pTL_F, pTR_F, pBL_F, pBR_F, pTL_B, pTR_B, pBL_B, pBR_B ) = l
		if flatted == False:
			return ( pTL_F, pTR_F, pBL_F, pBR_F, pTL_B, pTR_B, pBL_B, pBR_B )
		else:
			l = []
			for pt in ( pTL_F, pTR_F, pBL_F, pBR_F, pTL_B, pTR_B, pBL_B, pBR_B ):
				l.append( pt.toPointF() )
			return l


	def rotate3DPoint(self, p, rev = False, scale = None ):
		center = QVector3D( self._chartRect.center().x(), self._chartRect.center().y(), 0 )
		m = QMatrix4x4()
		if scale != None:
			m.scale( scale, scale, scale )						
		if rev == True:
			m.rotate(-self.angleX, 1, 0, 0)
			m.rotate(-self.angleY, 0, 1, 0)
			m.rotate(-self.angleZ, 0, 0, -1)
		else:
			m.rotate(self.angleX, 1.0, 0, 0)
			m.rotate(self.angleY, 0, 1.0, 0)
			m.rotate(self.angleZ, 0, 0, -1.0)
		p -= center
		p *= m
		p += center
		return p


	def transform2DPoint(self, point, depth):
		pt =  QVector3D( point.x(), point.y(), depth )
		return self.rotate3DPoint(pt, False, self.scale).toPointF()


	def columnType( self, column ):
		return Type.Bar


	def rotate(self, angle, axis):
		if axis == Qt.XAxis:
			self.angleX = angle
		elif axis == Qt.YAxis:
			 self.angleY = angle
		elif axis == Qt.YAxis:
			 self.angleZ = angle
		else:
			raise( TypeError, "axis must be a Qt.Axis value" )


	def setAngles(self, x, y ,z):
		self.angleX = x
		self.angleY = y
		self.angleZ = z
		self.viewport().update()


	def setDepth(self, depth):
		self._depth = depth

