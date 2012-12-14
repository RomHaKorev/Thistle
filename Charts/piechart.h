#ifndef PIECHART_H
#define PIECHART_H

#include <QAbstractItemView>

#include "chart.h"

class PieChart : public QAbstractItemView {
  Q_OBJECT
protected:

  int          my_col;
  QColor       my_color;
  bool         my_manualBounds;
  qreal        my_max;
  qreal        my_min;
  Qt::PenStyle my_penStyle;
  QPolygonF    my_points;

  void        updateChart();

  int         rows(const QModelIndex &index = QModelIndex()) const;

  void        scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);

  bool        isIndexHidden(const QModelIndex &index) const;

  int         horizontalOffset() const;
  int         verticalOffset() const;

  QRect       visualRect(const QModelIndex &index) const;
  QRegion     visualRegionForSelection(const QItemSelection &selection) const;

public:
  explicit PieChart( QWidget* parent = 0 );
  ~PieChart();
  void   setBounds( qreal min, qreal max );
  void   setManualbounds( bool manual = true );

  QColor Color() const;
  void   setColor( QColor c );
  void   setColor( Chart::PredefinedColor c );

  int  columnData();
  void setColumnData( int c );

  QModelIndex indexAt(const QPoint &point) const;
  QRect       itemRect(const QModelIndex &index) const;
  void        setSelection(const QRect&, QItemSelectionModel::SelectionFlags command);

  QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction,
                         Qt::KeyboardModifiers modifiers);

  virtual void paintChart( QPainter* painter );

protected slots:
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);
    void rowsInserted(const QModelIndex &parent, int start, int end);
};

#endif // PIECHART_H
