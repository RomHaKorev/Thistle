#include "abstractchart.h"

#include "abstractchart_p.h"

#include <QPainter>

namespace Marb {

AbstractChart::AbstractChart(QWidget* parent ) : AbstractItemView( new AbstractChartPrivate(), parent ) {
  this->setContentsMargins( 20, 20, 20, 20 );
}

AbstractChart::AbstractChart( AbstractChartPrivate* d, QWidget* parent ) : AbstractItemView( d, parent ) {
}


AbstractChart::~AbstractChart() {
}

void AbstractChart::setScrollBarValues() {
}


QModelIndex AbstractChart::indexAt(const QPoint &point) const {
    if ( this->model() == 0 ) {
        return QModelIndex();
    }
    for( int row = 0; row < this->model()->rowCount(); ++row ) {
        for( int col = 0; col < this->model()->columnCount(); ++col ) {
            QModelIndex index = this->model()->index( row, col );
            QPainterPath r = this->itemPath( index );
            if ( r.contains( point ) ) {
                return index;
            }
        }
    }
    return QModelIndex();
}


QPainterPath AbstractChart::itemPath(const QModelIndex &index) const {
    QPainterPath path;
    path.addRect( this->itemRect( index ) );
    return path;
}


void AbstractChart::resizeEvent(QResizeEvent * ev) {
    AbstractItemView::resizeEvent( ev );
    this->updateValues();
}


/*Paint the legend in the QRect self._legendRect
The legend corresponds to the text in the Horizontal QHeaderView and the style defined for each column.
*/
void AbstractChart::paintLegend( QPainter& painter) const {
    const Q_D( AbstractChart );
    painter.save();
    QFontMetrics metrics( this->font() );
    int metricsH = metrics.height();
    int h = metricsH + 5;
    int cols = this->model()->columnCount();
    int w = 0;
    int maxWidth = d->legendRect.width();
    QPoint legendPos( d->legendRect.topLeft() );
    QPoint pos = legendPos + QPoint( 50, 0);
    for (int c = 0; c < cols; ++c ) {
        QString s( this->model()->headerData( c, Qt::Horizontal ).toString() );
        int sWidth = metrics.width( s ) + 50;
        QPoint p;
        if ( ( w + sWidth ) > maxWidth ) {
            int y = pos.y();
            p = QPoint( d->legendRect.x() + 10, y + h );
            pos = QPoint( d->legendRect.x() + 50, y + h );
            w = sWidth;
            pos += QPoint( sWidth, 0 );
        } else {
            p = pos + QPoint( -40,    0 );
            w += sWidth;
            pos += QPoint( sWidth, 0 );
        }
        this->paintSerieLegend(painter, c, p + QPoint(0, metricsH), metricsH);
    }
    painter.restore();
}


void AbstractChart::paintEvent(QPaintEvent *event) {
    Q_UNUSED( event )
    if ( this->model() == 0 ) {
        return;
    }
    QPainter painter( viewport() );
    painter.setRenderHint( QPainter::Antialiasing );
    paintChart( painter );
    paintLegend( painter );
}


void AbstractChart::updateValues() {
    this->process();
}


bool AbstractChart::save( const QString& filename ) const {
    QPixmap pix( size() );
    pix.fill( Qt::white );
    QPainter painter( &pix );
    this->paintChart( painter );
    return pix.save( filename );
}


void AbstractChart::process() {
    Q_D( AbstractChart );
    if ( this->model() == 0 ) {
        return;
    }
    d->minBottomMargin = this->scan() + 10.0;
    this->updateRects();
}


void AbstractChart::calculateLegendRect( const QRect& source ) {
    Q_D( AbstractChart );
    QFontMetrics metrics( font() );
    int h = metrics.height() + 5;
    int cols = this->model()->columnCount();
    int nbLines = 1;
    int w = 40;
    int maxWidth = source.width();
    for( int c = 0; c < cols; ++c ) {
        QString s( this->model()->headerData( c, Qt::Horizontal ).toString() );
        int sWidth = metrics.width( s ) + 40;
        if ( ( w + sWidth ) > maxWidth ) {
            ++nbLines;
            w = sWidth;
        } else {
            w += sWidth;
        }
    }
    d->legendRect = QRect( this->contentsMargins().left(), this->contentsMargins().top(), maxWidth, nbLines * h );
}

void AbstractChart::setTitle( const QString& title ) {
    Q_D( AbstractChart );
    d->title = title;
    this->updateRects();
}

}