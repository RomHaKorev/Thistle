#ifndef GRAPHMODEL_H
#define GRAPHMODEL_H
#include <QStandardItemModel>

#include "edge.h"

#include "graphmodel_p.h"



namespace Thistle
{
class GraphModel: public QStandardItemModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE( GraphModel );
private:
    GraphModelPrivate* d_ptr;
protected:
    GraphModel( GraphModelPrivate* d, QObject* parent = 0 );
    GraphModel( GraphModelPrivate* d, int rows, int columns, QObject* parent = 0 );
public:
    GraphModel( QObject* parent = 0 );
    GraphModel( int rows, int columns, QObject* parent = 0 );
    ~GraphModel();

    void addEdge( const QModelIndex& idx1, const QModelIndex& idx2, Edge::Type type = Edge::NoArrow );
    void addEdge( int row1, int col1, int row2, int col2, Edge::Type type = Edge::NoArrow );

    QList<Edge> edges() const;
signals:
    void updateEdges();
};

}

#endif // GRAPHMODEL_H
