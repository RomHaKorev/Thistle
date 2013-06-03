from ..Global import Color, Shape, Type

from PySide.QtGui import QAbstractItemView, QPen, QBrush, \
QRegion, QColor, QFontMetrics, QPainter, QPixmap
from PySide.QtCore import QRect, QPoint, Qt, QModelIndex, QSize

class ChartStyle:
	'''
	ChartStyle class contains the options for a render.
	
	A ChartStyle is used to custom a serie of data. For example, you can specify the color of the a line and the shape of the points.
	You should not create a ChartStyle but use the Chart::columnStyle( column ).
	'''
	def __init__(self):
		self._brush = QBrush( Color.LightBlue )
		self._pen = QPen( QColor(Color.Blue), 2 )
		self._shape = Shape.Ellipse
		self._type = Type.Line | Type.Point


	def setBrush(self, brush):
		'''brush: instance of QBrush
		Sets the brush used to fill some part of the render (the point shape for a line chart or the bar in a bar chart, for example).
		'''
		assert isinstance(brush, QBrush)
		self._brush = brush

	
	def setPen(self, pen):
		'''pen: instance of QPen
		Sets the pen used to draw some part of the render (the borders of the bar in a bar chart, for example).
		'''
		assert isinstance(pen, QPen)
		self._pen = pen
		

	def setShape(self, shape):
		'''shape: Instance of Global.Shape
		Sets the shape used to represent the points in a line chart.
		'''
		self._shape = shape

	
	def setType(self, chartType):
		'''chartType: Instance of Global.Type
		Sets the render for the serie of data (line, points or bar).
		'''
		self._type = chartType

		
	def brush(self):
		'''returns the brush used to fill some part of the render.
		'''
		return self._brush

	
	def pen(self):
		'''Returns the pen used to draw some part of the render (the borders of the bar in a bar chart, for example).
		'''
		return self._pen

	
	def shape(self):
		'''Returns the shape used to represent the points in a Point/line chart.
		'''
		return self._shape

	
	def type(self):
		'''Sets the render type for the serie of data (line, points or bar).
		'''
		return self._type



class Chart(QAbstractItemView):
	'''The Chart class provides an abstract base for the chart viewes.
	
	The Chart class defines the standard interface for every chart views in Marb. It is not supposed to be instantiated directly but should be subclassed.  
	'''
	def __init__(self, parent=None):
		super(Chart, self).__init__( parent )
		self._min = 0
		self._max = 0
		self._minBound = 0
		self._maxBound = 0
		self._order = 1
		self._nbTicks = 10
		self._tickSize = 0
		self._alpha = 1
		self._beta = 1
		self._chartRect = QRect()
		self._valuesRect = QRect()
		self._legendRect = QRect()
		self._title = ""
		self._titleRect = QRect()
		self._origin = QPoint(0,0)
		self._style = {}
		self._nbDigits = 1
		
		self._marginX = 20
		self._marginY = 20
		
		self.resize( QSize(500, 400) )		
	
	def setColumnStyle(self, column, style):
		'''Sets the style for the column.
		'''
		self._style[ column ] = style


	def columnStyle(self, column ):
		'''Returns the style for the column.
		
		If no style has been set for the column, returns the style by default
		'''
		if column in self._style:
			return self._style[ column ]
		else:
			style =  ChartStyle()
			c1 = Color.lightColorAt( column )
			c2 = Color.regularColorAt( column )
			style.setPen( QPen( QColor(c2), 2 ) )
			style.setBrush( QBrush(c1) )
			return style

		
	def setTitle(self, title ):
		'''Sets the chart title to title
		
		Updates the chart to consider the modification
		'''
		if isinstance( title, str):
			self._title = title
			self._processSpec()
			self.update()
		else:
			raise( TypeError, "title must be a string" )
	
	def process( self ):
		'''Defines the metrics and components to display the chart.
		 Called when model ha changed.
		'''
		if self.model() == None:
			return None
		self._min = 0
		self._max = 0
								
		self._processSpec()

	def _processSpec(self):
		'''Calculates the size and the position of each view part (legend, title,chart)
		Called when the view is resized or model has changed.
		'''
		raise( NotImplementedError, "Must be implemented." )
	
	def _scanValues(self):
		'''Scans values in the model to find the minimum and the maximum. Returns the width needed to display the Y scale.
		
		If the values are greater than zero, the minimum is equal to 0. If the values are less than 0, the maximum is equal to 0.
		If a value is not a number (undefined, a string, etc.), she's considered as equal to 0. 
		'''
		rows = self.model().rowCount()
		cols = self.model().columnCount()
		metrics = QFontMetrics( self.font() )
		textWidth = 0
		for r in range( 0, rows ):
			s = str(self.model().headerData( r, Qt.Vertical ))
			textWidth = max( textWidth, metrics.width( s ) + 5 )
			for c in range( 0, cols ):
				value = self.model().index( r, c ).data()
				try:
					float(value)
				except:
					value = 0
				
				self._min = float(min( self._min, value ))
				self._max = float(max( self._max, value ))
		return textWidth


	def _calculateBounds(self):
		'''Calculates the minimum bounds and the maximum bounds (i.e., the minimum and the maximum displayed on the chart).
		Calculates the order and the tick size(delta between two ticks on the Y axis) of the charts values.
		
		If the minimum is equal to the maximum, the minimum bound is equal to minimum - 1 and the maximum bound to maximum + 1  
		'''
		self._minBound = self._min
		self._maxBound = self._max
		if abs(self._maxBound - self._minBound) == 0:
			self._maxBound += 1
			self._minBound -= 1
		
		self._order = self._calculateOrder( self._max - self._min )
		self._tickSize = (self._max - self._min ) / (self._nbTicks - 1)
		
		if self._order >= 10:
			self._nbDigits = 0
		elif self._order == 1:
			self._nbDigits = 2
		else:
			nbZero = str(self._order).count( '0' )
			self._nbDigits = nbZero + 2
			
		
	def _calculateOrder( self, value ):
		'''Calculates the order of scale for value (i.e. the power of ten greater than value).
		'''
		order = 1.0
		v = abs( value )
		if  v >= 1:
			while v > 1:
				order *= 10.0
				v /= 10.0
			order /= 10.0
		
		elif v != 0:
			while v < 1:
				order /= 10.0
				v *= 10.0
			order *= 10.0

		return int(order)
	

	def calculateLegendRect(self):
		if self.model() == None:
			return None
		'''Calculates the rectangle needed to display the legend according to the title of each column (given by the QHeaderView).
		'''
		metrics = QFontMetrics( self.font() )
		h = metrics.height() + 5
		cols = self.model().columnCount()
		nbLines = 1
		w = 40
		maxWidth = self._chartRect.width()
		for c in range( 0, cols ):
			s = str(self.model().headerData( c, Qt.Horizontal ))
			sWidth = metrics.width( s ) + 40
			if ( w + sWidth ) > maxWidth:
				nbLines += 1
				w = sWidth
			else:
				w += sWidth
				
		self._legendRect = QRect( self._marginX, self._marginY, maxWidth, nbLines * h )

	
	def valueToPx(self, value):
		'''Converts the given value in pixel according to the chart scale.
		
		The pixel value is given by the equation: y = alpha * value + beta 
		'''
		try:
			float(value)
		except:
			value = 0
		return value * self._alpha + self._beta
		
		
	def _setAlphaBeta(self):
		'''Calculates the coefficients alpha and beta used to convert a value in the model in the corresponding value in pixel in the chart.
		
		Translates the origin to place the X-Axis.
		'''
		self._alpha = -float(self._valuesRect.height()) / float( self._maxBound - self._minBound )
		self._beta = (self._maxBound * self._valuesRect.height() ) / ( self._maxBound - self._minBound ) + self._valuesRect.y()

		self._origin.setY( self._beta )
	
	
	def itemRect(self, index):
		'''Returns the QRect in which the index value is displayed on the view.
		'''
		return QRect()
	
		
	def resizeEvent(self, ev ):
		'''Overloaded method.
		'''
		QAbstractItemView.resizeEvent(self, ev )
		self._processSpec()
	
	
	def moveCursor(self, cursorAction, modifiers):
		'''Overloaded method.
		'''
		return QModelIndex()
	
	
	def horizontalOffset(self):
		return self.horizontalScrollBar().value()
	
	
	def verticalOffset(self):
		return self.verticalScrollBar().value()


	def indexAt(self, point):
		'''Overloaded method.
		'''
		return QModelIndex() # BUG IN PySide
		p = point - QPoint( self.horizontalOffset(), self.verticalOffset() )
		for index in self._itemPos.keys():
			r = self.itemRect(index)
			if r.contains( p ):
				None
		return index

	
	def visualRect(self, index):
		'''Overloaded method.
		'''
		r = self.itemRect(index)
		return r
	
	
	def visualRegionForSelection(self, selection):
		'''Overloaded method.
		'''
		ranges = selection.count()
		region = QRegion()
		for i in range(0, ranges):
			ran = self.selection.at(i)
			for row in range( ran.top(), ran.bottom() ):
				index = self.model().index( row, 0, self.rootIndex() )
				region += self.visualRect( index )
	

	def rows(self, index):
		'''Overloaded method.
		'''
		return self.model().rowCount( self.model().parent(index))
	
	
	def setModel(self, *args, **kwargs):
		'''Overloaded method.
		'''
		QAbstractItemView.setModel(self, *args, **kwargs)
		self.process()
	
	
	def dataChanged(self, top_left, bottomRight ):
		'''Overloaded method.
		'''
		QAbstractItemView.dataChanged(self, top_left, bottomRight)
		self.process()
		self.update( self.model().index(0,0) )
	

	def rowsAboutToBeRemoved(self, parent, start, end):
		'''Overloaded method.
		'''
		QAbstractItemView.rowsAboutToBeRemoved(self, parent, start, end)
		self.process()
		self.viewport().update()
		self.setScrollBarValues()
	
	
	def rowsInserted(self, parent, start, end):
		'''Overloaded method.
		'''
		QAbstractItemView.rowsInserted(self, parent, start, end)
		self.process()
		self.viewport().update()
		self.setScrollBarValues()
		
	
	def _paintLegend(self, painter):
		'''Paint the legend in the QRect self._legendRect
		
		The legend corresponds to the text in the Horizontal QHeaderView and the style defined for each column. 		
		'''
		painter.save()
		metrics = QFontMetrics( self.font() )
		metricsH = metrics.height()
		h = metricsH + 5
		cols = self.model().columnCount()
		w = 0
		maxWidth = self._legendRect.width()

		legendPos = QPoint(self._legendRect.topLeft()) + QPoint(0, metricsH)
		pos = legendPos + QPoint( 50, 0)
		for c in range( 0, cols ):
			s = str(self.model().headerData( c, Qt.Horizontal ))
			sWidth = metrics.width( s ) + 50
			if ( w + sWidth ) > maxWidth: # Paint on a new line
				y = pos.y()
				p = QPoint( self._legendRect.x() + 10, y + h )
				pos = QPoint( self._legendRect.x() + 50, y + h )
				w = sWidth
				pos += QPoint( sWidth, 0 )
			else:
				p = pos + QPoint( -40,  0 )
				w += sWidth
				pos += QPoint( sWidth, 0 )
			self._paintColumnLegend(painter, c, p, metricsH)
		painter.restore()
	
	
	def _paintColumnLegend(self, painter, c, pos, metricsH):
		'''Paint the legend for the given column. The kind of legend should be defined by each view (linear and radial represent legend differently).
		The column legend is represented by a square colored with the pen and brush style and the column name. 
		'''
		r = QRect( pos.x() + 20, pos.y() - 10, 20, 20 )
		posText = pos + QPoint( 45, metricsH/2 )
		
		style = self.columnStyle(c)
		
		s = str(self.model().headerData( c, Qt.Horizontal ))
		painter.drawText( posText, s )
		painter.save()
		painter.setPen( style.pen() )
		painter.setBrush( style.brush() )
		painter.drawRect (r )
		painter.restore()


	def paintEvent( self, event ) :
		if self.model() == None:
			return None
		
		painter = QPainter( self.viewport() )
		
		painter.setClipRect( event.rect() )
		
		self.paintChart( painter )

	def paintChart( self, painter ):
		'''Overloaded method.
		'''
		raise( NotImplementedError, "Must be implemented." )
	
	
	def save(self, filename, size = None ):
		if size == None:
			size = self.size()
		pixmap = QPixmap( size )
		pixmap.fill( Qt.white )
		painter = QPainter( pixmap )
		self.process()
		self.paintChart( painter )
		painter.end()
		return pixmap.save( filename )
	
