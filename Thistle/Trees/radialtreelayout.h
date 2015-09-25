#ifndef RADIALTREELAYOUT_H
#define RADIALTREELAYOUT_H

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

#endif // RADIALTREELAYOUT_H
