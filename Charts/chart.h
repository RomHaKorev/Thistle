/*
 This file is part of Marb.

  Marb is free software: you can redistribute it and/or modify
  it under the terms of the Lesser GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License.

  Marb is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  Lesser GNU General Public License for more details.

  You should have received a copy of the Lesser GNU General Public License
  along with Marb.  If not, see <http://www.gnu.org/licenses/>.

 Marb  Copyright (C) 2013  Dimitry Ernot
*/

#ifndef CHART_H
#define CHART_H

#include <QAbstractItemView>
#include "../Marb.h"
#include "chartstyle.h"

#include "../marbabstractitemview.h"

class PointChart;
class RadialChart;

class Chart : public MarbAbstractItemView {
  Q_OBJECT

protected:
  QPointF myOrigin;
  qreal myMin;
  qreal myMax;
  qreal myMinBound;
  qreal myMaxBound;
  qreal myOrder;
  int myNbTicks;
  qreal myTickSize;
  qreal myAlpha;
  qreal myBeta;
  QRect myChartRect;
  QRect myValuesRect;
  QRect myLegendRect;
  QString myTitle;
  QRect myTitleRect;
  QMap<int, ChartStyle> myStyle;

  int myX;
  int myNbDigits;
  int myMarginX;
  int myMarginY;
  int myMinBottomMargin;

  virtual void process();
  virtual void updateValues() = 0;
  int scanValues();
  void calculateBounds();
  void calculateLegendRect();
  qreal calculateOrder( qreal value ) const;
  qreal valueToPx( qreal value) const;
  void setAlphaBeta();
  virtual QRectF itemRect( const QModelIndex& index ) const = 0;
  virtual void resizeEvent( QResizeEvent* ev );
  virtual void paintLegend( QPainter& painter );
  virtual void paintColumnLegend( QPainter& painter, int column, QPoint pos, int maxHeight ) = 0;
  virtual void paintChart( QPainter& ) = 0;
  virtual void paintEvent(QPaintEvent *event);

  virtual void setScrollBarValues();
  virtual QModelIndex indexAt(const QPoint &point) const;

public:
  explicit Chart( QWidget* parent = 0 );
           ~Chart();
  void     setModel( QAbstractItemModel* model );

  void setTitle( QString title );
  void setColumnStyle( int column, ChartStyle style);
  ChartStyle columnStyle( int column ) const;
  bool save( QString filename );
};

#endif // CHART_H
