#include "chart.h"

#include "pointchart.h"
#include "linechart.h"
#include "barchart.h"

#include <QFontMetrics>
#include <qmath.h>
#include <QPen>
#include <QPainter>
#include <QPaintEvent>
#include <QScrollBar>
#include <QDebug>

Chart::Chart( QWidget* parent ) : QAbstractItemView( parent ),
  my_spec() {
  my_model = 0;
  my_topChart = -1;
}

void Chart::resizeEvent(QResizeEvent * ev) {

  QAbstractItemView::resizeEvent( ev );
  my_spec.chartSize = QSize( width() - 40 - my_spec.yLabelsLength, height() - 40 );
  foreach( PointChart* p, my_charts.values() ) {
    p->setFixedSize( my_spec.chartSize );
  }
  updateChart();
}

void Chart::setModel( QAbstractItemModel* model ) {
  QAbstractItemView::setModel( model );
  my_model = model;
  my_spec.model = model;
  foreach( PointChart* p, my_charts.values() ) {
    p->setModel( model );
  }
}

void Chart::setSelectionModel( QItemSelectionModel* s ) {
  QAbstractItemView::setSelectionModel( s );
  my_selections = s;
  foreach( PointChart* p, my_charts.values() ) {
    p->setSelectionModel( my_selections );
  }
}

void Chart::showData( int column, Chart::ChartStyle style ) {
  if ( my_model != 0 && column < my_model->columnCount() ) {
    if ( my_charts.contains( column ) ) {
      my_charts[column]->hide();
      my_charts[column]->deleteLater();
    }
    qDebug() << "ajout Chart" << style;
    PointChart* chart;
    if ( style == Chart::Point ) {
      chart = new PointChart();
    } else if ( style == Line ) {
      chart = new LineChart();
    } else if ( style == Bar ) {
      int nb = 1;
      foreach( PointChart* p, my_charts ) {
        BarChart* bar = qobject_cast<BarChart*>( p );
        if ( bar != 0 ) {
          ++nb;
        }
      }
      BarChart* bar = new BarChart();
      bar->setWidthRatio( nb, 0 );
      chart = bar;
      int i = 1;
      foreach( PointChart* p, my_charts ) {
        BarChart* bar = qobject_cast<BarChart*>( p );
        if ( bar != 0 ) {
          bar->setWidthRatio( nb, i );
          ++i;
        }
      }
    } else {
      chart = new PointChart();
    }
    QPalette p;
    p.setColor( QPalette:: Base, Qt::transparent );
    chart->setPalette( p );
    chart->resize( size() );
    chart->setModel( my_model );
    chart->setSelectionModel( my_selections );
    chart->setColumnData( column );
    chart->move( my_spec.chartPos );
    chart->setObjectName( "Chart " + QString::number( column ) );
    my_charts.insert( column, chart );
    chart->setColor( (Chart::PredefinedColor)(my_charts.count()) );
  } else {
    qWarning() << "Could not show data : Invalid model or column";
  }
  updateChart();
}

void Chart::hideData( int column ) {

}


Chart::~Chart() {

}

void Chart::updateChart() {
  if ( my_charts.isEmpty() ) {
    return;
  }

  my_spec.calculate( my_charts.keys() );

  foreach( PointChart* p, my_charts.values() ) {
    p->setFixedSize( my_spec.chartSize );

  }

  foreach( PointChart* p, my_charts.values() ) {
    p->setBounds( my_spec.min, my_spec.max );
    p->setManualbounds( true );
  }

  my_origin = QPointF( 0, my_spec.valueToPixel( 0 ) );

  viewport()->update();
}

int Chart::horizontalOffset() const {
  return horizontalScrollBar()->value();
}

QModelIndex Chart::indexAt(const QPoint &point) const {
  foreach( PointChart* p, my_charts ) {
    QModelIndex id = p->indexAt( point - my_spec.chartPos );
    if ( id.isValid() ) {
      return id;
    }
  }
  return QModelIndex();
}

bool Chart::isIndexHidden(const QModelIndex &index) const {
  return false;
}

QRect Chart::itemRect(const QModelIndex &index) const {
    return QRect();
}


QModelIndex Chart::moveCursor( QAbstractItemView::CursorAction cursorAction,
                       Qt::KeyboardModifiers modifiers ) {

  int minRow = model()->rowCount() - 1;
  int minCol = model()->columnCount() - 1;
  int maxRow = 0;
  int maxCol = 0;

  foreach( QItemSelectionRange range, my_selections->selection() ) {
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
  my_selections->select( selection, QItemSelectionModel::Clear | QItemSelectionModel::Select );
  viewport()->update();
  return QModelIndex();

}


void Chart::paintAxis( QPainter* painter ) {
  painter->save();
  painter->setRenderHint( QPainter::Antialiasing, false );
  painter->translate( my_spec.chartPos );
  painter->setPen( QPen(QColor( 20, 20, 20, 180 ), 1, Qt::SolidLine) );
  painter->setBrush(Qt::NoBrush);
  QPoint start = QPoint( -10, my_origin.y() );
  QPoint end = QPoint( my_spec.chartSize.width() + 10, my_origin.y() );
  painter->drawLine( start, end );
  start = QPoint( my_origin.x(), 0 );
  end = QPoint( my_origin.x(), my_spec.chartSize.height() );
  painter->drawLine( start, end );
  painter->restore();

}

void Chart::paintGrid( QPainter* painter ) {
  painter->setPen(QPen(Qt::lightGray, 0, Qt::SolidLine));

  QPoint start( 0, my_origin.y() );
  QPoint end( my_spec.chartSize.width(), my_origin.y() );

  for ( qreal i = my_spec.gridStartValue() + my_spec.yStep; i < my_spec.max;
        i +=  my_spec.yStep ) {
    start.setY( my_spec.valueToPixel( i ) );
    end.setY( my_spec.valueToPixel( i ) );
    painter->drawLine( start, end );
    painter->drawText( start - QPoint( my_spec.yLabelsLength + 3, 0 ), QString::number( i ) );
  }

  start = QPoint( my_origin.x(), my_origin.y() - 5 );
  end = QPoint( my_origin.x(), my_origin.y() + 5 );

  for( int i = 0; i < model()->rowCount(); ++i ) {
    start.setX( my_spec.xScale * i );
    end.setX( my_spec.xScale * i );
    painter->drawLine( start, end );
  }
}

void Chart::paintText( QPainter* painter ) {
  painter->save();
  painter->setRenderHint( QPainter::TextAntialiasing );
  painter->setPen(QPen(Qt::lightGray, 0, Qt::SolidLine));
  QFontMetrics metrics (font());
  QPointF textPos = my_origin + QPointF( 0, 3 );
  qreal textHeight = metrics.height();
  if ( my_spec.xLabelsLength < ( my_spec.xScale / 2 ) ) {
    for ( int i = 0; i < model()->rowCount(); ++i ) {
      QString header = model()->headerData( i, Qt::Vertical ).toString();
      painter->drawText( textPos + QPointF( my_spec.xScale * i, textHeight ), header );
    }
  } else {
    for ( int i = 0; i < model()->rowCount(); ++i ) {
      painter->save();
      QString header = model()->headerData( i, Qt::Vertical ).toString();
      qreal textWidth = metrics.width( header );
      painter->rotate( -90 );
      painter->translate( -textPos.y() - textWidth , textPos.x() + my_spec.xScale * i + textHeight / 2 );
      painter->drawText( QPointF(0, 0), header );
      painter->restore();
    }
  }
  painter->restore();
}

void Chart::paintEvent( QPaintEvent* event ) {

  QPainter painter( viewport() );
  painter.setClipRect( event->rect() );
  painter.setRenderHint( QPainter::Antialiasing );

  painter.save();
  painter.translate( my_spec.chartPos );

  paintGrid( &painter );

  QList<int> keys = my_charts.keys();
  keys.removeAll( my_topChart );

  foreach( int k, keys ) {
    my_charts[ k ]->paintChart( &painter );
  }

  if ( my_topChart != -1 && my_charts.contains( my_topChart ) ) {
    my_charts[ my_topChart ]->paintChart( &painter );
  }
  painter.restore();
  paintAxis( &painter );
  paintText( &painter );

}

int Chart::rows( const QModelIndex& index ) const {
  return model()->rowCount( model()->parent( index ) );
}

void Chart::scrollTo( const QModelIndex& index, ScrollHint hint ) {

}

void Chart::setSelection( const QRect& r, QItemSelectionModel::SelectionFlags command ) {
  my_selections->clear();

  QRect rect = r;

  if ( rect.width() < 10 ) {
    rect.translate( -5 + rect.width(), 0 );
    rect.setWidth( 10 );
  }

  if ( rect.height() < 10 ) {
    rect.translate( 0, -5 + rect.height() );
    rect.setHeight( 10 );
  }

  foreach ( PointChart* p, my_charts ) {
    p->setSelection( rect.translated( -1 * my_spec.chartPos ), command );
    if ( !my_selections->selection().isEmpty() ) {
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

QRegion Chart::visualRegionForSelection(const QItemSelection &selection) const {
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
    my_topChart = -1;
  } else {
    my_topChart = selected.first().indexes().first().column();
  }
  updateChart();
}
