#ifndef THISTLE_PIECHART3DWIDGET_P_H
#define THISTLE_PIECHART3DWIDGET_P_H

#include <QtCore/QRect>
#include <QLabel>

#include "../piechart3d.h"
#include "../pielegendview.h"


namespace Thistle
{

class LinearLegendView;
class PieChart3DWidget;

struct PieChart3DWidgetPrivate
{
    PieChart3DWidget* parent;
    PieChart3D chart;
    PieLegendView legendView;
	QLabel title;

    PieChart3DWidgetPrivate( PieChart3DWidget* parent );
    ~PieChart3DWidgetPrivate();

	void resize( const QRect& rect );

	void setModel( QAbstractItemModel* model );
	QAbstractItemModel* model() const;
};

}

#endif // THISTLE_PIECHART3DWIDGET_P_H
