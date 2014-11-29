#include "axischart_p.h"

#include "axischart.h"
#include "axis/axisview.h"

namespace Thistle
{

AxisChartPrivate::AxisChartPrivate( AxisChart* q ) : AbstractChartPrivate( q ), axisView( new AxisView() )
{}

void AxisChartPrivate::readModel()
{
    /*int rows = this->model()->rowCount();
    int cols = this->model()->columnCount();
    QFontMetrics metrics( this->font() );
    int textWidth = 0;
    int valueWidth = 0;
    qreal value = this->model()->index( 0, 0 ).data().toDouble();
    qreal min = 0;
    qreal max = 0;
    for ( int r = 0; r < rows; ++r )
    {
        QString s( this->model()->headerData( r, Qt::Vertical ).toString() );
        textWidth = qMax( textWidth, metrics.width( s ) + 5 );
        for ( int c = 0; c < cols; ++c )
        {
            value = this->model()->index( r, c ).data().toDouble();
            min = float( qMin( min, value ));
            max = float( qMax( max, value ));
            QString s = QString::number( value, 'f', this->precision(CartesianCoordinateSystem::Y) );
            valueWidth = qMax( valueWidth, metrics.width( s ) );
        }
    }

    //d->yAxis().setBounds( min, max );
    this->setBounds( CartesianCoordinateSystem::Y, min, max );
    this->setBounds( CartesianCoordinateSystem::X, 0, rows );
    this->setLabelsLength( CartesianCoordinateSystem::Y, valueWidth + 5 );
    this->setLabelsLength( CartesianCoordinateSystem::X, textWidth );*/
}

}
