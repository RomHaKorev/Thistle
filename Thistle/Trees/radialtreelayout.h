#ifndef THISTLE_RADIALTREELAYOUT_H
#define THISTLE_RADIALTREELAYOUT_H

#include "treelayout.h"
#include "private/radialtreelayout_p.h"

namespace Thistle
{
class TreeView;

class RadialTreeLayout : public TreeLayout
{
	Q_DECLARE_PRIVATE( RadialTreeLayout );
public:
	RadialTreeLayout( TreeView* parent = 0 );
	virtual ~RadialTreeLayout();
	virtual void update();

private:

};
}

#endif // THISTLE_RADIALTREELAYOUT_H
