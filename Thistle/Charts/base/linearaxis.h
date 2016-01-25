#ifndef THISTLE_LINEARAXIS_H
#define THISTLE_LINEARAXIS_H

#include "../base/abstractaxis.h"

namespace Thistle
{
class LinearAxisPrivate;

struct LinearAxis: public AbstractAxis
{
	Q_DECLARE_PRIVATE( LinearAxis );
	LinearAxis();
	virtual ~LinearAxis();

	void setLine( const QLineF& line );
	const QLineF& line() const;

	virtual QPointF pinpoint( qreal value ) const;
	virtual QPointF origin() const;

	inline virtual qreal length() const
	{
		return line().length();
	}
};

}
#endif // THISTLE_LINEARAXIS_H
