from PySide.QtGui import QColor

def enum(**enums):
	return type( 'Enum', (), enums)
	

class Color:
	'''List of predefined colors
	'''
	Blue       = 0x6090e4
	Green      = 0x9AEC7A
	Purple     = 0x8600C8
	Red        = 0xDB0000
	Orange     = 0xFF8000
	Gray       = 0x868686
		
	LightBlue   = 0x68B9FF
	LightGreen  = 0xc9ff71
	LightPurple = 0x8274B0
	LightRed    = 0xFF5656
	LightOrange = 0xFFB366
	LightGray   = 0xBEBEBE
	
	DarkBlue   = 0x0839A1
	DarkGreen  = 0x8CD01E
	DarkPurple = 0x7300AB
	DarkRed    = 0xBA0000
	DarkOrange = 0xFFF219
	DarkGray   = 0x505050
	
	__regularColors = [ Blue, Green, Orange, Purple, Red, Orange, Gray  ]
	__lightColors = [ LightBlue, LightGreen, LightOrange, LightPurple, LightRed, LightOrange, LightGray  ]
	__darkColors = [ DarkBlue, DarkGreen, DarkOrange, DarkPurple, DarkRed, DarkOrange, DarkGray  ]
	
	@staticmethod
	def regularColorAt(index):
		''' Returns the regular (not light, not dark) color corresponding to the index.
		
		Used to set a default color
		'''
		return QColor(Color().__regularColors[index%len(Color().__regularColors)])
	
	@staticmethod
	def lightColorAt(index):
		''' Returns the light color corresponding to the index.
		
		Used to set a default color.
		'''
		return QColor(Color().__lightColors[index])
	
	@staticmethod
	def darkColorAt(index):
		''' Returns the dark color corresponding to the index.
		
		Used to set a default color.
		'''
		return QColor(Color().__darkColors[index])
	
	@staticmethod
	def colorAt( index ):
		t = Color().__regularColors + Color().__lightColors + Color().__darkColors
		return QColor(t[index % len(t) ])


class Shape:
	'''List of available shapes for the points
	'''
	Rectangle = 0
	Ellipse   = 1
	RoundedRect = 2
	Diamond = 3
	Triangle = 4
	ReversedTriangle = 5


class Type:
	'''List of available types for the linear chart
	'''
	Point = 1
	Line  = 2
	Bar   = 4