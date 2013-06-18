
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



from Marb.Global import Color, Render

from PySide.QtGui import QAbstractItemView, QPen, \
QRegion, QColor, QFontMetrics, QPainter, QPixmap, QPainterPath
from PySide.QtCore import QRect, QPointF, Qt, QModelIndex, QLineF

class PieChart( QAbstractItemView ):
	class ColorConf:
		Normal = 0
		Lighter = 1
		Darker = 2
		Translucent = 4
		
	def __init__(self, parent=None):
		super(PieChart, self).__init__( parent )
		self.setEditTriggers( QAbstractItemView.NoEditTriggers )
		self._Ring = False
		self._Splitted = False
		self._StartAngle = 0.0
		self._Legend = ""
		self._Render = Render.Plain
		
		self._Rect = QRect()
		self._chartRect = QRect()
		self._leftLabelRect = QRect()
		self._rightLabelRect = QRect()

	def configureColor(self, painter, base, flag):
		painter.setPen( QPen( base, 2 ) )
		
		c = base
	
		if flag == PieChart.ColorConf.Darker:
			c = base.darker( 110 )
		elif flag == PieChart.ColorConf.Lighter:
			c = base.lighter( 120 )
			
		if self._Render == Render.Plain:
			painter.setPen( QPen( base.darker( 110 ), 2 ) )
		if self._Render == Render.Translucent:
				c.setAlpha( 180 )
		if self._Render == Render.Wireframe:
				c.setAlpha( 10 )

		painter.setBrush( c )


	def visualRect( self, index ):
		# To force to repaint the whole chart. Not only the area of the part
		return QRect( 0, 0, self.width(), self.height() )
	

	def visualRegionForSelection( self, selection ):
		region = QRegion()
		region += QRect( 0, 0, self.width(), self.height() )
		return region
	

	def scrollTo( self, index, hint ):
		pass


	def indexAt( self, point ):
		x = point.x() + self.horizontalOffset()
		y = point.y() + self.verticalOffset()

		for r in range( 0, self.model().rowCount() ) :
			index = self.model().index( r, 0 )
			path = self.itemPath( index )
			if ( path.contains( QPointF( x, y ) ) ) :
				return index
		
		return QModelIndex()
	

	def moveCursor( self, cursorAction, modifiers ) :
		return QModelIndex()


	def horizontalOffset( self ):
		return 0
	

	def verticalOffset( self ):
		return 0
	

	def isIndexHidden( self, index ):
		return False
	

	def updateChart( self ):
		metrics = QFontMetrics( self.font() )
		maxLabelWidth = 0
		self._Total = 0
		for i in range( 0, self.model().rowCount() ):
			index = self.model().index( i, 0 )
			self._Total += abs( self.model().data( index ) )
			text = str(self.model().headerData( i, Qt.Vertical ))
			maxLabelWidth = max( metrics.width( text ), maxLabelWidth )
		
		self._leftLabelRect = QRect( 0, 0, maxLabelWidth + 20, self.height() )
		self._rightLabelRect = QRect( 0, 0, maxLabelWidth + 20, self.height() )
		
		w = self.width() - self._leftLabelRect.width() - self._rightLabelRect.width()
		h = self.height() - 20
		if self._Legend != "":
			metrics = QFontMetrics( self.font() )
			r = metrics.boundingRect( 10, 0, self.width() - 20, self.height(), Qt.AlignHCenter | Qt.TextWordWrap, self._Legend )
			h -= 10 + r.height()
		
		x = ( self.width() - w ) / 2.0
		y = ( self.height() - h ) / 2.0
		
		if w < h:
			self._Rect = QRect( x, y + (h-w)/2, w , w )
		else :
			self._Rect = QRect( x + (w-h)/2, y, h , h )
		
		if self._Splitted == True:
			w = self._Rect.width() * 0.80
			h = self._Rect.height() * 0.80
			dw = self._Rect.width() - w
			dh = self._Rect.height() - h
			self._chartRect = QRect( self._Rect.x() + dw/2.0, self._Rect.y() + dh/2.0, w, h )
		else:
			self._chartRect = QRect( self._Rect )
		
		self._rightLabelRect.translate( self._Rect.topRight().x(), 0 )
		self._leftLabelRect.translate( self._Rect.topLeft().x() - self._leftLabelRect.width(), 0 )
	

	def paintEvent( self, event ) :
		if ( self.model() == 0 ) :
			return None
		
		painter = QPainter( self.viewport() )
		self.paintChart( painter )


	def paintChart( self, painter ):
		rows = self.model().rowCount()

		self.updateChart()

		painter.save()
		painter.setRenderHint( QPainter.Antialiasing )
		
		angle = self._StartAngle
		
		self._leftLabels = []
		self._rightLabels = []
		
		for i in range( 0, rows ):
			index = self.model().index( i, 0 )
			
			# Detect if model contains a specific color. Otherwise, use a predefined color.
			color = QColor( self.model().data( index, Qt.DecorationRole ) )
			if not color.isValid():
				color = Color.colorAt( i )

			v = abs( self.model().data( index ) )
			delta = 360.0 * v/self._Total
						
			centerAngle = angle + delta/2.0
			if centerAngle < 90: # Right side
				self._rightLabels.append( (i, centerAngle, color) )
			elif centerAngle > 270: # Right side
				self._rightLabels.insert( 0, (i, centerAngle, color) )
			else: # Left side
				self._leftLabels.append( (i, centerAngle, color) )
				
			# A part can be splitted if self._Splitted is True or if self._Splitted is False and the value is selected in the model.
			#  For now, only the first case is implemented (see, "bug in PySide ?")
			if self._Splitted == False:
				self.paintPart( painter, angle, delta, color )
			else:
				self.paintPartSplitted( painter, angle, delta, color )
				
			angle += delta
		
		painter.drawText( 10, self._Rect.bottomLeft().y(),
						self.width() - 20, self.height() - self._chartRect.height(), Qt.AlignHCenter | Qt.TextWordWrap, self._Legend )
		
		#self.paintLabels(painter)
		painter.restore()
		
	
	
	def paintLabels(self, painter):
		# Paint labels
		metrics = QFontMetrics( self.font() )
		ell = QPainterPath()
		if self._Splitted == False:
			ell.addEllipse( self._chartRect )
		else:
			ell.addEllipse( self._Rect )
		
		l = []
		j = 1
		self._leftLabels.reverse()
		while j < len(self._leftLabels):
			(i, centerAngle, color) = self._leftLabels[j]
			prevCenterAngle = self._leftLabels[j - 1][1]
			y1 = ell.pointAtPercent( centerAngle/360.0 ).y()
			y2 = ell.pointAtPercent( prevCenterAngle/360.0 ).y()
			d = abs(y2 - y1)
			y = 0
			if d < (metrics.height() + 2):
				y = y1 + metrics.height() - d + 20
			else:
				y = y1
			l.append( (i, centerAngle, y, color) )
			j += 1
			
		for (i, centerAngle, y, color) in l:
			textPos = QPointF(0,0)
			p1 = QPointF(0,0)
			text = str(self.model().headerData( i, Qt.Vertical ))
			p = ell.pointAtPercent( centerAngle/360.0 )
			textPos = QPointF( self._leftLabelRect.x(), y )
			p1 = QPointF(textPos.x(), p.y() )
			r = QRect( textPos.x(), textPos.y() - metrics.height(), self._leftLabelRect.width(), metrics.height() )
			
			
			painter.drawText( r, text )
			
			pen = painter.pen()
			painter.setPen( color )
			painter.drawLine( p1, p )
			painter.setPen( pen )

	

	def paintPart( self, painter, angle, delta, color ):		
		part = self.itemPart( angle, delta )

		painter.save()
		painter.setClipPath( part ) # To avoid the "borders superposition"
		self.configureColor(painter, color, PieChart.ColorConf.Lighter)
		painter.drawPath( part )

		painter.restore()
	

	def paintPartSplitted( self, painter, angle, delta, color ):
		part = self.itemPart( angle, delta, True )

		painter.save()
		self.configureColor(painter, color, PieChart.ColorConf.Lighter)
		painter.drawPath( part )
		painter.restore()
	

	def itemPath( self, index ):
		path = QPainterPath()
		angle = self._StartAngle
		for r in range( 0, index.row() ):
			idx = self.model().index( r, 0 )
			v = abs( self.model().data( idx ) )
			delta = 360.0 * v/self._Total
			angle += delta
		
		v = abs( self.model().data( index ) )
		delta = 360.0 * v/self._Total

		if index in self.selectionModel().selectedIndexes():
			path = self.itemPart( angle, delta, True )
		else :
			path = self.itemPart( angle, delta )
		
		return path
	
	
	def splittedOffset( self, angle, delta ):
		if self._Splitted == False:
			return ( 0, 0 )
		line = QLineF( QPointF(0,0), QPointF(self._chartRect.width()/2, 0) )
		line.setAngle( -angle - delta/2 )
		p1 = line.p2()
		line.setLength( line.length() * self._Rect.width() / self._chartRect.width() )
		p2 = line.p2()
		return ( p2.x() - p1.x(), p2.y() - p1.y() )


	def itemPart( self, angle, delta, splitted = False ):
		part = QPainterPath()
		part.moveTo( self._chartRect.center() )
		part.arcTo( self._chartRect, -angle, -delta )

		if splitted == True:
			(x, y) = self.splittedOffset(angle, delta)
			part.translate( x, y )
			
		part.closeSubpath()

		if self._Ring == True:
			p = QPainterPath()
			p.addEllipse( self._chartRect.center(), self._chartRect.width() * 0.2, self._chartRect.height() * 0.2 )
			part = part.subtracted( p )
		
		return part
		

	def setRing( self, ring ) :
		self._Ring = ring
	

	def setSplitted( self, splitted ) :
		self._Splitted = splitted
	

	def setLegend( self, legend ) :
		self._Legend = legend
	

	def save( self, filename ):
		pix = QPixmap( self.size() )
		pix.fill( Qt.white )
		painter = QPainter ( pix )
		self.paintChart( painter )
		return pix.save( filename )
	


'''

#include "piechart.h"

#include "Marb.h"
#include <QDebug>
#include <QPainter>
#include <QPaintEvent>

PieChart::PieChart( QWidget* parent ) :
  QAbstractItemView( parent ) {
  setEditTriggers( QAbstractItemView::NoEditTriggers );
  myRing = false;
  mySplitted = false;
  myStartAngle = 0;
  myLegend = QString();
}

QRect PieChart::visualRect( const QModelIndex& index ) const {
  /* To force to repaint the whole chart. Not only the area of the part*/
  return QRect( 0, 0, width(), height() );
}

QRegion PieChart::visualRegionForSelection(const QItemSelection& selection ) const {
  QRegion region;
  region += QRect( 0, 0, width(), height() );
  return region;
}

#if 0
QRect PieChart::visualRect( const QModelIndex& index ) const {
  QPainterPath path = itemPath( index );
  return path.boundingRect().translated( horizontalOffset(), verticalOffset() ).toRect();
}

QRegion PieChart::visualRegionForSelection(const QItemSelection& selection ) const {
  int ranges = selection.count();

  if (ranges == 0)
      return QRect();

  QRegion region;
  for (int i = 0; i < ranges; ++i) {
      QItemSelectionRange range = selection.at(i);
      for (int row = range.top(); row <= range.bottom(); ++row) {
          for (int col = range.left(); col <= range.right(); ++col) {
              QModelIndex index = model()->index(row, col, rootIndex());
              region += visualRect(index);
          }
      }
  }
  return region;
}
#endif

void PieChart::scrollTo(const QModelIndex &/*index*/, ScrollHint /*hint*/) {

}

QModelIndex PieChart::indexAt(const QPoint & point ) const {
  int x = point.x() + horizontalOffset();
  int y = point.y() + verticalOffset();

  for ( int r = 0; r < model()->rowCount(); ++r ) {
    QModelIndex index = model()->index( r, 0 );
    QPainterPath path = itemPath( index );
    if ( path.contains( QPointF( x, y ) ) ) {
      return index;
    }
  }
  return QModelIndex();
}

void PieChart::dataChanged(const QModelIndex &/*topLeft*/, const QModelIndex&/*bottomRight*/) {

}

void PieChart::rowsInserted(const QModelIndex& /*parent*/, int /*start*/, int /*end*/) {

}

void PieChart::rowsAboutToBeRemoved(const QModelIndex& /*parent*/, int /*start*/, int /*end*/) {

}

QModelIndex PieChart::moveCursor(QAbstractItemView::CursorAction cursorAction,
                       Qt::KeyboardModifiers modifiers ) {
  Q_UNUSED( modifiers )

  QModelIndex current = currentIndex();
  if ( !current.isValid() ) {
    return QModelIndex();
  }

  switch ( cursorAction ) {
  case QAbstractItemView::MoveDown:
  case QAbstractItemView::MoveRight:
    if (current.row() == model()->rowCount() - 1) {
      current = model()->index( 0, 0 );
    } else {
      current = model()->index( current.row() + 1, 0 );
    }
  break;
  case QAbstractItemView::MoveUp:
  case QAbstractItemView::MoveLeft:
    if ( current.row() == 0 ) {
      current = model()->index(model()->rowCount() - 1, 0 );
    } else {
      current = model()->index( current.row() - 1 , 0 );
    }
  break;
  default:
  break;
  }
  viewport()->update();
  return current;
}

int PieChart::horizontalOffset() const {
  return 0;
}

int PieChart::verticalOffset() const {
  return 0;
}

bool PieChart::isIndexHidden(const QModelIndex& /*index*/ ) const {
  return false;
}

void PieChart::updateChart() {

  qreal w = ( width() - 40 );
  qreal h = ( height() - 40 );
  if ( !myLegend.isNull() ) {
    QFontMetrics metrics( font() );
    QRect r = metrics.boundingRect( 10, 0, width() - 20, height(), Qt::AlignHCenter | Qt::TextWordWrap, myLegend );
    h -=  10 + r.height();
  }

  if ( w < h ) {
    myRect = QRect( 20, 20 + (h-w)/2, w , w );
  } else {
    myRect = QRect( 20 + (w-h)/2, 20, h , h );
  }

  myRect.translate( myRect.width() * 0.05, myRect.height() * 0.05 );
  myRect.setWidth( 0.9 * myRect.width() );
  myRect.setHeight( 0.9 * myRect.height() );

  myTotal = 0;
  for ( int i = 0; i < model()->rowCount(); ++i ) {
    QModelIndex index = model()->index( i, 0 );
    myTotal += qAbs( model()->data( index ).toReal() );
  }
}

void PieChart::paintEvent(QPaintEvent *event) {

  Q_UNUSED( event )

  if ( model() == 0 ) {
    return;
  }

  int rows = model()->rowCount();

  updateChart();
  QPainter painter( viewport() );
  painter.save();
  painter.setRenderHint( QPainter::Antialiasing );
  //painter.setClipRect( event->rect() );

  qreal angle = myStartAngle;
  for ( int i = 0; i < rows; ++i ) {
    QModelIndex index = model()->index( i, 0 );

    QColor color( model()->data( index, Qt::DecorationRole ).toString() );
    if ( !color.isValid() ) {
      color = Clint::predefinedColor( i );
    }

    qreal v = qAbs( model()->data( index ).toReal() );
    qreal delta = 360.0 * v/myTotal;

    bool isSelected = selectionModel()->selectedIndexes().contains( index )
                      || currentIndex() == index;

    if ( mySplitted == false ) {
      paintPart( painter, angle, delta, color, isSelected );
    } else {
      paintPartSplitted( painter, angle, delta, color, isSelected );
    }
    angle += delta;
  }

  painter.drawText( 10, myRect.bottomLeft().y() + 10,
                    width() - 20, height() - myRect.height(), Qt::AlignHCenter | Qt::TextWordWrap, myLegend );
  painter.restore();  
}

void PieChart::paintPart( QPainter& painter, qreal angle, qreal delta, QColor color, bool isSelected ) {

  if ( isSelected == true ) {
    paintPartSplitted( painter, angle, delta, color );
    return;
  }
  QPainterPath part = itemPart( angle, delta );

  painter.save();
  painter.setClipPath( part ); /* To avoid the "borders superposition" */
  int flag = 0;
  configureColor( painter, color, flag );
  painter.drawPath( part );

  painter.restore();
}

void PieChart::paintPartSplitted( QPainter &painter, qreal angle, qreal delta,
                                  QColor color, bool isSelected ) {

  QPainterPath part = itemPart( angle, delta, true );

  painter.save();
  if ( mySplitted == true
       && ( !selectionModel()->selectedIndexes().isEmpty() || currentIndex().isValid() )
       && isSelected == false ) {
    configureColor( painter, color, 2 );
  } else {
    configureColor( painter, color, 1 );
  }
  painter.drawPath( part );
  painter.restore();
}

void PieChart::configureColor(QPainter &painter, QColor base, int flag ) const {
  switch ( flag ) {
    case 0:
    default:
      painter.setPen( QPen( base.darker( 105 ), 4 ) );
      painter.setBrush( base );
    break;
    case 1:
      painter.setPen( QPen( base.darker( 105 ), 2 ) );
      painter.setBrush( base );
    break;
    case 2:
      base.setAlpha( base.alpha() * 0.75 );
      painter.setPen( QPen( base.lighter( 110 ), 2 ) );
      painter.setBrush( base.lighter( 120 ) );
  }
}

QPainterPath PieChart::itemPath( const QModelIndex& index ) const {
  QPainterPath path;
  qreal angle = myStartAngle;
  for ( int r = 0; r < index.row(); ++r ) {
    QModelIndex id = model()->index( r, 0 );
    qreal v = qAbs( model()->data( id ).toReal() );
    qreal delta = 360.0 * v/myTotal;
    angle += delta;
  }

  qreal v = qAbs( model()->data( index ).toReal() );
  qreal delta = 360.0 * v/myTotal;

  if ( selectionModel()->selectedIndexes().contains( index )) {
    path = itemPart( angle, delta, true );
  } else {
    path = itemPart( angle, delta );
  }
  return path;
}


'''