#ifndef CHART_H
#define CHART_H

#include <QPointer>
#include <QAbstractItemView>
#include "chartspec.h"
#include "Clint.h"
//#include "point.h"

class PointChart;
class RadialChart;

class Chart : public QAbstractItemView {
  Q_OBJECT

protected:
  ChartSpec                mySpec;
  QAbstractItemModel*      myModel;
  QItemSelectionModel*     mySelections;
  QMap< int, QPointer<PointChart> > myCharts;

  QPointF     myOrigin;
  int         myTopChart;

  void        resizeEvent(QResizeEvent * ev);

  void        updateChart();

  QModelIndex indexAt(const QPoint &point) const;
  QRect       itemRect(const QModelIndex &index) const;
  void        setSelection(const QRect&, QItemSelectionModel::SelectionFlags command);

  QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction,
                         Qt::KeyboardModifiers modifiers);

  void         paintEvent(QPaintEvent *event);
  virtual void paintAxis( QPainter& painter );
  virtual void paintGrid( QPainter& painter );
  virtual void paintText( QPainter& painter );

  int         rows(const QModelIndex &index = QModelIndex()) const;

  void        scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);

  bool        isIndexHidden(const QModelIndex &index) const;

  int         horizontalOffset() const;
  int         verticalOffset() const;

  QRect       visualRect(const QModelIndex &index) const;
  QRegion     visualRegionForSelection(const QItemSelection &selection) const;

public:
  explicit Chart( QWidget* parent = 0 );
           ~Chart();
  void     setModel( QAbstractItemModel* model );
  void     setSelectionModel( QItemSelectionModel* s );

  void     hideData( int column );

protected slots:
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);
    void rowsInserted(const QModelIndex &parent, int start, int end);
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
};

#endif // CHART_H
