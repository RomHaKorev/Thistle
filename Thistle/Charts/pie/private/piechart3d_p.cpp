#include "piechart3d_p.h"

#include "../piechart.h"
#include "../piechart3d.h"
#include <QDebug>

namespace Thistle
{

PieChart3DPrivate::PieChart3DPrivate( PieChart* q ) : PieChartPrivate( true, q )
{}

void PieChart3DPrivate::createRects( const QRect& area )
{
	Q_UNUSED( area )
	if ( q_ptr->model() == 0 )
	{
		return;
	}

	this->height = this->rect.height() * 0.20;
	this->rect.setHeight( this->rect.height() * 0.75 );
	this->angles.clear();
	qreal angle = this->startAngle;
	for ( int i = 0; i < q_ptr->model()->rowCount(); ++i )
	{
		this->angles << angle;
		QModelIndex index = q_ptr->model()->index( i, 0 );
		qreal v = qAbs( q_ptr->model()->data( index ).toReal() );
		qreal delta = 360.0 * v/this->total;
		this->angles << delta;
		angle += delta;
	}
	this->angles << 360.0;
}

}
