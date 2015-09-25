#ifndef THISTLE_SCATTERCHART_H
#define THISTLE_SCATTERCHART_H

#include "../linear/linearchart.h"
#include "private/scatterchart_p.h"

namespace Thistle
{

class ScatterChart: public LinearChart
{
	Q_DECLARE_PRIVATE( ScatterChart );
protected:
	virtual QPainterPath itemPath( const QModelIndex& index ) const;

public:
	ScatterChart( QWidget* parent = 0 );
	~ScatterChart();
};

}
#endif // THISTLE_SCATTERCHART_H
