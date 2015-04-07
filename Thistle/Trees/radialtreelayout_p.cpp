#include "radialtreelayout_p.h"
#include "treeview.h"
#include <qmath.h>
#include <QAbstractItemModel>

namespace Thistle
{

RadialTreeLayoutPrivate::RadialTreeLayoutPrivate()
  : TreeLayoutPrivate(),
    diagonal(1),
    perimeter(0),
    radius(10),
    maxRad(0),
    currentCenter( QPointF() )
{
  this->rect = QRect( -20, -20, 40, 40 );
  this->margins = QMargins( 20, 20, 20, 20 );
}



void RadialTreeLayoutPrivate::calculateTreePositions()
{
    this->itemTreePos.clear();
    this->orderedIndexes.clear();
    QPointF p = this->scan( this->view->model()->index(0,0) , QPointF(0,0) );
    this->left = p.x() + 1;
    this->depth = p.y();

    this->updatePerimeter();

}

void RadialTreeLayoutPrivate::calculateViewPositions()
{
    QAbstractItemModel* model = this->view->model();

    if ( model == 0 )
        return;

    QPolygonF polygon;

    if ( this->itemTreePos.isEmpty() || (this->radius == HUGE_VAL) || (this->radius == 0) )
        return;

    if ( (this->radius == HUGE_VAL) || (this->radius == 0) )
        return;

    this->size.setWidth( this->radius * 2 + this->itemRect().width());
    this->size.setHeight( this->radius * 2 + this->itemRect().height() );

    this->itemOffset = QPoint( 0, 0 );

    QPointF offset( this->size.width() / 2, this->size.height() / 2 );

    this->itemPos = this->itemTreePos;
    QPainterPath path;
    path.addEllipse( offset + this->itemRect().center(), this->radius, this->radius );
    qreal l = 0;
    qreal factor = 0;
    Q_FOREACH( QModelIndex index, this->orderedIndexes )
    {
        if ( model->rowCount( index ) != 0 )
        {
            continue;
        }
        factor = qreal(this->depth) / qreal(this->itemPos[index].y());
        if ( model->rowCount( index ) == 0 )
        {
            l +=    (qreal(this->diagonal)/2.0) * qreal(factor);
        }
        qreal percent = path.percentAtLength( l );
        this->itemPos[index] = QPointF( percent, this->itemPos[index].y() );
        if ( model->rowCount( index ) == 0 )
        {
            l +=    (qreal(this->diagonal)/2.0) * qreal(factor);
        }
    }
    QList<QModelIndex> reversedOrderedIndexes;

    Q_FOREACH ( QModelIndex idx, this->orderedIndexes )
    {
        reversedOrderedIndexes.prepend( idx );
    }

    Q_FOREACH ( QModelIndex index, reversedOrderedIndexes )
    {
        if ( model->rowCount( index ) == 0 )
        {
            continue;
        }
        QModelIndex left = model->index( 0,0, index );
        QModelIndex right = model->index( model->rowCount(index) - 1,0, index );
        this->itemPos[index] = QPointF( float(this->itemPos[left].x() + this->itemPos[right].x()) / 2.0, this->itemPos[index].y() );
    }
    this->itemPos[this->view->model()->index(0,0)] = QPointF(0, 0);

    Q_FOREACH ( QModelIndex index, this->orderedIndexes )
    {
        if ( this->depth == this->itemPos[index].y() )
        {
            factor = 1.0;
        }
        else
        {
            if ( this->itemPos[index].y() != 0 )
            {
                factor = qreal(this->depth) / qreal(this->itemPos[index].y());
            }
            else
            {
                factor = 1.0;
            }
        }
        qreal radius = qreal( this->radius ) / qreal(factor);
        QPainterPath circle;
        circle.addEllipse( this->itemRect().center(), radius, radius );
        this->itemPos[index] = circle.pointAtPercent( this->itemPos[index].x() ) + offset;
        if ( index != model->index( 0, 0 ) ) polygon << this->itemPos[index];
    }

    this->itemPos[ model->index(0,0) ] = offset;

    polygon.append( offset );

    this->boundingRect = polygon.boundingRect().toRect();
    this->boundingRect.translate( -this->itemRect().width() / 2, -this->itemRect().height() / 2 );
    this->boundingRect.setWidth( this->itemRect().width() + this->margins.left() + this->margins.right() + this->boundingRect.width() );
    this->boundingRect.setHeight( this->itemRect().height() + this->margins.top() + this->margins.bottom() + this->boundingRect.height() );

    this->itemOffset = -this->boundingRect.topLeft();

    this->boundingRect.translate( -this->boundingRect.topLeft() );

}

QPointF RadialTreeLayoutPrivate::scan( const QModelIndex& index, const QPointF& ld)
{
    QPointF leftDepth(ld);
    this->orderedIndexes.append( index );
    int rows = this->view->model()->rowCount( index );
    if ( !index.isValid() )
    {
        return QPointF();
    }
    else if ( rows == 0 )
    {
        this->setX( index, leftDepth.x() );
        this->setY(index, leftDepth.y() );
        return QPointF(leftDepth.x() + 1, 1);
    }
    qreal childDepth = 0;
    for( int r = 0; r < rows; ++r )
    {
        QModelIndex child = index.child( r, 0 );
        QPointF p = scan( child, leftDepth + QPointF( 0, 1 ) );
        leftDepth.setX( p.x() );
        childDepth = qMax( childDepth, p.y() );
    }
    qreal left = this->itemTreePos[ this->view->model()->index(0, 0, index) ].x();
    qreal right = this->itemTreePos[ this->view->model()->index( rows - 1, 0, index ) ].x();
    if ( rows >= 2 )
    {
        if ( rows % 2 == 1 )
        {
            qreal r = qFloor( rows/2 ) + 1;
            qreal v = this->itemTreePos[ this->view->model()->index( r - 1, 0, index ) ].x();
            this->setX( index, v );
        }
        else
        {
            this->setX( index, ( right + left + 1 ) / 2.0 );
        }
    }
    else
    {
        this->setX( index, left );
    }
    this->setY(index, leftDepth.y());
    return QPointF( right + 1 , childDepth + 1 );
}

void RadialTreeLayoutPrivate::updatePerimeter()
{
    qreal w = this->itemRect().width() + this->margins.left() + this->margins.right();
    qreal h = this->itemRect().height() + this->margins.top() + this->margins.bottom();
    this->diagonal = qSqrt( w * w + h * h );
    this->perimeter = 0;
    qreal factor = 1;
    Q_FOREACH ( QModelIndex index, this->itemTreePos.keys() )
    {
        if ( this->view->model()->rowCount( index ) == 0 )
        {
            factor = ( this->depth ) / ( this->itemTreePos.value( index ).y() );
            this->perimeter += this->diagonal * factor;
        }
    }

    this->radius = this->perimeter / ( 2 * M_PI );
    if ( this->radius*2 < this->diagonal * this->depth )
    {
        this->radius = ( this->diagonal * this->depth )/2 * 1.5;
    }
}

QRect RadialTreeLayoutPrivate::itemRect( const QModelIndex& index ) const
{
    QPoint offset = this->itemPos[index].toPoint() + QPoint( this->margins.left(), this->margins.top() );
    return this->rect.translated( offset + this->itemOffset.toPoint() );
}

}
