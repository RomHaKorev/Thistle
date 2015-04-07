#include "linearaxisdelegate_p.h"

namespace Thistle
{

LinearAxisDelegatePrivate::LinearAxisDelegatePrivate( AbstractCoordinateSystemView* aParent )
	:parent(aParent), base(QColor(20, 20, 20)), tick(QColor(150,150,150))
{}

LinearAxisDelegatePrivate::~LinearAxisDelegatePrivate()
{}

}
