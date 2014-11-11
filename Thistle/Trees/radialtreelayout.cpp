#include "radialtreelayout.h"
#include "radialtreelayout_p.h"

#include "treeview.h"

namespace Thistle
{

RadialTreeLayout::RadialTreeLayout(TreeView* parent)
    : TreeLayout( new RadialTreeLayoutPrivate(), parent )
{}

RadialTreeLayout::~RadialTreeLayout()
{}

void RadialTreeLayout::update()
{
    Q_D( RadialTreeLayout );
    d->calculateTreePositions();
    d->updatePerimeter();
    d->calculateViewPositions();
}


}