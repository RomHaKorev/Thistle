#ifndef THISTLE_SERIEFORMAT_P_H
#define THISTLE_SERIEFORMAT_P_H
#include <QBrush>
#include <QPen>
#include "../kernel/global.h"

namespace Thistle
{
struct SerieFormatPrivate
{
    QBrush brush;
    QPen pen;
    Thistle::Shape shape;
    Thistle::Types type;
};
}

#endif // THISTLE_SERIEFORMAT_P_H