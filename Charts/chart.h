#ifndef CHART_H
#define CHART_H

#include <QAbstractItemView>
#include "abstractmarbview.h"
#include "Marb.h"
#include "chartstyle.h"

class PointChart;
class RadialChart;

class Chart : public AbstractMarbView {
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
  virtual void processSpec() = 0;
  int scanValues();
  void calculateBounds();
  void calculateLegendRect();
  qreal calculateOrder( qreal value ) const;
  qreal valueToPx( qreal value) const;
  void setAlphaBeta();
  virtual QRectF itemRect( const QModelIndex& index ) const = 0;
  virtual void resizeEvent( QResizeEvent* ev );
  virtual void paintLegend( QPainter& painter );
  virtual void paintColumnLegend( QPainter& painter, int column, QPoint pos, int maxHeight );
  virtual void paintChart( QPainter& ) = 0;
  virtual void paintEvent(QPaintEvent *event);
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
