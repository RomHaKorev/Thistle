#include "abstractchart.h"

#include "abstractchart_p.h"

#include <Qdebug>
#include <QPainter>
#include <QHelpEvent>
#include <QPaintEngine>

#include "legends/chartlegend.h"

namespace Thistle
{

AbstractChart::AbstractChart(QWidget* parent ) : AbstractItemView( new AbstractChartPrivate( this ), parent )
{
    Q_D( AbstractChart );
    this->setContentsMargins( 20, 20, 20, 20 );
    this->setToolTipDuration( 1000 );
}


AbstractChart::AbstractChart( AbstractChartPrivate* d, QWidget* parent ) : AbstractItemView( d, parent )
{
    this->setContentsMargins( 20, 20, 20, 20 );
}


AbstractChart::~AbstractChart()
{
    Q_D( AbstractChart );
}

const QFont& AbstractChart::titleFont() const
{
    const Q_D(AbstractChart);
    return d->titleFont;
}


void AbstractChart::setTitleFont( const QFont& font )
{
    Q_D(AbstractChart);
    d->titleFont = font;
}


void AbstractChart::setLegend( ChartLegend* legend )
{
    Q_D( AbstractChart );
    if ( d->legend != 0 ) delete d->legend;
    d->legend = legend;
}

ChartLegend* AbstractChart::legend() const
{
    const Q_D( AbstractChart );
    return d->legend;
}

void AbstractChart::setScrollBarValues()
{
}


QModelIndex AbstractChart::indexAt(const QPoint& point) const
{
    if ( this->model() == 0 )
    {
        return QModelIndex();
    }
    for( int row = 0; row < this->model()->rowCount(); ++row )
    {
        for( int col = 0; col < this->model()->columnCount(); ++col )
        {
            QModelIndex index = this->model()->index( row, col );
            QPainterPath r = this->itemPath( index );
            if ( r.contains( point ) )
            {
                return index;
            }
        }
    }
    return QModelIndex();
}

QPainterPath AbstractChart::itemPath(const QModelIndex& index) const
{
    QPainterPath path;
    path.addRect( this->itemRect( index ) );
    return path;
}


void AbstractChart::resizeEvent(QResizeEvent* ev)
{
    AbstractItemView::resizeEvent( ev );
    this->updateValues();
}


void AbstractChart::paintEvent(QPaintEvent* event)
{
    Q_D( AbstractChart );
    Q_UNUSED( event )
    if ( this->model() == 0 )
    {
        return;
    }

    QPainter painter( viewport() );
    painter.setRenderHint( QPainter::Antialiasing );
    
    this->paintChart( painter );
    
    d->legend->paint( painter );
    
    painter.setFont( d->titleFont );
    painter.drawText( d->titleRect, Qt::AlignCenter | Qt::TextWordWrap, d->title );
}


void AbstractChart::updateValues()
{
    this->process();
}


void AbstractChart::process()
{
    Q_D( AbstractChart );
    if ( this->model() == 0 )
    {
        return;
    }
    this->scan();
    this->updateRects();
}


void AbstractChart::calculateLegendRect( const QRect& source )
{
    Q_D( AbstractChart );
    QFontMetrics metrics( font() );
    int h = metrics.height() + 5;
    int cols = this->model()->columnCount();
    int nbLines = 1;
    int w = 40;
    int maxWidth = source.width();
    for( int c = 0; c < cols; ++c )
    {
        QString s( this->model()->headerData( c, Qt::Horizontal ).toString() );
        int sWidth = metrics.width( s ) + 40;
        if ( ( w + sWidth ) > maxWidth )
        {
            ++nbLines;
            w = sWidth;
        }
        else
        {
            w += sWidth;
        }
    }
    d->legend->area = QRect( this->contentsMargins().left(), this->contentsMargins().top(), maxWidth, nbLines * h );
}


void AbstractChart::setTitle( const QString& title )
{
    Q_D( AbstractChart );
    d->title = title;
    this->updateRects();
}

bool AbstractChart::event( QEvent* e )
{
    QHelpEvent* he = static_cast<QHelpEvent*>( e );

    if ( he && he->type() == QEvent::ToolTip )
    {
        QModelIndex idx = this->nearestItem( he->pos() );
        QString label = this->model()->headerData( idx.row(), Qt::Horizontal ).toString();
        if ( label == 0 )
            label = QString::number ( idx.row() + 1 );

        if ( idx.isValid() )
            this->setToolTip( QString( "Serie: %1\nX: %2\nY: %3" )
                                .arg( this->model()->headerData( idx.column(), Qt::Vertical ).toString() )
                                .arg( label )
                                .arg( idx.data().toReal() ) );
    }

    return AbstractItemView::event(e);
}

QModelIndex AbstractChart::nearestItem( const QPointF& pos ) const
{
    if ( this->model() == 0 ) return QModelIndex();

    QModelIndex nearest = QModelIndex();
    
    unsigned int nearestLength = -1;

    for ( int row = 0; row < this->model()->rowCount(); ++row )
    {
        for ( int col = 0; col < this->model()->columnCount(); ++col )
        {
            QModelIndex idx = this->model()->index( row, col );
            QPointF p = this->itemRect( idx ).center() - pos;

            unsigned int length = qAbs( p.manhattanLength() );

            if ( qMin( length, nearestLength  ) == length )
            {
                nearestLength = length;
                nearest = idx;
            }
        }
    }
    return nearest;
}

}