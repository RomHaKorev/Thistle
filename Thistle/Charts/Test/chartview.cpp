#include "chartview.h"

#include "chartview_p.h"

#include <Qdebug>
#include <QPainter>
#include <QHelpEvent>
#include <QPaintEngine>

#include "legends/chartlegend.h"
#include "axisview.h"

namespace Thistle
{

ChartView::ChartView(QWidget* parent ) : AbstractItemView( new ChartViewPrivate( this ), parent )
{
    Q_D( ChartView );
    this->setContentsMargins( 20, 20, 20, 20 );
    this->setToolTipDuration( 1000 );
}


ChartView::ChartView( ChartViewPrivate* d, QWidget* parent ) : AbstractItemView( d, parent )
{
    this->setContentsMargins( 20, 20, 20, 20 );
}


ChartView::~ChartView()
{
}

const QFont& ChartView::titleFont() const
{
    const Q_D(ChartView);
    return d->titleFont;
}


void ChartView::setTitleFont( const QFont& font )
{
    Q_D(ChartView);
    d->titleFont = font;
}


void ChartView::setLegend( ChartLegend* legend )
{
    Q_D( ChartView );
    if ( d->legend != 0 ) delete d->legend;
    d->legend = legend;
}

ChartLegend* ChartView::legend() const
{
    const Q_D( ChartView );
    return d->legend;
}

void ChartView::setScrollBarValues()
{
}


QModelIndex ChartView::indexAt(const QPoint& point) const
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

QPainterPath ChartView::itemPath(const QModelIndex& index) const
{
    QPainterPath path;
    //path.addRect( this->itemRect( index ) );
    return path;
}


void ChartView::resizeEvent(QResizeEvent* ev)
{
    AbstractItemView::resizeEvent( ev );
    this->updateValues();
}


void ChartView::paintEvent(QPaintEvent* event)
{
    Q_D( ChartView );
    Q_UNUSED( event )
    /*if ( this->model() == 0 )
    {
        return;
    }*/

    QPainter painter( viewport() );
    painter.setRenderHint( QPainter::Antialiasing );
    
    painter.drawRect( d->legend->area );
    painter.drawRect( d->axisView->geometry() );
    painter.drawRect( d->titleRect );
    
    painter.setFont( d->titleFont );
    painter.drawText( d->titleRect, Qt::AlignCenter | Qt::TextWordWrap, d->title );
}


void ChartView::updateValues()
{
    this->process();
}


void ChartView::process()
{
    Q_D( ChartView );
    if ( this->model() == 0 )
    {
        return;
    }
    //this->doLayout();
}


void ChartView::calculateLegendRect( const QRect& source )
{
    Q_D( ChartView );
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

void ChartView::doLayout()
{
    Q_D( ChartView );
    this->calculateLegendRect( this->contentsRect() );

    QRect titleRect;
    if ( d->title != "" )
    {
        QFont font = d->titleFont;
        QFontMetrics metrics( font );
        QRect r( 0, 0, width(), 0 );
        d->titleRect = metrics.boundingRect( r, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, d->title );
    }

    QRect r( this->contentsMargins().left(), d->legend->area.bottom(), this->width() - this->contentsMargins().right(), d->titleRect.top() );

    d->axisView->setGeometry( r );

}

void ChartView::setTitle( const QString& title )
{
    Q_D( ChartView );
    d->title = title;
    this->doLayout();
}

}