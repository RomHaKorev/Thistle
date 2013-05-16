#ifndef LINEARCHART_H
#define LINEARCHART_H

#include "chart.h"
#include "chartstyle.h"

#include <QItemDelegate>

class PointDelegate;
class BarDelegate;

class LinearChart : public Chart {
  Q_OBJECT
public:
  explicit LinearChart(QWidget *parent = 0);
protected:
  PointDelegate* myPointDelegate;
  BarDelegate* myBarDelegate;
  bool myVerticalXAxis;

  QList<int> calculateColumnsOrder();
  virtual void processSpec();
  Marb::Types columnType( int column ) const;
  QList<int> barStyleColumns() const;
  QRectF itemRect( const QModelIndex& index ) const;
  void paintAxis( QPainter& painter );
  void paintXAxis( QPainter& painter );
  void paintYAxis( QPainter& painter );
  void paintTextAxis( QPainter& painter );
  void paintValues( QPainter& painter, int column);
  virtual void paintColumnLegend(QPainter &painter, int column, QPoint pos, int maxHeight);
  virtual void paintChart( QPainter& painter );

signals:
  
public slots:
  
};

/*from Marb.Global import Color, Shape, Type
from Marb.Chart import Chart

from PySide.QtGui import QPainter, QPen, QBrush, QStyleOptionViewItem, QColor, QFontMetrics
from PySide.QtCore import QSize, QRect, QPointF, QPoint, Qt

from Delegates import PointDelegate, BarDelegate

class LinearChartStyle:
  def __init__(self):
    self._brush = QBrush( Color.LightBlue )
    self._pen = QPen( QColor(Color.Blue), 2 )
    self._shape = Shape.Ellipse
    self._type = Type.Line | Type.Point


  def setBrush(self, brush):
    assert isinstance(brush, QBrush)
    self._brush = brush


  def setPen(self, pen):
    assert isinstance(pen, QPen)
    self._pen = pen


  def setShape(self, shape):
    self._shape = shape


  def setPointType(self, t):
    self._type = t


  def brush(self):
    return self._brush


  def pen(self):
    return self._pen


  def shape(self):
    return self._shape


  def pointType(self):
    return self._type



class LinearChart(Chart):
  def __init__(self, parent=None):
    super(LinearChart, self).__init__( parent )
    self._origin = QPointF(20, 10)
    self._x = 0
    self._pointDelegate = PointDelegate( self )
    self._barDelegate = BarDelegate( self )
    self._style = {}
    self._minBottomMargin = 0


  def _processSpec(self):

    self._chartRect = QRect( QPoint(10, 20),  self.viewport().size() - QSize( 20, 40 ) )
    metrics = QFontMetrics( self.font() )
    if self._legend != "":
      r = QRect( 0, 0, self._chartRect.width() - 40, 0 )
      self._legendRect = metrics.boundingRect( r, Qt.AlignHCenter | Qt.AlignTop | Qt.TextWordWrap, self._legend )

      self._chartRect.setHeight( self._chartRect.height() - self._legendRect.height() )
      self._legendRect.translate( self._origin.x(), self._chartRect.height() + 30 )


    self._calculateBounds()
    self._origin.setX( metrics.width( str(-1 * self._order) ) )

    self._valuesRect = QRect( self._chartRect )

    self._valuesRect.setX( self._origin.x() )
    self._x = float( self._valuesRect.width() ) / ( self.model().rowCount() )

    self._setAlphaBeta()
    delta = self._valuesRect.bottom() - ( self._origin.y() + self._minBottomMargin )

    if delta < 0:
      self._valuesRect.setHeight( self._valuesRect.height() + delta )
      self._setAlphaBeta()


  def process( self ):
    if self.model() == None:
      return None
    self._min = 0
    self._max = 0

    self._minBottomMargin = self._scanValues() + 10

    self._processSpec()


  def resizeEvent(self, ev ):
    Chart.resizeEvent(self, ev )
    self._processSpec()


  def columnStyle(self, column ):
    if column in self._style:
      return self._style[ column ]
    else:
      style =  LinearChartStyle()
      c1 = Color.lightColorAt( column )
      c2 = Color.regularColorAt( column )
      style.setPen( QPen( QColor(c2), 2 ) )
      style.setBrush( QBrush(c1) )
      return style


  def setColumnStyle(self, column, style):
    self._style[ column ] = style


  def columnType(self, column):
    if column in self._style:
      return self._style[ column ].pointType()

    return LinearChartStyle().pointType()


  def __barStyleColumns(self):
    bars = []
    for c in range( 0, self.model().columnCount() ):
      if self.columnType( c ) == Type.Bar:
        bars.append( c )
    return bars


  def itemRect(self, index ):
    r = QRect()
    t = self.columnType( index.column() )
    value = index.data()
    if value == None:
      return QRect()
    y = self.valueToPx(value)
    x = self._origin.x() + index.row() *  self._x
    space = self._x * 0.1
    if t == Type.Bar:
      bars = self.__barStyleColumns()
      w = float(self._x) / len(bars)
      x += w * index.column()
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


  def paintEvent(self, event):
    painter = QPainter( self.viewport() )

    painter.setClipRect( event.rect() )
    painter.setRenderHints( QPainter.Antialiasing | QPainter.TextAntialiasing )

    self._paintAxis(painter)
    cols = self.model().columnCount()
    for c in range(0, cols):
      self._paintValues( painter, c )

    self._paintTextAxis(painter)

    painter.drawText( self._legendRect, Qt.AlignHCenter | Qt.AlignTop | Qt.TextWordWrap, self._legend )


  def _paintAxis(self, painter):
    painter.save()
    painter.setPen( QPen( QColor(Color.LightGray), 1.5 ) )

    self.__paintXAxis(painter)
    self.__paintYAxis(painter)

    painter.restore()


  def __paintXAxis(self, painter):
    p1 = QPoint( self._origin.x(), self._origin.y() )
    p2 = p1 + QPoint( self._chartRect.width(), 0 )
    painter.drawLine( p1, p2 )

    x = self._x + self._origin.x()
    i = 0
    while (i < self.model().rowCount() ):
      p1 = QPoint( x, self._origin.y() - 3  )
      p2 = p1 + QPoint( 0, 6 )
      painter.drawLine( p1, p2 )
      i += 1
      x += self._x


  def __paintYAxis(self, painter):
    p1 = QPoint( self._origin.x(), 20 )
    p2 = QPoint( self._origin.x(), self._chartRect.height() + 20 )
    painter.drawLine( p1, p2 )

    painter.save()
    c = painter.pen().color()
    c.setAlpha( 150 )
    painter.setPen( QPen( c , 1 ) )
    y = self._minBound

    while y <= self._maxBound:
      p1 = QPoint( self._origin.x(), self.valueToPx(y)  )
      p2 = p1 + QPoint( self._chartRect.width(), 0 )
      painter.drawLine( p1, p2 )
      y += self._tickSize
    painter.restore()


  def _paintTextAxis(self, painter):
    painter.save()
    metrics = QFontMetrics( self.font() )
    h = metrics.height()
    textPos = QPoint( h/2 , self._origin.y() + 5 );

    x = self._x + self._origin.x()
    i = 0
    while (i < self.model().rowCount() ):
      p1 = QPoint( x, self._origin.y() - 3  )
      s = str(self.model().headerData( i, Qt.Vertical ))
      painter.save()
      painter.setPen( QPen( QColor(Color.DarkGray), 1.5 ) )
      painter.rotate( -90 )
      painter.translate( -textPos.y() - metrics.width( s ) , p1.x() - self._x + h )
      painter.drawText( 0, 0, s )
      painter.restore()
      i += 1
      x += self._x

    y = self._minBound

    while y <= self._maxBound:
      p1 = QPoint( self._origin.x(), self.valueToPx(y)  )
      if self._order >= 1:
        s = str(round(y))
        s = s.rstrip("0")
        s = s.rstrip(".")
        r = QRect( QPoint( 0, p1.y() - h/2 ), QSize( self._origin.x() - 5 ,h) )
        painter.drawText( r, Qt.AlignRight, s )
      y += self._tickSize

    painter.restore()


  def _paintValues(self, painter, column):
    t = self.columnType(column)

    delegate = None

    if t | Type.Point == t:
      delegate = self._pointDelegate
    elif t == Type.Bar:
      delegate = self._barDelegate

    rows = self.model().rowCount()

    painter.save()
    style = self.columnStyle( column )
    painter.setBrush( style.brush() )
    painter.setPen( style.pen() )

    for r in range(0, rows):
      index = self.model().index( r, column )
      option = QStyleOptionViewItem()
      value = index.data()
      if value < 0:
        option.Position = QStyleOptionViewItem.Bottom
      else:
        option.Position = QStyleOptionViewItem.Top

      option.rect = self.itemRect( index )

      if t | Type.Line == t:
        if r < (rows - 1):
          p1 = option.rect.center()
          p2 = self.itemRect( self.model().index( r + 1, column ) ).center()
          painter.drawLine( p1, p2 )


      if delegate != None:
        delegate.paint( painter, option, index )

    painter.restore()
*/

#endif // LINEARCHART_H
