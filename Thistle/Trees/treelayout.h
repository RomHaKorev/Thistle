#ifndef TREELAYOUT_H
#define TREELAYOUT_H

#include <QObject>
#include <QList>
#include <QModelIndex>
#include <QMargins>

#include <QPolygonF>

namespace Thistle
{
class TreeView;
class TreeLayoutPrivate;
class TreeLayout : public QObject
{
	Q_OBJECT
	Q_DECLARE_PRIVATE( TreeLayout );
protected:
	TreeLayoutPrivate* d_ptr;
	TreeLayout( TreeLayoutPrivate* d, TreeView* parent = 0 );
public:
	TreeLayout( TreeView* parent = 0 );
	virtual ~TreeLayout();

	void setView( TreeView* parent );
	TreeView* view() const;

	virtual void update();

	QList<QModelIndex> validIndexes() const;

	QModelIndex indexAt( const QPoint& p ) const;

	void setItemSize( const QSize& s );
	void setSpacing( const QMargins& margins );

	QRect itemRect( const QModelIndex& index = QModelIndex() ) const;

	/* Returns the rect needed to display all items */
	QRect boundingRect() const;

	void setOrientation( Qt::Orientation orientation );

};
}

#endif //TREELAYOUT_H
