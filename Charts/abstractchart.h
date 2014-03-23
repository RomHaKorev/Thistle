#ifndef ABSTRACTCHART_H
#define ABSTRACTCHART_H

#include "../kernel/marbabstractitemview.h"

#include <QWidget>

class AbstractChart :  public MarbAbstractItemView {
protected:
  QRect myLegendRect;
  QString myTitle;
  QRect myTitleRect;
  qreal myMin;
  qreal myMax;
  int myMinBottomMargin;

  virtual void process();
  virtual void defineRects() = 0;
  virtual int scan() = 0;
  void calculateLegendRect( const QRect& source );

  virtual void setScrollBarValues();
  virtual QPainterPath itemPath(const QModelIndex& index) const;
  virtual void updateRects() = 0;

  virtual void resizeEvent( QResizeEvent* ev );
  virtual void paintEvent(QPaintEvent* event);
  virtual void paintLegend( QPainter& painter ) const;
  virtual void paintSerieLegend( QPainter& painter, int serie, QPoint pos, int maxHeight ) const = 0;
  virtual void paintChart( QPainter& painter ) const = 0;
public:
    AbstractChart( QWidget* parent = 0 );
    ~AbstractChart();

    virtual QModelIndex indexAt(const QPoint& point) const;

    void updateValues();
    void setTitle( const QString& title );
    bool save( QString filename );
};

#endif //ABSTRACTCHART_H
