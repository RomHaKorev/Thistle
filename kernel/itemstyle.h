#ifndef ITEMSTYLE_H
#define ITEMSTYLE_H

#include <QBrush>
#include <QFont>
#include <QPen>

#include "global.h"


namespace Thistle {

class ItemStyle;

struct ItemStylePrivate {
    QBrush brush;
    QPen border;
    QFont font;
    QColor textColor;
    bool displayText;
    Global::Shape shape;
    ItemStylePrivate() {
        brush = QBrush( Global::Blue );
        border = QPen( QColor( Global::DarkBlue ), 1.5 );
        font = QFont();
        textColor = QColor( 0xFAFAFA );
        displayText = true;
        shape = Global::Rectangle;
        
    }
};

/*!
\class ItemStyle
\brief ItemStyle class is used by \a ItemDelegate to set the look and feel of item.

An item style consists of :
    - a \c QBrush for background
    - a \c QPen for the border
    - a \c QFont for the text
    - a \s Global::Shape for the item shape
*/
class ItemStyle {
    ItemStylePrivate* d_ptr;

public:
    ItemStyle();

    void setBrush( const QBrush& brush );
    void setPen( const QPen& pen );
    void setFont( const QFont& font );
    void setTextColor( const QColor& color );
    void setDisplayText( bool displayText = true );
    void setShape( Global::Shape shape );
    QBrush brush() const;
    QPen pen() const;
    QFont font() const;
    QColor textColor() const;
    bool displayText() const;
    Global::Shape shape() const;
};

}
#endif // ITEMSTYLE_H
