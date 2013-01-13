#include "chart.h"

#include "pointchart.h"


#include <QFontMetrics>
#include <qmath.h>
#include <QPen>
#include <QPainter>
#include <QPaintEvent>
#include <QScrollBar>
#include <QDebug>

Chart::Chart( QWidget* parent ) : QAbstractItemView( parent ),
  mySpec() {
  myModel = 0;
  myTopChart = -1;
}

void Chart::resizeEvent(QResizeEvent * ev) {

  QAbstractItemView::resizeEvent( ev );
  mySpec.chartSize = QSize( width() - 40 - mySpec.yLabelsLength, height() - 40 );

  foreach( PointChart* p, myCharts.values() ) {
    p->setFixedSize( mySpec.chartSize );
  }
  updateChart();
}

void Chart::setModel( QAbstractItemModel* model ) {
  QAbstractItemView::setModel( model );
  myModel = model;
  mySpec.model = model;
  foreach( PointChart* p, myCharts.values() ) {
    p->setModel( model );
  }
}

void Chart::setSelectionModel( QItemSelectionModel* s ) {
  QAbstractItemView::setSelectionModel( s );
  mySelections = s;
  foreach( PointChart* p, myCharts.values() ) {
    p->setSelectionModel( mySelections );
  }
}

void Chart::hideData( int column ) {
  if ( myCharts.contains( column ) ) {
    myCharts[column]->hide();
    myCharts[column]->deleteLater();
    myCharts.remove( column );
  }
}


Chart::~Chart() {

}

void Chart::updateChart() {
  if ( myCharts.isEmpty() ) {
    return;
  }

  mySpec.calculate( myCharts.keys() );

  foreach( PointChart* p, myCharts.values() ) {
    p->setFixedSize( mySpec.chartSize );
  }

  foreach( PointChart* p, myCharts.values() ) {
    p->setBounds( mySpec.min, mySpec.max );
    p->setManualbounds( true );
  }

  myOrigin = QPointF( 0, mySpec.valueToPixel( 0 ) );

  viewport()->update();
}

int Chart::horizontalOffset() const {
  return horizontalScrollBar()->value();
}

QModelIndex Chart::indexAt(const QPoint &point) const {
  return QModelIndex();
}

bool Chart::isIndexHidden(const QModelIndex& /*index*/ ) const {
  return false;
}

QRect Chart::itemRect(const QModelIndex& /*index*/ ) const {
    return QRect();
}


QModelIndex Chart::moveCursor( QAbstractItemView::CursorAction cursorAction,
                       Qt::KeyboardModifiers /*modifiers*/ ) {

  int minRow = model()->rowCount() - 1;
  int minCol = model()->columnCount() - 1;
  int maxRow = 0;
  int maxCol = 0;

  foreach( QItemSelectionRange range, mySelections->selection() ) {
    minRow = qMin( minRow, range.bottom() );
    maxRow = qMax( maxRow, range.top() );
    minCol = qMin( minCol, range.left() );
    maxCol = qMax( maxCol, range.right() );
  }

  if ( cursorAction == MoveLeft ) {
    if ( minRow > 0 ) {
      --minRow;
    }
    if ( maxRow > 0 ) {
      --maxRow;
    }
  } else if ( cursorAction == MoveRight ) {
    if ( minRow < model()->rowCount() - 1 ) {
      ++minRow;
    }
    if ( maxRow < model()->rowCount() - 1 ) {
      ++maxRow;
    }
  }

  QItemSelection selection(
                            model()->index(minRow, minCol, rootIndex()),
                            model()->index(maxRow, maxCol, rootIndex())
                          );
  mySelections->select( selection, QItemSelectionModel::Clear | QItemSelectionModel::Select );
  viewport()->update();
  return QModelIndex();
}


void Chart::paintAxis( QPainter& painter ) {
  painter.save();
  painter.setRenderHint( QPainter::Antialiasing, false );
  painter.translate( mySpec.chartPos );
  painter.setPen( QPen(QColor( 20, 20, 20, 180 ), 1, Qt::SolidLine) );
  painter.setBrush(Qt::NoBrush);
  QPoint start = QPoint( -10, myOrigin.y() );
  QPoint end = QPoint( mySpec.chartSize.width() + 10, myOrigin.y() );
  painter.drawLine( start, end );
  start = QPoint( myOrigin.x(), 0 );
  end = QPoint( myOrigin.x(), mySpec.chartSize.height() );
  painter.drawLine( start, end );
  painter.restore();
}

void Chart::paintGrid( QPainter& painter ) {
  painter.save();
  painter.translate( mySpec.chartPos );

  QPoint start( 0, myOrigin.y() );
  QPoint end( mySpec.chartSize.width(), myOrigin.y() );

  for ( qreal i = mySpec.gridStartValue() + mySpec.yStep; i < mySpec.max;
        i +=  mySpec.yStep ) {
    start.setY( mySpec.valueToPixel( i ) );
    end.setY( mySpec.valueToPixel( i ) );
    painter.setPen(QPen(Qt::lightGray, 0, Qt::SolidLine));
    painter.drawLine( start, end );
    painter.setPen(QPen(Qt::gray, 0, Qt::SolidLine));
    QPointF p = start - QPoint( mySpec.yLabelsLength + 3, 0 );
    painter.drawText( p, QString::number( i ) );
  }

  painter.restore();
  painter.save();
  painter.setPen(QColor( 20, 20, 20 ));
  painter.translate( mySpec.chartPos );
  start = QPoint( myOrigin.x(), myOrigin.y() - 5 );
  end = QPoint( myOrigin.x(), myOrigin.y() + 5 );

  for( int i = 0; i < model()->rowCount(); ++i ) {
    start.setX( mySpec.xScale * i );
    end.setX( mySpec.xScale * i );
    painter.drawLine( start, end );
  }
  painter.restore();
}

void Chart::paintText( QPainter& painter ) {
  painter.save();
  painter.translate( mySpec.chartPos);
  painter.setRenderHint( QPainter::TextAntialiasing );
  painter.setPen(QPen(QColor( 20, 20, 20 ), 0, Qt::SolidLine));
  QFontMetrics metrics (font());

  qreal textHeight = metrics.height();
  if ( mySpec.xLabelsLength < ( mySpec.xScale / 2 ) ) {
    QPointF textPos = myOrigin;
    for ( int i = 0; i < model()->rowCount(); ++i ) {
      QString header = model()->headerData( i, Qt::Vertical ).toString();
      QPointF p = textPos + QPointF( mySpec.xScale * i, textHeight );
      painter.drawText( p, header );
    }
  } else {
    QPointF textPos = myOrigin + QPointF( -5, 7 );
    for ( int i = 0; i < model()->rowCount(); ++i ) {
      painter.save();
      QString header = model()->headerData( i, Qt::Vertical ).toString();
      qreal textWidth = metrics.width( header );
      painter.rotate( -90 );
      painter.translate( -textPos.y() - textWidth , textPos.x() + mySpec.xScale * i + textHeight );
      painter.drawText( QPointF(0, 0), header );
      painter.restore();
    }
  }
  painter.restore();
}

void Chart::paintEvent( QPaintEvent* event ) {

  QPainter painter( viewport() );
  painter.setClipRegion( event->region() );
  painter.setRenderHint( QPainter::Antialiasing );

  painter.save();
  paintGrid( painter );
  painter.translate( mySpec.chartPos );

  QList<int> keys = myCharts.keys();
  keys.removeAll( myTopChart );

  foreach( int k, keys ) {
    myCharts[ k ]->paintChart( painter );
  }

  if ( myTopChart != -1 && myCharts.contains( myTopChart ) ) {
    myCharts[ myTopChart ]->paintChart( painter );
  }
  painter.restore();
  paintAxis( painter );
  paintText( painter );
}

int Chart::rows( const QModelIndex& index ) const {
  return model()->rowCount( model()->parent( index ) );
}

void Chart::scrollTo( const QModelIndex& /*index*/, ScrollHint /*hint*/ ) {
}

void Chart::setSelection( const QRect& r, QItemSelectionModel::SelectionFlags command ) {
  mySelections->clear();

  QRect rect = r;

  if ( rect.width() < 10 ) {
    rect.translate( -5 + rect.width(), 0 );
    rect.setWidth( 10 );
  }

  if ( rect.height() < 10 ) {
    rect.translate( 0, -5 + rect.height() );
    rect.setHeight( 10 );
  }

  foreach ( PointChart* p, myCharts ) {
    p->setSelection( rect.translated( -1 * mySpec.chartPos ), command );
    if ( !mySelections->selection().isEmpty() ) {
      viewport()->update();
      return;
    }
  }
  viewport()->update();
}

int Chart::verticalOffset() const {
  return verticalScrollBar()->value();
}

QRect Chart::visualRect(const QModelIndex &index) const {
  QRect rect = itemRect(index);
  if (rect.isValid())
      return QRect(rect.left() - horizontalScrollBar()->value(),
                   rect.top() - verticalScrollBar()->value(),
                   rect.width(), rect.height());
  else
      return rect;
}

QRegion Chart::visualRegionForSelection(const QItemSelection& /*selection*/ ) const {
  return QRegion();
}

/***************************************
**
**        PROTECTED SLOTS
**
***************************************/

void Chart::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight) {
  QAbstractItemView::dataChanged(topLeft, bottomRight);
  updateChart();
}

void Chart::rowsAboutToBeRemoved(const QModelIndex& parent, int start, int end) {
  QAbstractItemView::rowsAboutToBeRemoved(parent, start, end);
  updateChart();
}

void Chart::rowsInserted(const QModelIndex& parent, int start, int end) {
  QAbstractItemView::rowsInserted(parent, start, end);
  updateChart();
}

void Chart::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
  QAbstractItemView::selectionChanged( selected, deselected );
  if ( selected.isEmpty() ) {
    myTopChart = -1;
  } else {
    myTopChart = selected.first().indexes().first().column();
  }
  updateChart();
}
