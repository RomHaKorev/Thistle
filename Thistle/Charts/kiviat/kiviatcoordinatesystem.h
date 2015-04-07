#ifndef KIVIATCOORDINATESYSTEM_H
#define KIVIATCOORDINATESYSTEM_H

#include "../base/abstractcoordinatesystem.h"
#include "private/kiviatcoordinatesystem_p.h"

namespace Thistle {

class KiviatCoordinateSystem : public AbstractCoordinateSystem
{
	Q_DECLARE_PRIVATE( KiviatCoordinateSystem )
protected:
	virtual QPointF origin() const;
	virtual void update( QAbstractItemModel* model = 0 );
	virtual qreal stepSize() const;

	virtual void setRect( const QRectF& r );
public:
	KiviatCoordinateSystem();
	~KiviatCoordinateSystem();

private:
	
};

} // namespace Thistle

#endif // KIVIATCOORDINATESYSTEM_H
