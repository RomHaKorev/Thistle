#include "abstracttree_p.h"

namespace Thistle {

AbstractTreePrivate::AbstractTreePrivate() : AbstractItemViewPrivate() {
    realSize = QSize( 200, 200);
    itemRect = QRect( -40, -20, 80, 40 );
    yDistance = 50;
    xDistance = 20;
    depth = 0;
    left = 0;
    connectionPen = QPen( QColor( Global::LightGray ), 2 );
    delegate = new ItemDelegate();
}

}