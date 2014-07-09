#include "graphmodel.h"
#include <QDebug>

namespace Thistle {

GraphModel::GraphModel( QObject* parent ) : QStandardItemModel( parent ) {
    d_ptr = new GraphModelPrivate();
}

GraphModel::GraphModel( int rows, int columns, QObject* parent ) : QStandardItemModel( rows, columns, parent ) {
    d_ptr = new GraphModelPrivate();
}

GraphModel::GraphModel( GraphModelPrivate* d, QObject* parent ) : QStandardItemModel( parent ){
    d_ptr = d;
}

GraphModel::GraphModel( GraphModelPrivate* d, int rows, int columns, QObject* parent ) : QStandardItemModel( rows, columns, parent ) {
    d_ptr = d;
}

GraphModel::~GraphModel() {
    delete d_ptr;
}

void GraphModel::addEdge( int row1, int col1, int row2, int col2, Edge::Type type ) {
    this->addEdge( this->index( row1, col1 ), this->index( row2, col2 ), type );
}

void GraphModel::addEdge( const QModelIndex& idx1, const QModelIndex& idx2, Edge::Type type ) {
    if ( ( idx1 == idx2 ) || !idx1.isValid() || !idx2.isValid() ) return;
    d_ptr->edges.append( Edge( idx1, idx2, type ) );
    emit updateEdges();
}

QList<Edge> GraphModel::edges() const {
    return d_ptr->edges;
}


}