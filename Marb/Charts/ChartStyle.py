from ..Global import Color, Shape, Type
from PySide.QtGui import QPen, QColor, QBrush

class ChartStyle:
	'''
	ChartStyle class contains the options for a render.
	
	A ChartStyle is used to custom a serie of data. For example, you can specify the color of the a line and the shape of the points.
	You should not create a ChartStyle but use the Chart::columnStyle( column ).
	'''
	def __init__(self):
		'''Constructor. Constructs an instance of ChartStyle.
		You should not construct your own instance. Use Chart::columnStyle() method, instead.
		'''
		self._brush = QBrush( Color.LightBlue )
		self._pen = QPen( QColor(Color.Blue), 1 )
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