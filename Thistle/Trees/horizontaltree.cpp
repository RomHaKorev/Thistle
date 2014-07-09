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

#include "horizontaltree.h"
#include <QPainter>
#include <QScrollBar>

namespace Thistle {

HorizontalTree::HorizontalTree() : VerticalTree() {
}

void HorizontalTree::positionsInView() {
    Q_D( VerticalTree );
    d->realSize.setWidth( ( d->depth ) * ( d->xDistance + d->itemRect.width() ) + d->itemRect.width() );
    d->realSize.setHeight( d->left * ( d->yDistance + d->itemRect.height() ) + d->itemRect.height() );

    setScrollBarValues();

    QPointF offset( d->itemRect.width()/2, d->itemRect.height()/2 );
    offset += d->itemOffset;
    Q_FOREACH( QModelIndex index, d->itemTreePos.keys() ) {
            QPointF p = d->itemTreePos[ index ];
            d->itemPos[index] = QPointF( p.y() * ( d->xDistance + d->itemRect.width() ), p.x() * ( d->yDistance + d->itemRect.height() ) ) + offset;
    }
}

void HorizontalTree::setScrollBarValues() {
    Q_D( VerticalTree );
    VerticalTree::setScrollBarValues();
    qreal delta = height() - d->realSize.height();
    if ( delta > 20 ) {
        d->itemOffset = QPointF( 10, delta/2.0 );
    }
}

}