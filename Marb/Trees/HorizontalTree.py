
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



from .VerticalTree import VerticalTree

from PySide.QtCore import QPointF
class HorizontalTree(VerticalTree):
		_max = 0
		_min = 0
		def __init__(self, parent=None):
				super(HorizontalTree, self).__init__( parent )
				self.setDirectConnection(True)
				

		def _positionsInView(self):
				self._realSize.setWidth( ( self._depth ) * ( self._xDistance + self._rect.width() ) + self._rect.width() )
				self._realSize.setHeight( self._left * ( self._yDistance + self._rect.height() ) + self._rect.height() )

				self.setScrollBarValues()
				
				offset = QPointF( self._rect.width()/2, self._rect.height()/2 )
				for index in self._itemTreePos.keys():
						p = self._itemTreePos[ index ]
						self._itemPos[index] = QPointF( p.y() * ( self._xDistance + self._rect.width() ), p.x() * ( self._yDistance + self._rect.height() ) ) + offset
