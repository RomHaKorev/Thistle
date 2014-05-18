#include "abstractchart.h"

#include "abstractchart_p.h"

#include <Qdebug>
#include <QPainter>

#include "legends/chartlegend.h"

namespace Thistle {

    AbstractChart::AbstractChart(QWidget* parent ) : AbstractItemView( new AbstractChartPrivate( this ), parent ) {
        Q_D( AbstractChart );
        this->setContentsMargins( 20, 20, 20, 20 );
    }


    AbstractChart::AbstractChart( AbstractChartPrivate* d, QWidget* parent ) : AbstractItemView( d, parent ) {
        this->setContentsMargins( 20, 20, 20, 20 );
    }


    AbstractChart::~AbstractChart() {
    }

    /*!
    Returns the \c QFont used to write the title.
    \sa setTitleFont()
    */
    const QFont& AbstractChart::titleFont() const {
        const Q_D(AbstractChart);
        return d->titleFont;
    }


    /*!
    Sets the \c QFont used to write the title.
    \sa titleFont()
    */
    void AbstractChart::setTitleFont( const QFont& font ) {
        Q_D(AbstractChart);
        d->titleFont = font;
    }


    void AbstractChart::setLegend( ChartLegend* legend ) {
        Q_D( AbstractChart );
        d->legend = legend;
    }

    ChartLegend* AbstractChart::legend() const {
        const Q_D( AbstractChart );
        return d->legend;
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

    /*!
    Returns the \c QPainterPath defining the item's shape.
    */
    QPainterPath AbstractChart::itemPath(const QModelIndex &index) const {
        QPainterPath path;
        path.addRect( this->itemRect( index ) );
        return path;
    }


    /*void AbstractChart::mousePressEvent( QMouseEvent* ev ) {
    Q_D( AbstractChart );
    }
    void AbstractChart::mouseReleaseEvent( QMouseEvent* ev ) {
    Q_D( AbstractChart );
    }*/

    void AbstractChart::resizeEvent(QResizeEvent * ev) {
        AbstractItemView::resizeEvent( ev );
        this->updateValues();
    }


    void AbstractChart::paintEvent(QPaintEvent *event) {
        Q_D( AbstractChart );
        Q_UNUSED( event )
            if ( this->model() == 0 ) {
                return;
            }
            QPainter painter( viewport() );
            painter.setRenderHint( QPainter::Antialiasing );
            paintChart( painter );
            //paintLegend( painter );
            d->legend->paint( painter );
            painter.setFont( d->titleFont );
            painter.drawText( d->titleRect, Qt::AlignCenter | Qt::TextWordWrap, d->title );
    }

    /*!
    This method should be called when the widget has been resized.
    */
    void AbstractChart::updateValues() {
        this->process();
    }


    /*!
    Analyzes the model to define the bounds and axis values. Calculates the different areas of the chart (title, legend, etc.).
    */
    void AbstractChart::process() {
        Q_D( AbstractChart );
        if ( this->model() == 0 ) {
            return;
        }
        this->scan();
        this->updateRects();
    }

    /*!
    Calculates the rectangle in which the legend should be drawn.
    */
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
        d->legend->area = QRect( this->contentsMargins().left(), this->contentsMargins().top(), maxWidth, nbLines * h );
    }


    /*!
    Sets the title displayed at the bottom.
    */
    void AbstractChart::setTitle( const QString& title ) {
        Q_D( AbstractChart );
        d->title = title;
        this->updateRects();
    }

}