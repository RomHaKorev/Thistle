from ..Global import Color, Shape, Type
from PySide.QtGui import QPen, QBrush, \
QColor, QFontMetrics, QPainter, QPixmap, QAbstractItemView
from PySide.QtCore import QRect, QPoint, Qt, QModelIndex, QSize
from ..MarbAbstractItemView import MarbAbstractItemView

from .ChartStyle import ChartStyle

class Chart(MarbAbstractItemView):
	'''The Chart class provides an abstract base for the chart viewes.
	The Chart class defines the standard interface for every chart views in Marb. It is not supposed to be instantiated directly but should be subclassed.
	'''
	def __init__(self, parent=None):
		'''Constructor. constructs a Chart object with the given 'parent'.
		'''
		super(Chart, self).__init__( parent )
		self.setEditTriggers( QAbstractItemView.NoEditTriggers )
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


	def calculateLegendRect(self):
		'''Calculates the rectangle needed to display the legend according to the title of each column (given by the QHeaderView).
		'''
		if self.model() == None:
			return None
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


	def _calculateOrder( self, value ):
		'''Calculates the order of scale for value (i.e. the power of ten greater than value).
		'''
		order = 1.0
		v = abs( value )
		if v >= 1:
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


	def columnStyle(self, column ):
		'''Returns the style for the column.
		If no style has been set for the column, returns the style by default
		'''
		if column in self._style:
			return self._style[ column ]
		else:
			style = ChartStyle()
			c1 = Color.lightColorAt( column )
			c2 = Color.regularColorAt( column )
			style.setPen( QPen( QColor(c2), 2 ) )
			style.setBrush( QBrush(c1) )
			return style


	def defineRects(self):
		''' Define the bounding rectangles used to draw the chart.
		* chartRect: The chart bounds (data + axis)
		* valueRect: Area where the data representation will be drawn. Should be defined inside chartRect
		* legendRect: Area where the legend will be drawn
		* titleRect: Area where the title will be drawn
		'''
		self._chartRect = QRect( QPoint(self._marginX, self._marginY), self.size() - QSize( self._marginX*2, self._marginY*2 ) )
		self.calculateLegendRect()
		self._chartRect.setHeight( self._chartRect.height() - self._legendRect.height() - 10 )
		self._chartRect.translate( 0, self._legendRect.height() + 10 )
		if self._title != "":
			font = self.font()
			font.setItalic( True )
			m = QFontMetrics( font )
			r = QRect( 0, 0, self._chartRect.width() - 40, 0 )
			self._titleRect = m.boundingRect( r, Qt.AlignHCenter | Qt.AlignTop | Qt.TextWordWrap, self._title )
			self._chartRect.setHeight( self._chartRect.height() - self._titleRect.height() - 20 )


	def indexAt(self, point):
		'''Returns the model index of the item at the viewport coordinates point.
		'''
		if self.model() == None:
			return QModelIndex()
		for row in range( self.model().rowCount() ):
			for col in range( self.model().columnCount() ):
				index = self.model().index( row, col )
				r = self.itemPath(index)
				if r.contains( point ):
					return index
		return QModelIndex()


	def paintChart( self, painter ):
		'''Reimplemented.
		Paints the chart itself on the given painter device. Called by paintEvent and save(). 
		'''
		raise( NotImplementedError, "Must be implemented." )


	def _paintColumnLegend(self, painter, c, pos, metricsH):
		'''Paints the legend for the given column. The kind of legend should be defined by each view (linear and radial represent legend differently).
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


	def paintEvent( self, event ):
		if self.model() == None:
			return None
		painter = QPainter( self.viewport() )
		painter.setClipRect( event.rect() )
		self.paintChart( painter )


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
				p = pos + QPoint( -40, 0 )
				w += sWidth
				pos += QPoint( sWidth, 0 )
			self._paintColumnLegend(painter, c, p, metricsH)
		painter.restore()


	def _paintValues(self, painter, column):
		'''Paints items on the paint device [painter] for the [column]
		'''
		raise( NotImplementedError, "Must be implemented." )

	def process( self ):
		'''Defines the metrics and components to display the chart.
		 Called when model ha changed.
		'''
		if self.model() == None:
			return None
		self._min = 0
		self._max = 0
		self._updateRects()


	def resizeEvent(self, ev ):
		self.updateValues()
		return MarbAbstractItemView.resizeEvent(self, ev )


	def save(self, filename, size = None ):
		''' Save the chart in the given file filename.
		The file should be an PNG image. 
		'''
		if size == None:
			size = self.size()
		pixmap = QPixmap( size )
		pixmap.fill( Qt.white )
		painter = QPainter( pixmap )
		self.process()
		self.paintChart( painter )
		painter.end()
		return pixmap.save( filename )


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


	def _setAlphaBeta(self):
		'''Calculates the coefficients alpha and beta used to convert a value in the model in the corresponding value in pixel in the chart.
		Translates the origin to place the X-Axis.
		'''
		self._alpha = -float(self._valuesRect.height()) / float( self._maxBound - self._minBound )
		self._beta = (self._maxBound * self._valuesRect.height() ) / ( self._maxBound - self._minBound ) + self._valuesRect.y()
		self._origin.setY( self._beta )		


	def setColumnStyle(self, column, style):
		'''Sets the style for the column.
		'''
		self._style[ column ] = style


	def setTitle(self, title ):
		'''Sets the chart title to title
		Updates the chart to consider the modification
		'''
		if isinstance( title, str):
			self._title = title
			self._updateRects()
			self.update()
		else:
			raise( TypeError, "title must be a string" )


	def _updateRects(self):
		'''Calculates the size and the position of each view part (legend, title,chart)
		Called when the view is resized or model has changed.
		'''
		raise( NotImplementedError, "Must be implemented." )


	def updateValues(self):
		self.process()	


	def valueToPx(self, value):
		'''Converts the given value in pixel according to the chart scale.
		The pixel value is given by the equation: y = alpha * value + beta 
		'''
		try:
			value = float(value)
		except:
			value = 0
		return value * self._alpha + self._beta


