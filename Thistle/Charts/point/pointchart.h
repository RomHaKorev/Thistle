#ifndef THISTLE_POINTCHART_H
#define THISTLE_POINTCHART_H

#include "../linear/linearchart.h"
#include "private/pointchart_p.h"

namespace Thistle
{

class PointChart: public LinearChart
{
    Q_DECLARE_PRIVATE( PointChart );
protected:
    virtual QRectF itemRect( const QModelIndex& index ) const;

public:
    PointChart( QWidget* parent = 0 );
    ~PointChart();
};

}
#endif // THISTLE_POINTCHART_H
