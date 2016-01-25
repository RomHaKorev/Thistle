#ifndef THISTLE_SCATTERCHART_H
#define THISTLE_SCATTERCHART_H

#include "../line/linechart.h"
#include "private/scatterchart_p.h"

namespace Thistle
{

class ScatterChart: public LineChart
{
	Q_DECLARE_PRIVATE( ScatterChart );
protected:
	virtual QPainterPath itemPath( const QModelIndex& index ) const;

public:
	ScatterChart( QWidget* parent = 0 );
	~ScatterChart();

	SerieFormat serieFormat( int column ) const;
};

}
#endif // THISTLE_SCATTERCHART_H
