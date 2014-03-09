#include "graphmodel.h"

GraphModel::GraphModel( QObject* parent ) : QStandardItemModel( parent ) {
}

GraphModel::GraphModel( int rows, int columns, QObject* parent ) : QStandardItemModel( rows, columns, parent ) {
}

/* Add an edge between the two nodes referenced by the coordinates ([row1], [col1]) and ([row2], [col2]). */
void GraphModel::addEdge( int row1, int col1, int row2, int col2, Edge::Type type ) {
    this->addEdge( this->index( row1, col1 ), this->index( row2, col2 ), type );
}

 /*Add an edge between the two nodes referenced by the QModelIndex idx1 and idx2.*/
void GraphModel::addEdge( QModelIndex idx1, QModelIndex idx2, Edge::Type type ) {
 if ( ( idx1 == idx2 ) || !idx1.isValid() || !idx2.isValid() ) {
        return;
 }
 myEdges.append( Edge( idx1, idx2, type ) );
}

QList<Edge> GraphModel::edges() const {
 return myEdges;
}
