#include "abstractchart.h"

#include <QPainter>


AbstractChart::AbstractChart(QWidget* parent ) : MarbAbstractItemView( parent ) {
  this->setContentsMargins( 20, 20, 20, 20 );
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
    MarbAbstractItemView::resizeEvent( ev );
    this->updateValues();
}


/*Paint the legend in the QRect self._legendRect
The legend corresponds to the text in the Horizontal QHeaderView and the style defined for each column.
*/
void AbstractChart::paintLegend( QPainter& painter) const {
        painter.save();
        QFontMetrics metrics( this->font() );
        int metricsH = metrics.height();
        int h = metricsH + 5;
        int cols = this->model()->columnCount();
        int w = 0;
        int maxWidth = myLegendRect.width();
        QPoint legendPos( myLegendRect.topLeft() );
        QPoint pos = legendPos + QPoint( 50, 0);
        for (int c = 0; c < cols; ++c ) {
            QString s( this->model()->headerData( c, Qt::Horizontal ).toString() );
            int sWidth = metrics.width( s ) + 50;
            QPoint p;
            if ( ( w + sWidth ) > maxWidth ) {
                int y = pos.y();
                p = QPoint( myLegendRect.x() + 10, y + h );
                pos = QPoint( myLegendRect.x() + 50, y + h );
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


bool AbstractChart::save( QString filename ) {
    QPixmap pix( size() );
    pix.fill( Qt::white );
    QPainter painter( &pix );
    this->paintChart( painter );
    return pix.save( filename );
}


void AbstractChart::process() {
    if ( this->model() == 0 ) {
        return;
    }
    myMin = 0;
    myMax = 0;
    myMinBottomMargin = this->scan() + 10.0;
    this->updateRects();
}


void AbstractChart::calculateLegendRect( const QRect& source ) {
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
        myLegendRect = QRect( this->contentsMargins().left(), this->contentsMargins().top(), maxWidth, nbLines * h );
}

void AbstractChart::setTitle( const QString& title ) {
    myTitle = title;
    this->updateRects();
}
