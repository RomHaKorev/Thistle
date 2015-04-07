#ifndef THISTLE_LINEARLEGENDVIEW_P_H
#define THISTLE_LINEARLEGENDVIEW_P_H

#include <QPainter>
#include <QRect>

#include "../../base/private/abstractlegendview_p.h"

#include <QVector>

namespace Thistle
{

class LinearLegendView;
class LinearChart;
class DotDelegate;

struct LinearLegendViewPrivate : public AbstractLegendViewPrivate
{
    DotDelegate* dotDelegate;

    LinearLegendViewPrivate( LinearLegendView* parent, LinearChart* associatedChart );
    ~LinearLegendViewPrivate();

    void calculateSizeHint( const QSize& source );
};

}

#endif // THISTLE_LINEARLEGENDVIEW_P_H
