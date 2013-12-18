from ..Global import Color
from .Chart import Chart, ChartStyle, Axis
from .Delegates import PointDelegate
from PySide.QtGui import QPainter, QPen, QColor, QFontMetrics, QPainterPath, QBrush, QStyleOptionViewItem, QStyle, QLinearGradient
from PySide.QtCore import QSize, QRect, QPointF, QPoint, Qt, QLineF


class RadarAxis( Axis ):
    def __init__( self ):
        super( RadarAxis, self ).__init__()
        self.line = QLineF()
        self.pen = QPen( QColor(Color.LightGray), 1.5 )

    def paint( self, painter ):
        painter.save()
        painter.setPen( self.pen )
        painter.drawLine( self.line )
        painter.restore()

    def setP1( self, p1 ):
        self.line.setP1( p1 )

    def setP2( self, p2 ):
        self.line.setP2( p2 )

    def valueToPoint( self, value ):
        e = float( self.max - self.min )
        p = abs( float( value - self.min) / e )
        return self.line.pointAt( p )


class RadarChart(Chart):
    ''' RadarChart provides a view for QAbstracItemModel to represent a Kiviat Diagram. '''
    def __init__(self, parent=None):
        super(RadarChart, self).__init__( parent )
        self._origin = QPointF(20, 20)
        self.axis = []
        self._minBottomMargin = 0
        self._pointDelegate = PointDelegate( self )

    def itemRect(self, index ):
        '''Overloaded method.
        '''
        r = QRect()
        value = 0
        try:
            value = float( index.data() )
        except:
            value = 0
        p = self.axis[ index.row() ].valueToPoint( value )
        r = QRect( -5, -5, 10 ,10 ).translated( p.x(), p.y() ) 
        return r.normalized()


    def paintChart(self, painter):
        '''Overloaded method.
        '''
        painter.setRenderHints( QPainter.Antialiasing | QPainter.TextAntialiasing )
        # painter.drawRect( self._valuesRect )
        # painter.drawRect( self._chartRect )
        # painter.drawRect( self._legendRect )
        # painter.drawRect( self._titleRect )
        for axis in self.axis:
            axis.paint( painter )

        for c in range( self.model().columnCount() ):
            self._paintValues( painter, c )
        self._paintLegend(painter)
        font = self.font()
        font.setItalic( True )
        painter.setFont( font )
        painter.drawText( self._titleRect, Qt.AlignHCenter | Qt.AlignTop | Qt.TextWordWrap, self._title )


    def _paintValues( self, painter, column ):
        rows = self.model().rowCount()
        isActive = False
        selectedIndexes = self.selectionModel().selectedIndexes()
        painter.save()
        style = self.columnStyle( column )
        painter.setBrush( style.brush() )
        painter.setPen( style.pen() )
        isActive = True
        if self.selectionModel() != None:
            selectedIndexes = self.selectionModel().selectedIndexes()
            if len( selectedIndexes ) != 0:
                isActive = False
                for idx in selectedIndexes:
                    if idx.column() == column:
                        isActive = True
                        break
        for r in range( rows ):
            index = self.model().index( r, column )
            option = QStyleOptionViewItem()
            try:
                value = float( index.data() )
            except:
                value = 0
            if isActive == False:
                option.state = QStyle.State_Off
            elif index in self.selectionModel().selectedIndexes():
                option.state = QStyle.State_Selected
            option.rect = self.itemRect( index )
            if r < (rows - 1):
                p1 = option.rect.center()
                p2 = self.itemRect( self.model().index( r + 1, column ) ).center()
            else:
                p1 = option.rect.center()
                p2 = self.itemRect( self.model().index( 0, column ) ).center()
            if isActive == False:
                line = QLineF( p1, p2 )
                l = QLineF( line.pointAt( 0.5 ), line.p2() ).normalVector()
                l.setLength( 4 )
                gradient = QLinearGradient( l.p1(), l.p2() )
                c = QColor( Qt.darkGray )
                c.setAlpha( 50 )
                gradient.setColorAt( 0, c )
                gradient.setColorAt( 1, Qt.transparent )
                gradient.setSpread( QLinearGradient.ReflectSpread )
                painter.save()
                pen = QPen( QBrush( gradient ), 8 )
                painter.setPen( pen ) 
                painter.drawLine( p1, p2 )
                painter.restore()
            else:
                painter.drawLine( p1, p2 )
            self._pointDelegate.paint( painter, option, index )
        painter.restore()


    def _updateRects(self):
            if self.model() == None:
                return None
            textWidth = self._scanValues()
            self.defineRects()
            w = min( self._chartRect.width(), self._chartRect.height() )
            self._valuesRect = QRect( -w/2, -w/2, w, w )
            self._valuesRect.translate( self._chartRect.center().x(), self._chartRect.center().y() )
            self._titleRect.moveTo( self._chartRect.bottomLeft() )
            self._titleRect.translate( (self._chartRect.width() - self._titleRect.width())/2, 10 )

            ellipse = QPainterPath()
            ellipse.addEllipse( self._valuesRect )

            rowCount = self.model().rowCount()
            for c in range( rowCount ):
                self.axis[c].setP1( self._valuesRect.center() )
                p = ellipse.pointAtPercent( float(c) / float( rowCount ) )
                self.axis[c].setP2( p )
                    
    def _paintColumnLegend(self, painter, c, pos, metricsH):
        r = QRect( pos.x() + 10, pos.y() - 10, 20, 20 )
        posText = pos + QPoint( 45, metricsH/2 )
        style = self.columnStyle(c)
        s = str(self.model().headerData( c, Qt.Horizontal ))
        painter.drawText( posText, s )
        painter.save()
        
        painter.setPen( style.pen() )
        painter.setBrush( style.brush() )

        p = QPainterPath()
        p.moveTo( 20, 10 )
        p.lineTo( 14, 20 )
        p.lineTo( 0, 10 )
        p.lineTo( 14, 0 )
        p.closeSubpath()
        p.translate( r.topLeft() )
        painter.drawPath( p )

        painter.restore()

    def process( self ):
        '''Defines the metrics and components to display the chart.
         Called when model ha changed.
        '''
        if self.model() == None:
                return None
        self._updateRects()


    def _scanValues(self):
        '''Scans values in the model to find the minimum and the maximum. Returns the width needed to display the Y scale.
        If the values are greater than zero, the minimum is equal to 0. If the values are less than 0, the maximum is equal to 0.
        If a value is not a number (undefined, a string, etc.), she's considered as equal to 0. 
        '''
        rows = self.model().rowCount()
        cols = self.model().columnCount()
        metrics = QFontMetrics( self.font() )
        textWidth = 0
        self.axis = []
        _min = 0
        _max = 0
        for r in range( 0, rows ):
            for c in range( 0, cols ):
                value = self.model().index( r, c ).data()
                try:
                    value = float(value)
                except:
                    value = 0
                _min = float(min( _min, value ))
                _max = float(max( _max, value ))
        if _min == _max:
            _min -= 1
            _max += 1
        for r in range( 0, rows ):
            axis = RadarAxis()
            axis.min = _min
            axis.max = _max
            self.axis.append( axis )
        return textWidth