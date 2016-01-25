#include "abstractitemview_p.h"
#include "../abstractitemview.h"

namespace Thistle
{

AbstractItemViewPrivate::AbstractItemViewPrivate( AbstractItemView* parent ):
	q_ptr( parent ),
	margins( 20, 20, 20, 20 )
{
	Q_UNUSED( parent )
	delegate = new ItemDelegate();
	//rubberBand = new QRubberBand( QRubberBand::Rectangle, parent );
}


AbstractItemViewPrivate::~AbstractItemViewPrivate()
{
	delegate->deleteLater();
	//rubberBand->deleteLater();
}

QModelIndex AbstractItemViewPrivate::findItemAt( const QPointF& pos ) const
{
	Q_FOREACH( QModelIndex index, this->itemPos.keys() )
	{
		QPainterPath path = q_ptr->itemPath( index );
		if ( path.contains( pos ) )
		{
			return index;
		}
	}
	return QModelIndex();
}

}
