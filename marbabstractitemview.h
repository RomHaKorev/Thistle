#ifndef MARBABSTRACTITEMVIEW_H
#define MARBABSTRACTITEMVIEW_H

#include <QAbstractItemView>
#include <QPen>
#include <QPointer>

class MarbItemDelegate;

class MarbAbstractItemView : public QAbstractItemView {
  Q_OBJECT
protected:
  QPointer<MarbItemDelegate> myDelegate;

  virtual void updateValues() = 0;

  virtual void setScrollBarValues() = 0;
  QModelIndex moveCursor( QAbstractItemView::CursorAction cursorAction,
                          Qt::KeyboardModifiers modifiers );

public:
  explicit MarbAbstractItemView( QWidget* parent = 0 );

  int         horizontalOffset() const;
  int         verticalOffset() const;

  bool        isIndexHidden( const QModelIndex& index ) const;
  void        setSelection( const QRect&, QItemSelectionModel::SelectionFlags command );
  void        scrollTo( const QModelIndex& index, ScrollHint hint = EnsureVisible );
  QRect       visualRect( const QModelIndex& index ) const;
  QRegion     visualRegionForSelection( const QItemSelection& selection ) const;


  virtual QRectF itemRect( const QModelIndex& index ) const;

  int rows( const QModelIndex& index ) const;

  virtual void setModel( QAbstractItemModel* model );

protected slots:
  void dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);
  void rowsAboutToBeRemoved(const QModelIndex& parent, int start, int end);
  void rowsInserted(const QModelIndex &parent, int start, int end);


public slots:
  virtual bool save( QString filename ) = 0;
};

#endif // MARBABSTRACTITEMVIEW_H
