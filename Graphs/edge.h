#ifndef EDGE_H
#define EDGE_H

#include <QModelIndex>
#include <QPainter>

namespace Thistle {

    /*!
    \class Edge
    \brief The \a Edge class represents an edge between two \a Node objects in a \a Graph.

    The \a Edge class stores the two connected \a Node objects and its \a Type. It will be in charge to paint the connections on the \a Graph.
    */
    class Edge {
    public:
        /*!
        Defines the type of connection between two \a Node objects.
        */
        enum Type {
            NoArrow = 0,
            Bilateral    = 1,
            Unilateral = 2
        };
        Q_ENUMS( Type )

         /*!
         Constructs a \a Edge between \a idx1 and \a idx2 with the given \a type.
         */
        Edge( QModelIndex idx1, QModelIndex idx2, Edge::Type type = NoArrow );

        QModelIndex leftIndex;
        QModelIndex rightIndex;
        Edge::Type type;

        /*!
        Paints the connections between its two \a Node objects. \a r1 and \r2 defines the bounding rect of \a leftIndex and \a rightIndex.
        */
        void paintEdge( QPainter& painter, QRectF r1, QRectF r2 ) const;

        /*!
        If the \a Type is different to \a Type::NoArrow, paints the arrows of the \a Edge.
        */
        void paintArrow( QPainter& painter, const QLineF& line ) const;
    };

}
#endif // EDGE_H
