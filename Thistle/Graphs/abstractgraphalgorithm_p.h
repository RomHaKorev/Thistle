#ifndef ABSTRACTGRAPHALGORITHM_P_H
#define ABSTRACTGRAPHALGORITHM_P_H

#include <QModelIndex>
#include <QMap>

#include "node.h"


namespace Thistle {

    struct AbstractGraphAlgorithmPrivate {
        QMap<QModelIndex, Node> itemPos;

        AbstractGraphAlgorithmPrivate();
        ~AbstractGraphAlgorithmPrivate();
    };

}

#endif // ABSTRACTGRAPHALGORITHM_P_H
