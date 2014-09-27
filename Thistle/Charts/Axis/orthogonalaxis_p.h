#ifndef ORTHOGONALAXIS_P_H
#define ORTHOGONALAXIS_P_H

#include "abstractaxis_p.h"

namespace Thistle
{

struct OrthogonalAxisPrivate : public AbstractAxisPrivate
{
    OrthogonalAxisPrivate();

    QLineF yaxis;
    QLineF xaxis;
    bool startOnAxis;
    bool verticalLabels;
};

}

#endif // ORTHOGONALAXIS_P_H
