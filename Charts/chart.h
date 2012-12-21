#ifndef CHART_H
#define CHART_H

#include <QAbstractItemView>
#include "chartspec.h"

//#include "point.h"

class PointChart;

class Chart : public QAbstractItemView {
  Q_OBJECT
public:
  enum ChartStyle {
    Point  = 0,
    Line   = 1,
    Bar    = 2,
    Radial = 3
  };

  enum PointShape {
    None     = 0,
    Circle   = 1,
    Square   = 2,
    Triangle = 3,
    Diamond  = 4
  };

  enum PredefinedColor {
    Blue   = 1,
    Green  = 2,
    Red    = 3,
    Purple = 4,
    Orange = 5
  };

/*  Q_ENUMS( PredefinedColor )
  Q_ENUMS( PointShape )
  Q_ENUMS( ChartStyle )*/

protected:
  enum Type {
    TypeLinear = 0,
    TypeRadial = 1
  };

  Type                     myType;
  ChartSpec                mySpec;
  QAbstractItemModel*      myModel;
  QItemSelectionModel*     mySelections;
  QMap< int, PointChart* > myLinearCharts;
  QMap< int, PointChart* > myRadialCharts;

  QPointF     myOrigin;
  int         myTopChart;

  void        resizeEvent(QResizeEvent * ev);

  void        updateChart();

  QModelIndex indexAt(const QPoint &point) const;
  QRect       itemRect(const QModelIndex &index) const;
  void        setSelection(const QRect&, QItemSelectionModel::SelectionFlags command);

  QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction,
                         Qt::KeyboardModifiers modifiers);

  void        paintEvent(QPaintEvent *event);
  void        paintAxis( QPainter* painter );
  void        paintGrid( QPainter* painter );
  void        paintText( QPainter* painter );

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

  void     showData( int column, Chart::ChartStyle style = Chart::Point );
  void     hideData( int column );

protected slots:
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);
    void rowsInserted(const QModelIndex &parent, int start, int end);
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
};

#endif // CHART_H
