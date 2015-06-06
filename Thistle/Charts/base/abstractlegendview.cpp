#include "abstractlegendview.h"
#include "private/abstractlegendview_p.h"
#include "abstractchart.h"
#include "serieformat.h"

#include <QPainter>
#include <QWidget>
#include <QScrollBar>

namespace Thistle
{
AbstractLegendView::AbstractLegendView( AbstractChart* chart, QWidget* parent )
: AbstractItemView( new AbstractLegendViewPrivate( this, chart ), parent )
{
    this->setFrameShape( QFrame::NoFrame );
}

AbstractLegendView::AbstractLegendView( AbstractLegendViewPrivate* d, QWidget* parent )
: AbstractItemView( d, parent )
{
    this->setFrameShape( QFrame::NoFrame );
}


AbstractLegendView::~AbstractLegendView()
{}


QModelIndex AbstractLegendView::indexAt( const QPoint& point ) const
{
    const Q_D( AbstractLegendView );
    QPoint p = point + QPoint( horizontalOffset(), verticalOffset() );

    if ( ( this->model() == 0 ) || d->itemRects.isEmpty() )
        return QModelIndex();

    int cols = this->model()->columnCount();

    for ( int col = 0; col < cols; ++col )
    {
        QRect r = d->itemRects[ col ];
        if ( r.contains( p ) )
        {
            return this->model()->index( 0, col );
        }
    }
    return QModelIndex();
}


void AbstractLegendView::setScrollBarValues()
{
    Q_D( AbstractLegendView );
    if ( d->parentManageSize )
        return;

    if ( d->sizeHint.height() > this->size().height() )
        this->verticalScrollBar()->setMaximum( d->sizeHint.height() - this->size().height() );
    else
        this->verticalScrollBar()->setMaximum( 0 );

    if ( d->sizeHint.width() > this->size().width() )
        this->horizontalScrollBar()->setMaximum( d->sizeHint.width() - this->size().width() );
    else
        this->horizontalScrollBar()->setMaximum( 0 );
}



QPainterPath AbstractLegendView::itemPath( const QModelIndex& index ) const
{
    const Q_D( AbstractLegendView );

    QPainterPath path;
    if ( index.parent().isValid() )
        if ( index.column() < d->itemRects.count() )
            path.addRect( d->itemRects[index.column()] );

    return path;
}


void AbstractLegendView::updateValues()
{}


QSize AbstractLegendView::sizeHint() const
{
    const Q_D( AbstractLegendView );
    return d->sizeHint;
}


void AbstractLegendView::updateSizeHint( const QSize& source )
{
    Q_D( AbstractLegendView );
    d->calculateSizeHint( source );
}

void AbstractLegendView::paintEvent( QPaintEvent* ev )
{
    Q_UNUSED( ev )

    Q_D( AbstractLegendView );
    if ( this->model() == 0 )
        return;

    QPainter painter( viewport() );
    painter.translate( -this->horizontalOffset(), -this->verticalOffset() );
    painter.setRenderHint( QPainter::Antialiasing );

    painter.save();

    for (int c = 0; c < d->itemRects.count(); ++c )
    {
        this->paintSerie(painter, c, d->itemRects[c] );
    }
    painter.restore();

#if 0
    bool isActive = true;

    if ( this->selectionModel() != 0 )
    {
        QModelIndexList selectedIndexes = this->selectionModel()->selectedIndexes();
        if ( !selectedIndexes.isEmpty() )
        {
            isActive = false;
        }
    }
#endif

}

#if 0
void AbstractLegendView::paintSerie( QPainter& painter, int serie, const QRect& rect ) const
{
    const Q_D( AbstractLegendView );

    QPoint p1( rect.left(), rect.center().y() );
    QPoint p2( rect.left() + 30, rect.center().y() );
    QPoint posText( rect.left() + 35, rect.top() );

    QString s( this->model()->headerData( serie, Qt::Horizontal ).toString() );
    SerieFormat style = d->associatedChart->serieFormat( serie );
    Thistle::Types t = style.type();

    bool isActive = this->isActiveColumn( serie );

    if ( !isActive )
    {
        QPen p = painter.pen();
        QColor c = p.color();
        c.setAlpha( c.alpha() * 0.5 );
        QPen pen( p );
        pen.setColor( c );
        painter.setPen( pen );
        painter.drawText( rect.translated( 40, 0 ), Qt::AlignVCenter, s );
        painter.setPen( p );
    }
    else
        painter.drawText( rect.translated( 40, 0 ), Qt::AlignVCenter, s );

    painter.save();

    QPen pen( style.pen() );
    pen.setWidth( 2 );

    if ( !isActive )
    {
        QColor c( pen.color() );
        c.setAlpha( c.alpha() * 0.5 );
        pen.setColor( c );
    }

    painter.setPen( pen );
    painter.setBrush( style.brush() );

    if ( t.testFlag( Thistle::Area ) )
    {
        painter.setBrush( style.brush() );
        QPolygon poly;
        poly << p1 + QPoint( 0, 8 ) << p1 << p1 + QPoint( 10, -8 ) << p1 + QPoint( 15, -3 ) << p1 + QPoint( 20, -6 ) << p2 << p2 + QPoint( 0, 8 );
        painter.drawPolygon( poly );
    }
    else if ( t.testFlag( Thistle::Line ) )
    {
        painter.drawLine( p1, p2 );
    }
    else if ( t.testFlag( Thistle::Spline ) )
    {
        QBrush brush = painter.brush();
        painter.setBrush( Qt::NoBrush );
        QPainterPath path;
        QPoint p11 = p1 + QPoint( 0, 7 );
        QPoint p22 = p2 - QPoint( 0, 7 );
        path.moveTo( p1 );
        path.cubicTo( p11  + QPoint( 25, 0 ), p22 - QPoint( 25, 0 ), p22 );
        painter.drawPath( path );
        painter.setBrush( brush );
    }
    else if ( t.testFlag( Thistle::Bar ) )
    {
        int j = 0;
        Q_FOREACH( int i, QList<int>() << 15 << 8 << 17 << 5 )
        {
            painter.setPen( Qt::NoPen );
            QRect r( 10 + p1.x() + j * 8, rect.bottom() - i, 5, i );
            painter.drawRect( r );
            j += 1;
        }
    }

    if ( t.testFlag( Thistle::Dot ) )
    {
        QStyleOptionViewItem option;
        option.rect = QRect( p1.x() + abs(p1.x() - p2.x())/2 - 5, p1.y() - 5, 10, 10 );

        if ( isActive )
            d->dotDelegate->paintEnabled( &painter, option, this->model()->index( 0, serie ) );
        else
            d->dotDelegate->paintDisabled( &painter, option, this->model()->index( 0, serie ) );
    }
    painter.restore();
}
#endif


void AbstractLegendView::resizeEvent( QResizeEvent* ev )
{
    Q_D( AbstractLegendView );
    AbstractItemView::resizeEvent( ev );
#if 0
    if ( !d->parentManageSize )
        this->updateSizeHint( ev->size() );
#endif
}

#if 0
void AbstractLegendView::parentManageSize( bool leave )
{
    Q_D( AbstractLegendView );
    d->parentManageSize = leave;
}
#endif

void AbstractLegendView::setSelection( const QRect& rect, QItemSelectionModel::SelectionFlags command )
{
    Q_D( AbstractLegendView );
    QRect contentsRect = rect.translated(
                             this->horizontalScrollBar()->value(),
                             this->verticalScrollBar()->value()).normalized();

    int count = 0;

    for( int col = 0; col < d->itemRects.count(); ++col )
    {
        QRect r = d->itemRects[ col ];
        if ( !r.intersected( contentsRect ).isEmpty() )
        {
            ++count;
            this->selectionModel()->select( this->model()->index( 0, col ), command );
        }
    }
    if ( count == 0 )
    {
        this->selectionModel()->clear();
    }
    this->viewport()->update();
}


bool AbstractLegendView::isActiveColumn( int column ) const
{
    bool isActive = true;
    if ( this->selectionModel() != 0 )
    {
        QModelIndexList selectedIndexes = this->selectionModel()->selectedIndexes();
        if ( !selectedIndexes.isEmpty() )
        {
            isActive = false;
            Q_FOREACH( QModelIndex idx, selectedIndexes )
            {
                if ( idx.column() == column )
                {
                    isActive = true;
                    break;
                }
            }
        }
    }
    return isActive;
}

}
