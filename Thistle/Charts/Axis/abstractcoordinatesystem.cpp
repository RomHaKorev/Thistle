#include "abstractcoordinatesystem.h"
#include "../../kernel/global.h"
#include <QAbstractItemModel>

namespace Thistle
{

AbstractCoordinateSystem::AbstractCoordinateSystem() : d_ptr( new AbstractCoordinateSystemPrivate() )
{
    //d_ptr->tickSize = 0;
    //d_ptr->nbTicks = 10;
    d_ptr->axisPen = QPen( QBrush(Thistle::Colors::Gray), 1.5 );
    d_ptr->tickPen = QPen( QBrush(Thistle::Colors::LightGray), 0 );
    d_ptr->textPen = QPen( QBrush(Thistle::Colors::DarkGray), 1 );
}

AbstractCoordinateSystem::AbstractCoordinateSystem( AbstractCoordinateSystemPrivate* d ) : d_ptr( d )
{}


QRect& AbstractCoordinateSystem::valuesRect()
{
    return d_ptr->valuesRect;
};
QRect& AbstractCoordinateSystem::chartRect()
{
    return d_ptr->chartRect;
};

QFont AbstractCoordinateSystem::font() const
{
    return d_ptr->font;
};

QPen AbstractCoordinateSystem::axisPen() const
{
    return d_ptr->axisPen;
};
QPen AbstractCoordinateSystem::tickPen() const
{
    return AbstractCoordinateSystem::d_ptr->tickPen;
};
QPen AbstractCoordinateSystem::textPen() const
{
    return d_ptr->textPen;
};

QAbstractItemModel* AbstractCoordinateSystem::model() const
{
    return d_ptr->model;
};


void AbstractCoordinateSystem::setValuesRect( const QRect& r )
{
    d_ptr->valuesRect = r;
};


void AbstractCoordinateSystem::setChartRect( const QRect& r )
{
    d_ptr->chartRect = r;
};


void AbstractCoordinateSystem::setFont( const QFont& f )
{
    d_ptr->font = f;
};


void AbstractCoordinateSystem::setAxisPen( const QPen& p )
{
    d_ptr->axisPen = p;
};


void AbstractCoordinateSystem::setTickPen( const QPen& p )
{
    d_ptr->tickPen = p;
};


void AbstractCoordinateSystem::setTextPen( const QPen& p )
{
    d_ptr->textPen = p;
};


void AbstractCoordinateSystem::setModel( QAbstractItemModel* m )
{
    d_ptr->model = m;
};


}
