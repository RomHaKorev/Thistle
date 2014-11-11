#include "treelayout.h"
#include "treelayout_p.h"

#include <QModelIndex>
#include <QAbstractItemModel>
#include <QRect>
#include <QDebug>

#include "treeview.h"

namespace Thistle
{

TreeLayout::TreeLayout( TreeView* parent )
    :QObject( parent ), d_ptr( new TreeLayoutPrivate() )
{
    Q_D( TreeLayout );
    d->view = parent;
}

QRect TreeLayout::boundingRect() const
{
    return d_ptr->boundingRect;
}

TreeLayout::TreeLayout( TreeLayoutPrivate* d, TreeView* parent )
    :QObject( parent ), d_ptr( d )
{
    d->view = parent;
}

TreeLayout::~TreeLayout(void)
{}

void TreeLayout::update()
{
    d_ptr->calculateTreePositions();
    d_ptr->calculateViewPositions();
}


QList<QModelIndex> TreeLayout::validIndexes() const
{
    return d_ptr->itemPos.keys();
}


void TreeLayout::setView( TreeView* parent )
{
    d_ptr->view = parent;
}


TreeView* TreeLayout::view() const
{
    return d_ptr->view;
}

QModelIndex TreeLayout::indexAt( const QPoint& p ) const
{
    Q_FOREACH( QModelIndex id, d_ptr->itemPos.keys() )
    {
        QRectF r = this->itemRect( id );
        if ( r.contains( p ) )
        {
            return id;
        }
    }
    return QModelIndex();
}

void TreeLayout::setItemSize( const QSize& s )
{
    d_ptr->calculateItemRect( s );
}

void TreeLayout::setSpacing( const QMargins& margins )
{
    d_ptr->calculateItemRect( margins );
}


QRect TreeLayout::itemRect( const QModelIndex& index ) const
{
    if ( !index.isValid() )
        return d_ptr->itemRect();

    return d_ptr->itemRect( index );
}

void TreeLayout::setOrientation( Qt::Orientation orientation )
{
    d_ptr->orientation = orientation;
    d_ptr->calculateViewPositions();
}

}


