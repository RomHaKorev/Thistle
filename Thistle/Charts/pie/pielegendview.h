#ifndef THISTLE_PIELEGENDVIEW_H
#define THISTLE_PIELEGENDVIEW_H

#include "../base/abstractlegendview.h"

namespace Thistle
{
class PieChart;
class PieLegendViewPrivate;

class PieLegendView : public AbstractLegendView
{
	Q_OBJECT
	Q_DECLARE_PRIVATE( PieLegendView )
protected:
	virtual void paintSeriePicto( QPainter &painter, const QRect& rect, int serie ) const;
	inline virtual int indexToSerie( const QModelIndex& index ) const;
	inline virtual QModelIndex serieToIndex( int serie ) const;
public:
	PieLegendView( QWidget* parent = 0 );
	virtual ~PieLegendView();
	virtual unsigned int serieCount() const;
	virtual QString serieName( unsigned int serieIdx ) const;
	void setAssociatedPieChart( QPointer<PieChart> chart );
	QPointer<PieChart> setAssociatedPieChart() const;
	virtual QModelIndex indexAt( const QPoint& point ) const;
};
}

#endif // THISTLE_PIELEGENDVIEW_H
