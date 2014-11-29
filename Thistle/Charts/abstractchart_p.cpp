#include "abstractchart_p.h"

#include "legends/chartlegend.h"

namespace Thistle
{

AbstractChartPrivate::AbstractChartPrivate( AbstractChart* q ) : q_ptr( q ), titleFont()
{
    this->titleFont.setPixelSize( 14 );
    this->titleFont.setItalic( true );
    this->legend = 0;
}

AbstractChartPrivate::~AbstractChartPrivate()
{
    if ( this->legend != 0 ) delete this->legend;
}

void AbstractChartPrivate::addLayer( AbstractLayer* layer )
{
    std::list<AbstractLayer*>::iterator it = this->layers.begin();
    for ( ; it != layers.end(); ++it )
    {
        qreal zValue = (*it)->zValue();
        if ( zValue >= layer->zValue() )
        {
            this->layers.insert( it, layer );
            return;
        }
    }
    this->layers.push_back( layer );
}

void AbstractChartPrivate::applyLayer( QPainter& painter )
{
    std::list<AbstractLayer*>::iterator it = this->layers.begin();
    for ( ; it != this->layers.end(); ++it )
    {
        (*it)->paint( painter );
    }
}

}