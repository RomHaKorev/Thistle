#ifndef THISTLE_COORDINATESYSTEMVIEWPRIVATE_P_H
#define THISTLE_COORDINATESYSTEMVIEWPRIVATE_P_H

#include <QPen>
#include <QFont>

class QAbstractItemModel;

namespace Thistle
{
class AbstractCoordinateSystem;
class LinearAxisDelegate;
class AbstractCoordinateSystemView;

struct AbstractCoordinateSystemViewPrivate
{
  AbstractCoordinateSystemViewPrivate( AbstractCoordinateSystem* coordSystem, AbstractCoordinateSystemView* coordSysView );
  virtual ~AbstractCoordinateSystemViewPrivate();
  virtual AbstractCoordinateSystem* coordinateSystem() const;

  QRect rect;
  QFont font;
  QPen axisPen;
  QPen tickPen;
  QPen textPen;
  QAbstractItemModel* model;
  LinearAxisDelegate* delegate;

protected:
  AbstractCoordinateSystem* coordSystem;
};

}

#endif // THISTLE_COORDINATESYSTEMVIEWPRIVATE_P_H
