/*
 This file is part of Thistle.

    Thistle is free software: you can redistribute it and/or modify
    it under the terms of the Lesser GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License.

    Thistle is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    Lesser GNU General Public License for more details.

    You should have received a copy of the Lesser GNU General Public License
    along with Thistle.    If not, see <http://www.gnu.org/licenses/>.

 Thistle    Copyright (C) 2013    Dimitry Ernot & Romha Korev
*/

#ifndef HORIZONTALTREE_H
#define HORIZONTALTREE_H

#include "verticaltree.h"

#include "verticaltree_p.h"

namespace Thistle {

class HorizontalTree : public VerticalTree {
    Q_DECLARE_PRIVATE( VerticalTree );
protected:
    virtual void positionsInView();
    virtual void setScrollBarValues();
public:
    HorizontalTree();
};

}

#endif // HORIZONTALTREE_H
