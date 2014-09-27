#include "abstractgraphalgorithm.h"

#include "graphview.h"

namespace Thistle
{
AbstractGraphAlgorithm::AbstractGraphAlgorithm( GraphView* parent )
    : QObject( parent ), d_ptr ( new AbstractGraphAlgorithmPrivate() )
{}

AbstractGraphAlgorithm::AbstractGraphAlgorithm( AbstractGraphAlgorithmPrivate* d, GraphView* parent )
    : QObject( parent ), d_ptr( d )
{}

AbstractGraphAlgorithm::~AbstractGraphAlgorithm()
{}


const Node AbstractGraphAlgorithm::node( const QModelIndex& index ) const
{
    return d_ptr->itemPos.value( index, Node() );
}
}