#include "abstractlayer.h"

#include "../abstractchart.h"
#include "abstractlayer_p.h"

#include <QAbstractItemModel>
#include <QDebug>

namespace Thistle
{

AbstractLayer::AbstractLayer( AbstractChart* parent )
    : QObject( parent ), d_ptr( new AbstractLayerPrivate( parent ) )
{}

AbstractLayer::~AbstractLayer()
{}

qreal AbstractLayer::zValue() const
{
    return d_ptr->zValue;
}

void AbstractLayer::setZValue( qreal value )
{
    d_ptr->zValue = value;
}

void AbstractLayer::paint( QPainter& painter )
{
    d_ptr->paint( painter );
}

}