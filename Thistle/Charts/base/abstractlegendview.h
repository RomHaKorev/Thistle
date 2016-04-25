#ifndef THISTLE_ABSTRACTLEGENDVIEW_H
#define THISTLE_ABSTRACTLEGENDVIEW_H


#include <QPainter>
#include <QRect>

#include "../../Core/abstractitemview.h"

#include "serieformatproxy.h"

namespace Thistle
{

class AbstractLegendViewPrivate;

class AbstractLegendView : public AbstractItemView
{
	Q_OBJECT
	//Q_DECLARE_PRIVATE( AbstractLegendView );
	inline AbstractLegendViewPrivate* d_func()
	{
		return reinterpret_cast<AbstractLegendViewPrivate *>(qGetPtrHelper(d_ptr));
	}
	inline const AbstractLegendViewPrivate* d_func() const
	{
		return reinterpret_cast<const AbstractLegendViewPrivate *>(qGetPtrHelper(d_ptr));
	}
	friend class AbstractLegendViewPrivate;

private:
	virtual void setScrollBarValues();

	virtual QModelIndex indexAt(const QPoint &point) const = 0;
	virtual void setSelection( const QRect& rect, QItemSelectionModel::SelectionFlags command );
	virtual QPainterPath itemPath( const QModelIndex& index ) const;
	virtual void paintSerie( QPainter &painter, int serie ) const;
	virtual void paintSeriePicto( QPainter &painter, const QRect& rect, int serie ) const = 0;
	virtual void paintEvent( QPaintEvent* ev );
	bool isActiveSerie( int serieIdx, bool& hasSelecion ) const;

protected:
	AbstractLegendView( AbstractLegendViewPrivate* d, QWidget* parent = 0 );
	inline virtual int indexToSerie( const QModelIndex& index ) const = 0;
	inline virtual QModelIndex serieToIndex( int serie ) const = 0;
public:
	virtual ~AbstractLegendView();
	void updateSizeHint( const QSize& source );
	virtual QSize sizeHint() const;
	virtual unsigned int serieCount() const = 0;
	virtual QString serieName( unsigned int serie ) const = 0;
	virtual void updateValues()
	{}
	QPointer<Thistle::SerieFormatProxy> serieFormatProxy() const;
	void setSerieFormatProxy( QPointer<Thistle::SerieFormatProxy> );
};

}


#endif // THISTLE_ABSTRACTLEGENDVIEW_H
