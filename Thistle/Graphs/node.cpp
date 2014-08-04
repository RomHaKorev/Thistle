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

#include "node.h"

namespace Thistle {

Node::Node() {
    d_ptr = new NodePrivate();
}

Node::~Node() {
    delete d_ptr;
}

QPointF Node::pos() const {
    return d_ptr->pos;
}

void Node::setPos( qreal x, qreal y ) {
    setPos( QPointF( x, y ) );
}

void Node::setPos( QPointF p ) {
    d_ptr->pos = p;
}

void Node::process() {

}

void Node::addEdge( Node* node ) {
    if ( node != this ) {
        d_ptr->connectedNode << node;
    }
}

QList<Node*> Node::edges() const {
    return d_ptr->connectedNode;
}

}