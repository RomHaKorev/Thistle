#ifndef GRAPHMODEL_H
#define GRAPHMODEL_H
#include <QStandardItemModel>

#include "edge.h"

class GraphModel: public QStandardItemModel {
    Q_OBJECT
protected:
    QList<Edge> myEdges;
public:
    GraphModel( QObject* parent = 0 );
    GraphModel( int rows, int columns, QObject* parent = 0 );

    void addEdge( QModelIndex idx1, QModelIndex idx2, Edge::Type type = Edge::NoArrow );
    void addEdge( int row1, int col1, int row2, int col2, Edge::Type type = Edge::NoArrow );

    QList<Edge> edges() const;
};

#endif // GRAPHMODEL_H
