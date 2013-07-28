#ifndef GRAPHMODEL_H
#define GRAPHMODEL_H

#include <QStandardItemModel>
#include <QPair>
#include "node.h"
#include "edge.h"

class GraphModel : public QStandardItemModel {
  Q_OBJECT
public:
  explicit GraphModel( QObject *parent = 0 );
  QList<Node> nodes() const;
signals:
  
public slots:
  
};

#endif // GRAPHMODEL_H
