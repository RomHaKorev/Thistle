#include "abstractchart_p.h"

namespace Thistle
{
AbstractChartPrivate::AbstractChartPrivate( AbstractChart* q ) : q_ptr( q )
{}

AbstractChartPrivate::~AbstractChartPrivate()
{}

#if 0
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
#endif

}
