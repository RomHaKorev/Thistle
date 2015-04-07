#include "abstractcoordinatesystemview_p.h"
#include "../abstractcoordinatesystem.h"
#include "../../../kernel/global.h"
#include "../linearaxisdelegate.h"

#include <QAbstractItemModel>

namespace Thistle
{

AbstractCoordinateSystemViewPrivate::AbstractCoordinateSystemViewPrivate( AbstractCoordinateSystem* coordSys, AbstractCoordinateSystemView* coordSysView )
  : tickPen(Qt::lightGray, 1), model(0), delegate( new LinearAxisDelegate( coordSysView ) ), coordSystem(coordSys)
{}


AbstractCoordinateSystemViewPrivate::~AbstractCoordinateSystemViewPrivate()
{
  if ( delegate )
    delete delegate;
}


AbstractCoordinateSystem* AbstractCoordinateSystemViewPrivate::coordinateSystem() const
{
  return coordSystem;
}

}
