from PySide.QtGui import QColor, QStyledItemDelegate
from PySide.QtCore import Qt

class TimelineConnection():
	def __init__(self):
		self._pen = QPen()
		self._brsuh = QBrush()
		
		
	def setPen(self, pen):
		if not isinstance( pen, QPen ):
			raise "pen must be a "
		self._pen = pen
		
	def paint(self, anchorRect, anchorPoint):
		raise NotImplementedError