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

#include "node.h"


Node::Node() : myPos(0, 0) {

}

QPointF Node::pos() const {
    return myPos;
}

void Node::setPos( qreal x, qreal y ) {
    setPos( QPointF( x, y ) );
}

void Node::setPos( QPointF p ) {
    myPos = p;
}

void Node::process() {

}

void Node::addEdge( Node* node ) {
    if ( node != this ) {
        myConnectedNode << node;
    }
}

QList<Node*> Node::edges() const {
    return myConnectedNode;
}
