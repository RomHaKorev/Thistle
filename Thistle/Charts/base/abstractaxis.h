#ifndef THISTLE_ABSTRACTAXIS_H
#define THISTLE_ABSTRACTAXIS_H

#include <QtCore>
#include "private/abstractaxis_p.h"

namespace Thistle
{
class AbstractAxisPrivate;

struct AbstractAxis
{
	AbstractAxisPrivate* d_ptr;

	AbstractAxis();
	AbstractAxis( AbstractAxisPrivate* d );
	virtual ~AbstractAxis();

	void setBounds( qreal min, qreal max );
	qreal minimum() const;
	qreal maximum() const;
	int precision() const;
	qreal tickIncrement() const;
	int ticksCount() const;
	qreal labelsLength() const;
	double order() const;
	QString name() const;
	inline virtual qreal length() const
	{
		return 0;
	}

	void setPrecision( int precision );
	void setTicksCount( int count );
	void setLabelsLength( int length );
	void setTickIncrement( qreal size );
	void setOrder( double order );
	void setName( const QString& name );

	virtual QPointF pinpoint( qreal value ) const = 0;

	virtual QPointF origin() const = 0;
};

}
#endif // THISTLE_ABSTRACTAXIS_H
