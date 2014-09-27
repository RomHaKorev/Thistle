#include "abstractitemview_p.h"
#include "abstractitemview.h"

namespace Thistle
{

AbstractItemViewPrivate::AbstractItemViewPrivate( AbstractItemView* parent ): margins( 20, 20, 20, 20 )
{
    delegate = new ItemDelegate();
    rubberBand = new QRubberBand( QRubberBand::Rectangle, parent );
}


AbstractItemViewPrivate::~AbstractItemViewPrivate()
{
    delegate->deleteLater();
    rubberBand->deleteLater();
}

}