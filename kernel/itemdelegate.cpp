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

#include "itemdelegate.h"
#include "itemdelegate_p.h"

#include <QPainter>
#include <QLineEdit>

ItemDelegate::ItemDelegate( QWidget* parent ) :
    QStyledItemDelegate(parent) {
    d = new ItemDelegatePrivate();
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
    painter->setBrush( d->style.background() );
    painter->setPen( d->style.border() );
    const QRect& r = option.rect;
    switch( d->style.shape() ) {
        case Marb::Ellipse:
            painter->drawEllipse( r );
        break;
        case Marb::RoundedRect:
            painter->drawRoundedRect( r, 5, 5 );
        break;
        case Marb::Diamond:
        painter->drawPolygon( QPolygon()
                                                    << r.topLeft() + QPoint( r.width()/2, 0 )
                                                    << r.topRight() + QPoint( 0, r.height()/2 )
                                                    << r.bottomLeft() + QPoint( r.width()/2, 0 )
                                                    << r.topLeft() + QPoint( 0, r.height()/2 )
                );
        break;
    case Marb::Triangle:
        painter->drawPolygon( QPolygon()
                                                    << r.topLeft() + QPoint( r.width()/2, 0 )
                                                    << r.bottomLeft() << r.bottomRight()
                         );
    break;
    case Marb::ReversedTriangle:
        painter->drawPolygon( QPolygon()
                                                    << r.bottomLeft() + QPoint( r.width()/2, 0 )
                                                    << r.topLeft() << r.topRight()
                         );
    break;
    default:
            painter->drawRect( r );
    }
    if ( d->style.displayText() == true ) {
        painter->setPen( d->style.textColor() );
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
    d->style = s;
}

ItemStyle ItemDelegate::itemStyle() const {
    return d->style;
}
