#include "abstractlayer_p.h"
#include "../AbstractChart.h"

#include <QPainter>

namespace Thistle
{
AbstractLayerPrivate::AbstractLayerPrivate( AbstractChart* aChart )
    : chart( aChart ), zValue( 10 )
{}

AbstractLayerPrivate::~AbstractLayerPrivate()
{}

QAbstractItemModel* AbstractLayerPrivate::model() const
{
    if ( this->chart != 0 )
        return this->chart->model();
    
    return 0;
}

void AbstractLayerPrivate::paint( QPainter& painter ) const
{
    QAbstractItemModel* model = this->model();

    int rows = model->rowCount();
    int cols = model->columnCount();
    for ( int c = 0; c < cols; ++c )
    {
        SerieFormat format = this->chart->serieFormat( c );
        for ( int r = 0; r < rows; ++r )
        {
            QModelIndex index = model->index( r, c );
            QLineF vector = index.data( Qt::UserRole ).toLineF();
            if ( !vector.isNull() )
            {
                QPointF p = this->chart->itemRect( index ).center();
                vector.translate( -vector.p1() + p );
                painter.drawLine( vector );
            }
        }
    }
}
}