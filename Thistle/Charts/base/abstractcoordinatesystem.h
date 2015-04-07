#ifndef THISTLE_COORDINATESYSTEM_H
#define THISTLE_COORDINATESYSTEM_H

#include <QPainter>
#include <QObject>

#include "private/abstractcoordinatesystem_p.h"

class QAbstractItemModel;

namespace Thistle
{

class AbstractCoordinateSystem
{
protected:
    AbstractCoordinateSystemPrivate* d_ptr;
    AbstractCoordinateSystem( AbstractCoordinateSystemPrivate* d );
public:
    AbstractCoordinateSystem();

	const QRectF& rect() const;
    virtual void setRect( const QRectF& r );

    QAbstractItemModel* model() const;
    void setModel( QAbstractItemModel* m );

    virtual QPointF origin() const = 0;
    virtual qreal stepSize() const = 0;

    int precision( int axisIndex ) const;
    void setPrecision( int axisIndex, int precision );

	AbstractAxis& axis( unsigned int index ) const;
	unsigned int axisCount() const;

	virtual void update( QAbstractItemModel* model = 0 ) = 0;
};

}

#endif // THISTLE_COORDINATESYSTEM_H
