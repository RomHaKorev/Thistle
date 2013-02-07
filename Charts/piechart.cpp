#include "piechart.h"

#include "Clint.h"
#include <QDebug>
#include <QPainter>
#include <QPaintEvent>

PieChart::PieChart( QWidget* parent ) :
  QAbstractItemView( parent ) {
}

QRect PieChart::visualRect(const QModelIndex &/*index*/) const {
  return QRect();
}

void PieChart::scrollTo(const QModelIndex &/*index*/, ScrollHint /*hint*/) {

}

QModelIndex PieChart::indexAt(const QPoint &/*point*/) const {
  return QModelIndex();
}

void PieChart::dataChanged(const QModelIndex &/*topLeft*/, const QModelIndex&/*bottomRight*/) {

}

void PieChart::rowsInserted(const QModelIndex& /*parent*/, int /*start*/, int /*end*/) {

}

void PieChart::rowsAboutToBeRemoved(const QModelIndex& /*parent*/, int /*start*/, int /*end*/) {

}

QModelIndex PieChart::moveCursor(QAbstractItemView::CursorAction /*cursorAction*/,
                       Qt::KeyboardModifiers /*modifiers*/) {
  return QModelIndex();
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
  qreal w = ( width() - 20 );
  qreal h = ( height() - 20 );

  if ( w < h ) {
    myRect = QRect( 10, 10 + (h-w)/2, w , w );
  } else {
    myRect = QRect( 10 + (w-h)/2, 10, h , h );
  }

  myRect.setHeight( myRect.height() * 0.75 );

  myTotal = 0;
  for ( int i = 0; i < model()->rowCount(); ++i ) {
    QModelIndex index = model()->index( i, 0 );
    myTotal += qAbs( model()->data( index ).toReal() );
  }

}

void PieChart::paintEvent(QPaintEvent *event) {

  if ( model() == 0 ) {
    return;
  }

  int rows = model()->rowCount();

  updateChart();
  QPainter painter( viewport() );
  painter.save();
  painter.setRenderHint( QPainter::Antialiasing );
  painter.setClipRect( event->rect() );

  QPainterPath front;
  front.addRect( myRect.x(),
                myRect.y() + myRect.height()/2,
                myRect.width(),
                myRect.height()/2 + 50 );
  QPainterPath ellipse;
  ellipse.addEllipse( myRect );
  front = front.subtracted( ellipse );
  ellipse.translate( 0, 30 );
  QPainterPath path;
  path.addRect( myRect.x() - 1,
                myRect.y() + myRect.height()/2 - 10,
                myRect.width() + 2, 50 );
  ellipse = ellipse.united( path);
  front = front.intersected( ellipse );
  /*front.addRect( myRect.x() - 1,
                 myRect.y() + myRect.height()/2 - 10,
                 myRect.width() + 2, 50);*/

  //painter.drawEllipse( myRect );
  qreal angle = 0;
  for ( int i = 0; i < rows; ++i ) {
    painter.save();
    QColor c = Clint::predefinedColor( i );
    painter.setPen( QPen(c, 0) );
    //c.setAlpha( c.alpha() * 0.75 );
    painter.setBrush( c );

    QModelIndex index = model()->index( i, 0 );
    qreal v = qAbs( model()->data( index ).toReal() );
    qreal delta = (360.0 /*- rows*/) * v/myTotal;
    painter.drawPie(myRect, int(angle*16), int(delta*16));

    angle += delta /*+ 1*/;
    painter.restore();
  }

  painter.setBrush( Qt::red );
  painter.setPen( Qt::NoPen );
  painter.drawPath( front );
  painter.restore();
}

QRegion PieChart::visualRegionForSelection(const QItemSelection& /*selection*/ ) const {
  return QRegion();
}

void PieChart::setSelection(const QRect &/*rect*/, QItemSelectionModel::SelectionFlags /*command*/ ) {

}
