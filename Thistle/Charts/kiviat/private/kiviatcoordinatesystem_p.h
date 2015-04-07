#ifndef THISTLE_KIVIATCOORDINATESYSTEM_P_H
#define THISTLE_KIVIATCOORDINATESYSTEM_P_H

#include "../../base/private/abstractcoordinatesystem_p.h"
#include "../../base/linearaxis.h"

namespace Thistle
{
class KiviatCoordinateSystemPrivate: public AbstractCoordinateSystemPrivate
{
private:
	QList<LinearAxis> axisList;
public:
	KiviatCoordinateSystemPrivate();
	~KiviatCoordinateSystemPrivate();

	virtual AbstractAxis& axis( unsigned int index );
	virtual unsigned int axisCount() const;
	
	void update( QAbstractItemModel* model, const QRectF& chartRect );
};

}
#endif // THISTLE_KIVIATCOORDINATESYSTEM_P_H
