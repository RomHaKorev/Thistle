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

#ifndef MARBITEMDELEGATE_H
#define MARBITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QPen>
#include <QBrush>
#include "Marb.h"



class MarbStyle {
    QBrush myBrush;
    QPen myBorder;
    QFont myFont;
    QColor myTextColor;
    bool myDisplayText;
    Marb::Shape myShape;

public:
    MarbStyle() {
        myBrush = QBrush( Marb::Blue );
        myBorder = QPen( QColor( Marb::DarkBlue ), 1.5 );
        myFont = QFont();
        myTextColor = QColor( 0xFAFAFA );
        myDisplayText = true;
        myShape = Marb::Rectangle;
    }

    void setBackground( QBrush brush ) {
        myBrush = brush;
    }

    void setBorder( QPen pen ) {
        myBorder = pen;
    }

    void setFont( QFont font ) {
        myFont = font;
    }

    void setTextColor( QColor color) {
        myTextColor = color;
    }

    void setDisplayText( bool displayText = true) {
        myDisplayText = displayText;
    }

    void setShape( Marb::Shape shape) {
        myShape = shape;
    }

    QBrush background() const {
        return myBrush;
    }

    QPen border() const {
        return myBorder;
    }

    QFont font() const {
        return myFont;
    }

    QColor textColor() const {
        return myTextColor;
    }

    bool displayText() const {
        return myDisplayText;
    }

    Marb::Shape shape() const {
        return myShape;
    }

};


class MarbItemDelegate : public QStyledItemDelegate {
    Q_OBJECT
protected:
    QPolygon createDiamond( QRect r ) const;
    MarbStyle myStyle;
public:

    void setItemStyle( MarbStyle myStyle );
    MarbStyle itemStyle() const;
    explicit MarbItemDelegate(QWidget* parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:

public slots:

};


#endif // MARBITEMDELEGATE_H