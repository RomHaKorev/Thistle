#ifndef THISTLE_SERIECHART_P_H
#define THISTLE_SERIECHART_P_H

#include "abstractchart_p.h"
#include "../abstractcoordinatesystem.h"

#include <QMap>

namespace Thistle
{
class SerieChart;
class AbstractCoordinateSystemView;
class DotDelegate;
class BarDelegate;
class AxisView;
class AbstractChartDelegate;
class AbstractCoordinatePlaneView;


class SerieChartPrivate : public AbstractChartPrivate
{
protected:
  QPair<QPointF, QPointF> controlPoints( const QPointF& p0, const QPointF& p1, const QPointF& p2, qreal t = 0.25 ) const;
    int x;
  AbstractChartDelegate* currentDelegate;
  AbstractCoordinateSystemView* coordinateSysView;

  SerieChart* q_func() const
  {
    return reinterpret_cast<SerieChart*>( q_ptr );
  }

public:
  DotDelegate* pointDelegate;
  BarDelegate* barDelegate;

    SerieChartPrivate( SerieChart* q, AbstractCoordinateSystemView* coordSysView );
  virtual ~SerieChartPrivate();

  AbstractCoordinateSystemView* coordinateSystemView() const;
  void setCoordinateSystemView( AbstractCoordinateSystemView* );

  void selectDelegate( Thistle::Types type );
  inline AbstractChartDelegate* delegate() const
  {
    return currentDelegate;
  }

  void paintDelegate( QPainter& painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
  void paint( QPainter& painter, int column, Thistle::Types types, bool active ) const;
  virtual void paintStraightLine( QPainter& painter, int column, bool isActive, bool area = false ) const;
  virtual void paintSpline( QPainter& painter, int column, bool isActive, bool area = false ) const;
  virtual void paintRaw( QPainter& painter, int column, bool isActive ) const;


    void readModel();
};

}

#endif // THISTLE_SERIECHART_P_H
