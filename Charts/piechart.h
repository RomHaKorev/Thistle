#ifndef PIECHART_H
#define PIECHART_H

#include <QAbstractItemView>

class PieChart : public QAbstractItemView {
  Q_OBJECT

public:
  explicit PieChart(QWidget *parent = 0);

    QRect visualRect(const QModelIndex &index) const;
    void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);
    QModelIndex indexAt(const QPoint &point) const;

protected slots:
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void rowsInserted(const QModelIndex &parent, int start, int end);
    void rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);

protected:
    QRect myRect;
    qreal myTotal;

    void updateChart();

    QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction,
                           Qt::KeyboardModifiers modifiers);

    int horizontalOffset() const;
    int verticalOffset() const;

    bool isIndexHidden(const QModelIndex &index) const;

    void paintEvent(QPaintEvent *event);

    QRegion visualRegionForSelection(const QItemSelection &selection) const;
    void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command);
  
};

#endif // PIECHART_H
