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