/*
 This file is part of Marb.

    Marb is free software: you can redistribute it and/or modify
    it under the terms of the Lesser GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License.

    Marb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    Lesser GNU General Public License for more details.

    You should have received a copy of the Lesser GNU General Public License
    along with Marb.    If not, see <http://www.gnu.org/licenses/>.

 Marb    Copyright (C) 2013    Dimitry Ernot & Romha Korev
*/

#ifndef HORIZONTALTREE_H
#define HORIZONTALTREE_H

#include "verticaltree.h"

class HorizontalTree : public VerticalTree {
protected:
    virtual void positionsInView();
    virtual void setScrollBarValues();
public:
    HorizontalTree();
    //virtual void paintEvent(QPaintEvent *);
};

#endif // HORIZONTALTREE_H
