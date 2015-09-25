#ifndef ITEMDELEGATE_P_H
#define ITEMDELEGATE_P_H

#include "../itemdelegate.h"
#include "../global.h"
namespace Thistle
{

struct ItemDelegatePrivate
{
	ItemStyle style;

	ItemDelegatePrivate();
	virtual ~ItemDelegatePrivate();
};

}
#endif // ITEMDELEGATE_P_H
