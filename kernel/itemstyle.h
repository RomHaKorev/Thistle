#ifndef ITEMSTYLE_H
#define ITEMSTYLE_H

#include <QBrush>
#include <QFont>
#include <QPen>

#include "global.h"

namespace Thistle {
    class ItemStylePrivate;
    /*!
    \class ItemStyle
    \brief ItemStyle class is used by \a ItemDelegate to set the look and feel of item.

    An item style consists of :
    - a \c QBrush for background
    - a \c QPen for the border
    - a \c QFont for the text
    - a \s Thistle::Shape for the item shape
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
        void setShape( Thistle::Shape shape );
        QBrush brush() const;
        QPen pen() const;
        QFont font() const;
        QColor textColor() const;
        bool displayText() const;
        Thistle::Shape shape() const;
    };

}
#endif // ITEMSTYLE_H
