#ifndef THISTLE_XYCHART_H
#define THISTLE_XYCHART_H

#include "linearchart.h"
#include "xychart_p.h"

namespace Thistle
{

class XYChart: public LinearChart
{
    Q_OBJECT
    Q_DECLARE_PRIVATE( XYChart );
protected:
    //virtual void paintChart( QPainter& );
    //virtual void paintSerie( QPainter& painter, int column );
    
    //virtual void updateRects();
public:
    XYChart(QWidget *parent = 0);
    ~XYChart();
};

}
#endif // THISTLE_XYCHART_H
