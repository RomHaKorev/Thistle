#ifndef ITEMSTYLE_H
#define ITEMSTYLE_H

#include <QBrush>
#include <QFont>
#include <QPen>

#include "global.h"

class ItemStyle;

struct ItemStylePrivate {
    QBrush brush;
    QPen border;
    QFont font;
    QColor textColor;
    bool displayText;
    Marb::Shape shape;
    ItemStylePrivate() {
        brush = QBrush( Marb::Color::Blue );
        border = QPen( QColor( Marb::Color::DarkBlue ), 1.5 );
        font = QFont();
        textColor = QColor( 0xFAFAFA );
        displayText = true;
        shape = Marb::Rectangle;
        
    }
};

class ItemStyle {
    ItemStylePrivate* d;

public:
    ItemStyle();

    void setBackground( const QBrush& brush );
    void setBorder( const QPen& pen );
    void setFont( const QFont& font );
    void setTextColor( const QColor& color );
    void setDisplayText( bool displayText = true );
    void setShape( Marb::Shape shape );
    QBrush background() const;
    QPen border() const;
    QFont font() const;
    QColor textColor() const;
    bool displayText() const;
    Marb::Shape shape() const;
};


#endif // ITEMSTYLE_H
