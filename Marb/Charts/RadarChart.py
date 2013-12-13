from ..Global import Color
from .Chart import Chart, ChartStyle
from PySide.QtGui import QPainter, QPen, QColor, QFontMetrics, QPainterPath, QBrush
from PySide.QtCore import QSize, QRect, QPointF, QPoint, Qt

class Axis:
	def __init__( self ):
		self.min = 0
		self.max = 0
		self.angle = 0
		self.alpha = 0
		self.beta = 0
		self.angle = 0

class RadarChart(Chart):
	''' RadarChart provides a view for QAbstracItemModel to represent a Kiviat Diagram. '''
	def __init__(self, parent=None):
		super(RadarChart, self).__init__( parent )
		self._origin = QPointF(20, 20)
		self.axis = []