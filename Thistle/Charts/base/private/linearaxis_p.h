#ifndef THISTLE_LINEARAXIS_P_H
#define THISTLE_LINEARAXIS_P_H

#include <QtCore>

#include "../../base/private/abstractaxis_p.h"

namespace Thistle
{
struct LinearAxisPrivate: public AbstractAxisPrivate
{
	QLineF line;
	LinearAxisPrivate();
};

}
#endif // THISTLE_LINEARAXIS_P_H
