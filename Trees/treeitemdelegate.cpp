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
  along with Marb.  If not, see <http://www.gnu.org/licenses/>.

 Marb  Copyright (C) 2013  Dimitry Ernot
*/

#include "treeitemdelegate.h"

#include <QPainter>
#include <QDebug>
#include <QLineEdit>

TreeItemDelegate::TreeItemDelegate( Tree* parent ) :
  QStyledItemDelegate(parent) {
  myStyle  = TreeStyle();
}

QWidget* TreeItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
  Q_UNUSED( parent )
  Q_UNUSED( option )
  Q_UNUSED( index )
  QLineEdit* editor = new QLineEdit( parent );
  editor->setGeometry( option.rect );
  return editor;
}

void TreeItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index ) const {
  QLineEdit* lineEdit = qobject_cast<QLineEdit*>( editor );
  if ( lineEdit != 0 ) {
    lineEdit->setText( index.model()->data( index ).toString() );
  }

}
void TreeItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
  QLineEdit* lineEdit = qobject_cast<QLineEdit*>( editor );
  if ( lineEdit != 0 ) {
    model->setData( index, lineEdit->text(), Qt::DisplayRole );
  }
}

void TreeItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
  painter->save();
  painter->setBrush( myStyle.background() );
  painter->setPen( myStyle.border() );
  const QRect& r = option.rect;
  switch( myStyle.shape() ) {
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
  if ( myStyle.displayText() == true ) {
    painter->setPen( myStyle.textColor() );
    painter->drawText( r, Qt::AlignCenter, index.model()->data( index ).toString() );
  }
  painter->restore();
}

QPolygon TreeItemDelegate::createDiamond( QRect r ) const {
  QPolygon poly;
  poly << r.topLeft() + QPoint( r.width()/2, 0 );
  poly << r.topRight() + QPoint( 0, r.height()/2 );
  poly << r.bottomLeft() + QPoint( r.width()/2, 0 );
  poly << r.topLeft() + QPoint( 0, r.height()/2 );
  return poly;
}

void TreeItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const {
  Q_UNUSED( editor )
  Q_UNUSED( option )
  Q_UNUSED( index )
}


void TreeItemDelegate::setTreeStyle( TreeStyle s ) {
  myStyle = s;
}
