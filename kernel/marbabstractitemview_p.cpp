#include "marbabstractitemview_p.h"

#include "marbabstractitemview.h"

MarbAbstractItemViewPrivate::MarbAbstractItemViewPrivate( MarbAbstractItemView* parent ): p(parent),
    rubberBand( QRubberBand::Rectangle, parent ), margins( 20, 20, 20, 20 ) {
        delegate = new MarbItemDelegate( parent );
}
