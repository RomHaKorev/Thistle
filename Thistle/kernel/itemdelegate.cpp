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

#include "itemdelegate.h"
#include "itemdelegate_p.h"

#include <QPainter>
#include <QLineEdit>

namespace Thistle {

    ItemDelegate::ItemDelegate( QWidget* parent ) : QStyledItemDelegate(parent) {
        d_ptr = new ItemDelegatePrivate();
    }

    ItemDelegate::~ItemDelegate() {
        delete d_ptr;
    }

    QWidget* ItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        Q_UNUSED( parent )
        Q_UNUSED( option )
        Q_UNUSED( index )
        
        QLineEdit* editor = new QLineEdit( parent );
        editor->setGeometry( option.rect );
        return editor;
    }

    void ItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index ) const {
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>( editor );
        if ( lineEdit != 0 ) {
            lineEdit->setText( index.model()->data( index ).toString() );
        }

    }
    void ItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>( editor );
        if ( lineEdit != 0 ) {
            model->setData( index, lineEdit->text(), Qt::DisplayRole );
        }
    }

    void ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        painter->save();
        painter->setBrush( d_ptr->style.brush() );
        painter->setPen( d_ptr->style.pen() );
        const QRect& r = option.rect;
        switch( d_ptr->style.shape() ) {
        case Thistle::Ellipse:
            painter->drawEllipse( r );
            break;
        case Thistle::RoundedRect:
            painter->drawRoundedRect( r, 5, 5 );
            break;
        case Thistle::Diamond:
            painter->drawPolygon( QPolygon()
                << r.topLeft() + QPoint( r.width()/2, 0 )
                << r.topRight() + QPoint( 0, r.height()/2 )
                << r.bottomLeft() + QPoint( r.width()/2, 0 )
                << r.topLeft() + QPoint( 0, r.height()/2 )
                );
            break;
        case Thistle::Triangle:
            painter->drawPolygon( QPolygon()
                << r.topLeft() + QPoint( r.width()/2, 0 )
                << r.bottomLeft() << r.bottomRight()
                );
            break;
        case Thistle::ReversedTriangle:
            painter->drawPolygon( QPolygon()
                << r.bottomLeft() + QPoint( r.width()/2, 0 )
                << r.topLeft() << r.topRight()
                );
            break;
        default:
            painter->drawRect( r );
        }
        if ( d_ptr->style.displayText() == true ) {
            painter->setPen( d_ptr->style.textColor() );
            painter->drawText( r, Qt::AlignCenter, index.model()->data( index ).toString() );
        }
        painter->restore();
    }

    QPolygon ItemDelegate::createDiamond( const QRect& r ) const {
        QPolygon poly;
        poly << r.topLeft() + QPoint( r.width()/2, 0 );
        poly << r.topRight() + QPoint( 0, r.height()/2 );
        poly << r.bottomLeft() + QPoint( r.width()/2, 0 );
        poly << r.topLeft() + QPoint( 0, r.height()/2 );
        return poly;
    }

    void ItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        Q_UNUSED( editor )
        Q_UNUSED( option )
        Q_UNUSED( index )
    }

    void ItemDelegate::setItemStyle( const ItemStyle& s ) {
        d_ptr->style = s;
    }

    ItemStyle& ItemDelegate::itemStyle() const {
        return d_ptr->style;
    }

}