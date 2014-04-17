#ifndef VERTICALTREE_P_H
#define VERTICALTREE_P_H


#include "tree_p.h"

struct VerticalTreePrivate : public TreePrivate {
    VerticalTreePrivate();
    int connectorType;
    qreal max;
    qreal min;
};

#endif // VERTICALTREE_P_H
