#include "Pointchart.h"

#include <QPen>
#include <QPainter>
#include <QPaintEvent>
#include <QScrollBar>
#include <QDebug>

PointChart::PointChart( QWidget* parent ) : QAbstractItemView( parent ) {
  my_max = 0;
  my_min = 0;
  my_manualBounds = true;

  setColor( Chart::Blue );

  my_pointShape = Chart::Circle;
  my_col = 0;
  setAttribute( Qt::WA_TransparentForMouseEvents );
  setFrameStyle( QFrame::NoFrame );
}

PointChart::~PointChart() {

}

void PointChart::setBounds( qreal min, qreal max ) {
  my_min = min;
  my_max = max;
}

void PointChart::setManualbounds( bool manual ) {
  my_manualBounds = manual;
}

QColor PointChart::Color() const {
  return my_color;
}

void PointChart::setColor( QColor c ) {
  my_color = c;
}

void PointChart::setColor( Chart::PredefinedColor c ) {
  switch( c) {
  case Chart::Blue:
    my_color = QColor( 80, 120, 190 );
  break;
  case Chart::Green:
    my_color = QColor( 170, 220, 90 );
  break;
  case Chart::Red:
    my_color = QColor( 250, 70, 70 );
  break;
  case Chart::Purple:
    my_color = QColor( 130, 100, 160 );
  break;
  case Chart::Orange:
    my_color = QColor( 250, 120, 20 );
  break;
  }
}

void PointChart::updateMinMax() {
  if ( my_manualBounds == false ) {
    if ( model()->rowCount() != 0 ) {
      QModelIndex index = model()->index( 0, my_col );
      my_max = model()->data( index ).toReal();
      my_min = my_max;
    }

    for ( int i = 1; i < model()->rowCount(); ++i ) {
      QModelIndex index = model()->index( i, my_col );
      qreal v = model()->data( index ).toReal();
      if ( my_max < v ) {
        my_max = v;
      }
      if ( my_min > v ) {
        my_min = v;
      }
    }
  }
}

void PointChart::updateChart() {
  my_points.clear();
  if ( model() == 0 || model()->rowCount() == 0 ) {
    return;
  }

  qreal h = height();
  qreal w = width();

  qreal xScale = w / model()->rowCount();

  /* Equation of coordinates transform */
  qreal m = -h /( my_max - my_min );
  qreal p = h - m * my_min;

  qreal x = 0;
  for ( int i = 0; i < model()->rowCount(); ++i ) {
    QModelIndex index = model()->index( i, my_col );
    qreal v = model()->data( index ).toReal();
    my_points << QPointF( x, m * v + p );
    x += xScale;
  }
}

int PointChart::horizontalOffset() const {
  return horizontalScrollBar()->value();
}

QModelIndex PointChart::indexAt(const QPoint &point) const {

  QPainterPath path;
  path.addEllipse( point, 30, 30 );
  path.translate( QPointF(horizontalScrollBar()->value(), verticalScrollBar()->value()) );

  qreal w = width();
  qreal h = height();
  qreal xScale = w / model()->rowCount();

  if  ( model()->rowCount() > 1 ) {
    xScale = w / (model()->rowCount() - 1 );
  } else {
    xScale = w / model()->rowCount();
  }

  qreal m = -h /( my_max - my_min );
  qreal p = h - m * my_min;

  for ( int i = 0; i < model()->rowCount(); ++i ) {
    QModelIndex index = model()->index( i, my_col );
    qreal v = model()->data( index ).toReal();
    qreal x = xScale * index.row() + 10;
    if ( path.contains( QPointF( x, m * v + p ) ) ) {
      return index;
    }

  }
  return QModelIndex();
}

bool PointChart::isIndexHidden(const QModelIndex &index) const {

  return false;
}

QRect PointChart::itemRect(const QModelIndex &index) const {
  if ( index.row() >= my_points.count() ) {
    return QRect();
  }
  QPointF p = my_points.at( index.row() ) - QPointF( 10, 10 );
  return QRect( p.toPoint(), QSize(20, 20) );
}

QModelIndex PointChart::moveCursor( QAbstractItemView::CursorAction cursorAction,
                       Qt::KeyboardModifiers modifiers ) {
  return QModelIndex();
}

void PointChart::paintChart( QPainter* painter ) {

  painter->save();
  painter->setRenderHint( QPainter::Antialiasing );
  updateChart();
  qreal yScale = 1;
  if ( my_max - my_min != 0 ) {
    yScale = height() / ( my_max - my_min );
  }

  QPainterPath shape;
  QPainterPath selectedShape;
  switch( my_pointShape ) {
    case Chart::None:
    break;
    case Chart::Circle:
      shape.addEllipse( QPointF(0,0), 4, 4 );
      selectedShape.addEllipse( QPointF(0,0), 6, 6 );
    break;
    case Chart::Square:
      shape.addRect( -4, -4, 8, 8 );
      selectedShape.addRect( -6, -6, 12, 12 );
    break;
    case Chart::Triangle:
      shape.addPolygon( QPolygonF() << QPointF( -4, 4 ) << QPointF( 4, 4 ) << QPointF( 0, -4 ) );
      selectedShape.addPolygon( QPolygonF() << QPointF( -6, 6 ) << QPointF( 6, 6 ) << QPointF( 0, -6 ) );
    break;
    case Chart::Diamond:
    break;
  }

  painter->setPen( Qt::transparent );
  painter->setBrush( my_color );
  if ( my_pointShape != Chart::None ) {
    foreach( QPointF p, my_points ) {
      painter->drawPath( shape.translated( p ) );
    }

    for ( int r = 0; r < model()->rowCount(); ++r ) {
      QModelIndex id = model()->index( r, my_col, rootIndex() );
      if ( selectionModel()->isSelected( id ) ||
           currentIndex() == id ) {
        painter->drawPath( selectedShape.translated( my_points.at( r ) ) );
      }
    }
  }
  painter->restore();
}

int PointChart::rows(const QModelIndex &index) const {
  return model()->rowCount(model()->parent(index));
}

void PointChart::scrollTo(const QModelIndex &index, ScrollHint hint ) {

}

void PointChart::setSelection( const QRect& rect, QItemSelectionModel::SelectionFlags command ) {

  QRect selectRect = rect;

  int rows = model()->rowCount(rootIndex());
  QModelIndexList indexes;

  for (int row = 0; row < rows; ++row) {
    QModelIndex index = model()->index(row, my_col, rootIndex());
    if ( !itemRect( index ).intersect( selectRect ).isEmpty() ) {
      indexes.append( index );
    }
  }

  if ( indexes.isEmpty() ) {
    QModelIndex dummyIndex;
    QItemSelection selection(dummyIndex, dummyIndex);
    selectionModel()->select(selection, command);
    return;
  }
    int firstRow = indexes[0].row();
    int lastRow = indexes[0].row();

    for (int i = 1; i < indexes.size(); ++i) {
        firstRow = qMin(firstRow, indexes[i].row());
        lastRow = qMax(lastRow, indexes[i].row());
    }

    QItemSelection selection(
                              model()->index(firstRow, my_col, rootIndex()),
                              model()->index(lastRow, my_col, rootIndex())
                            );

    selectionModel()->select(selection, command);
    qDebug() << selectionModel()->selection();
    viewport()->update();
}

int PointChart::verticalOffset() const {
  return verticalScrollBar()->value();
}

QRect PointChart::visualRect(const QModelIndex &index) const {
  QRect rect = itemRect(index);
  if (rect.isValid())
      return QRect(rect.left() - horizontalScrollBar()->value(),
                   rect.top() - verticalScrollBar()->value(),
                   rect.width(), rect.height());
  else
      return rect;
}

QRegion PointChart::visualRegionForSelection(const QItemSelection &selection) const {

  int ranges = selection.count();

  QRegion region;
  for (int i = 0; i < ranges; ++i) {

      QItemSelectionRange range = selection.at(i);
      for (int row = range.top(); row <= range.bottom(); ++row) {
        QModelIndex index = model()->index(row, my_col, rootIndex());
        region += visualRect(index);
      }
  }
  viewport()->update();
  return region;
}


int PointChart::columnData() {
  return my_col;
}

void PointChart::setColumnData( int c ) {
  if ( c < model()->columnCount() && c >= 0 ) {
    my_col = c;
  }
}

/***************************************
**
**        PROTECTED SLOTS
**
***************************************/

void PointChart::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight) {
  QAbstractItemView::dataChanged(topLeft, bottomRight);
  updateChart();
}

void PointChart::rowsAboutToBeRemoved(const QModelIndex& parent, int start, int end) {
  QAbstractItemView::rowsAboutToBeRemoved(parent, start, end);
  updateChart();
}

void PointChart::rowsInserted(const QModelIndex& parent, int start, int end) {
  QAbstractItemView::rowsInserted(parent, start, end);
  updateChart();
}
