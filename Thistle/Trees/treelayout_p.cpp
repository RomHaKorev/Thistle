#include "treelayout_p.h"
#include "treeview.h"

#include <qmath.h>

namespace Thistle
{

TreeLayoutPrivate::TreeLayoutPrivate()
    : view( 0 ), margins( 20, 10, 20, 10 ),
      rect( -50, -40, 100, 80 ), orientation( Qt::Vertical )
{}

void TreeLayoutPrivate::calculateViewPositions()
{
    int xMargin = this->margins.right() + this->margins.left();
    int yMargin = this->margins.bottom() + this->margins.top();

    QPolygonF polygon;

    if ( this->orientation == Qt::Vertical )
    {

        QPointF offset( this->itemRect().width()/2, this->itemRect().height()/2 );
        offset += this->itemOffset;
        Q_FOREACH( QModelIndex index, this->itemTreePos.keys() )
        {
            QPointF p = this->itemTreePos[ index ];
            this->itemPos[index] = QPointF( p.x() * ( xMargin + this->itemRect().width() ), p.y() * ( yMargin + this->itemRect().height() ) ) + offset;
            polygon << this->itemPos[index];
        }
    }
    else
    {
        QPointF offset( this->itemRect().width()/2, this->itemRect().height()/2 );
        offset += this->itemOffset;
        Q_FOREACH( QModelIndex index, this->itemTreePos.keys() )
        {
            QPointF p = this->itemTreePos[ index ];
            this->itemPos[index] = QPointF( p.y() * ( xMargin + this->itemRect().width() ), p.x() * ( yMargin + this->itemRect().height() ) ) + offset;
            polygon << this->itemPos[index];
        }
    }

    this->boundingRect = polygon.boundingRect().toRect();
    this->boundingRect.translate( -this->itemRect().width() / 2, -this->itemRect().height() / 2 );
    this->boundingRect.setWidth( this->itemRect().width() + this->margins.left() + this->margins.right() + this->boundingRect.width() );
    this->boundingRect.setHeight( this->itemRect().height() + this->margins.top() + this->margins.bottom() + this->boundingRect.height() );
}

void TreeLayoutPrivate::calculateTreePositions()
{
    this->itemTreePos.clear();
    this->depth = this->scan( this->view->model()->index(0,0), QPointF(0,0)).y();
    this->left = 0;
    Q_FOREACH( QModelIndex idx, this->itemTreePos.keys() )
    {
        QPointF p = this->itemTreePos[idx];
        this->left = qMax( this->left, p.x() );
    }
    this->depth -= 1;
}

QPointF TreeLayoutPrivate::scan( const QModelIndex& index, const QPointF& ld )
{
    QPointF leftDepth( ld );
    int rows = this->view->model()->rowCount( index );

    if ( !index.isValid() )
    {
        return QPointF(0, 0);
    }
    else if ( rows == 0 )
    {
        this->setX( index, leftDepth.x() );
        this->setY( index, leftDepth.y() );
        return QPointF(leftDepth.x() + 1, 1);
    }

    qreal childDepth = 0;
    for ( int r = 0; r < rows; ++r )
    {
        QModelIndex child = index.child( r, 0 );
        QPointF p = this->scan( child, leftDepth + QPointF( 0, 1 ) );
        leftDepth.setX( p.x() );
        childDepth = qMax( childDepth, p.y() );
    }

    qreal left = this->itemTreePos[ this->view->model()->index(0, index.column(), index) ].x();
    qreal right = this->itemTreePos[ this->view->model()->index( rows - 1, index.column(), index ) ].x();

    if ( rows >= 2 )
    {
        if ( rows % 2 == 1 )
        {
            qreal r = qFloor( rows/2 ) + 1;
            qreal v = this->itemTreePos[ this->view->model()->index( r - 1, index.column(), index ) ].x();
            this->setX( index, v );
        }
        else
        {
            this->setX( index, (right + left) / 2 );
        }
    }
    else
    {
        this->setX( index, left );
    }
    this->setY(index, leftDepth.y());

    return QPointF( right + 1, childDepth + 1 );
}

void TreeLayoutPrivate::setX( const QModelIndex& index, qreal x )
{
    if ( !this->itemTreePos.contains( index ) )
    {
        this->itemTreePos.insert( index, QPointF() );
    }
    this->itemTreePos[ index ].setX( x );
}

void TreeLayoutPrivate::setY( const QModelIndex& index, qreal y )
{
    if ( !this->itemTreePos.contains( index ) )
    {
        this->itemTreePos.insert( index, QPointF() );
    }

    this->itemTreePos[ index ].setY( y );
}

void TreeLayoutPrivate::calculateItemRect( const QSize& size )
{
    this->size = size;
    QSize s( size + QSize( this->margins.left() + this->margins.right(),
                           this->margins.bottom() + this->margins.top()
                         )
           );

    this->rect = QRect( -s.width() / 2,
                        -s.height() / 2,
                        s.width(),
                        s.height()
                      );

    this->calculateViewPositions();
}

void TreeLayoutPrivate::calculateItemRect( const QMargins& margins )
{
    this->margins = margins;
    this->calculateViewPositions();
}

QRect TreeLayoutPrivate::itemRect() const
{
    return this->rect;
}

QRect TreeLayoutPrivate::itemRect( const QModelIndex& index ) const
{
    QPoint offset = this->itemPos[index].toPoint() + QPoint( this->margins.left(), this->margins.top() );
    return this->rect.translated( offset );
}

}