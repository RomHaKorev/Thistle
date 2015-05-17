#ifndef THISTLE_PieLegendView_H
#define THISTLE_PieLegendView_H

#include "../base/abstractlegendview.h"

namespace Thistle
{
class PieChart;
class PieLegendView : public AbstractLegendView
{
protected:
    PieChart* chart;
    //virtual void paint( QPainter& painter ) const;
	virtual void paintSerie( QPainter &painter, int serie, const QRect& rect ) const;
public:
	PieLegendView( PieChart* parent );
	~PieLegendView();
private:

};
}

#endif // THISTLE_PieLegendView_H
