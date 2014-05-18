#ifndef VERTICALTREE_P_H
#define VERTICALTREE_P_H


#include "abstracttree_p.h"

namespace Thistle {

struct VerticalTreePrivate : public AbstractTreePrivate {
    VerticalTreePrivate();
    int connectorType;
    qreal max;
    qreal min;
};

}

#endif // VERTICALTREE_P_H
