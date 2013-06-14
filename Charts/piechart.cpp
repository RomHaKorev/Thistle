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

void PieChart::paintChart( QPainter &painter ) {
  updateChart();
  painter.save();
  int rows = model()->rowCount();
  qreal angle = myStartAngle;
  for ( int i = 0; i < rows; ++i ) {
    QModelIndex index = model()->index( i, 0 );

    QColor color( model()->data( index, Qt::DecorationRole ).toString() );
    if ( !color.isValid() ) {
      color = Marb::predefinedColor( i );
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

void PieChart::paintEvent(QPaintEvent *event) {
  Q_UNUSED( event )

  if ( model() == 0 ) {
    return;
  }

  QPainter painter( viewport() );
  painter.setRenderHint( QPainter::Antialiasing );
  paintChart( painter );
  //painter.setClipRect( event->rect() );


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

QPointF PieChart::splittedOffset( qreal angle, qreal delta ) const {
  QPainterPath part;
  part.moveTo( myRect.center() );
  part.arcTo( myRect, -angle, -delta );
  part.closeSubpath();
  QPointF p = part.pointAtPercent( 0.5 );
  QLineF line( p, myRect.center() );
  line.setLength( line.length() * 1.1 );
  p = line.p2();
  return myRect.center() - p;
}

QPainterPath PieChart::itemPart( qreal angle, qreal delta, bool splitted ) const {
  QPainterPath part;
  part.moveTo( myRect.center() );
  part.arcTo( myRect, -angle, -delta );
  if ( splitted == true ) {
    QPointF p = splittedOffset( angle, delta );
    part.translate( p.x(), p.y() );
  }

  part.closeSubpath();

  if ( myRing == true ) {
    QPainterPath p;
    p.addEllipse( myRect.center(), myRect.width() * 0.3, myRect.height() * 0.3 );
    part = part.subtracted( p );
  }
  return part;
}

void PieChart::setSelection(const QRect& rect, QItemSelectionModel::SelectionFlags command ) {
  QRect selectRect = rect;
  selectRect.translate( horizontalOffset(), verticalOffset() );
  selectRect = selectRect.normalized();

  int rows = model()->rowCount();
  QModelIndexList indexes;

  for (int row = 0; row < rows; ++row) {
    QModelIndex index = model()->index(row, 0, rootIndex());
    QPainterPath path = itemPath( index );
    if ( path.intersects( selectRect ) ) {
      indexes << index;
    }
  }

  if (indexes.size() > 0) {
      int firstRow = indexes[0].row();
      int lastRow = indexes[0].row();
      for ( int i = 1; i < indexes.size(); ++i ) {
          firstRow = qMin( firstRow, indexes[i].row() );
          lastRow = qMax( lastRow, indexes[i].row() );
      }

      QModelIndex firstIndex = model()->index( firstRow, 0 );
      QModelIndex lastIndex = model()->index( lastRow, 0 );
      QItemSelection selection( firstIndex, lastIndex );
      selectionModel()->select( selection, command );
      setCurrentIndex( lastIndex );
  } else {
      QModelIndex invalidIndex;
      QItemSelection selection( invalidIndex, invalidIndex );
      selectionModel()->select( selection, command );
  }
  update();
}

void PieChart::setRing( bool ring ) {
  myRing = ring;
}

void PieChart::setSplitted( bool splitted ) {
  mySplitted = splitted;
}

void PieChart::setLegend( QString legend ) {
  myLegend = legend;
}

bool PieChart::save( QString filename ) {
  QPixmap pix( size() );
  pix.fill( Qt::white );
  QPainter painter( &pix );
  painter.setRenderHint( QPainter::Antialiasing );
  paintChart( painter );
  return pix.save( filename );
}
