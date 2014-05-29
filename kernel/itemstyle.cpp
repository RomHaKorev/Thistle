#include "itemstyle.h"
#include "itemstyle_p.h"

namespace Thistle {

    /*! Constructs a style used to custom the items.
    */
    ItemStyle::ItemStyle() {
        d_ptr = new ItemStylePrivate();
    }

    /*!
    Defines the \c QBrush used to draw the item's background.
    */
    void ItemStyle::setBrush( const QBrush& brush ) {
        d_ptr->brush = brush;
    }


    /*!
    Defines the \c QPen used to draw the item's border.
    */
    void ItemStyle::setPen( const QPen& pen ) {
        d_ptr->border = pen;
    }


    /*!
    Defines the font used to draw the item's text.
    */
    void ItemStyle::setFont( const QFont& font ) {
        d_ptr->font = font;
    }


    /*!
    Defines the text color to \a color.
    */
    void ItemStyle::setTextColor( const QColor& color ) {
        d_ptr->textColor = color;
    }


    /*!
    If \a displayText is equal to \c true, the item displays the text. Otherwise, the text is hidden.
    It is useful for small items.
    */
    void ItemStyle::setDisplayText( bool displayText ) {
        d_ptr->displayText = displayText;
    }


    /*! Defines the item's \c shape.
    */
    void ItemStyle::setShape( Thistle::Shape shape ) {
        d_ptr->shape = shape;
    }


    /*!
    Returns the \c QBrush used to draw the item's backgournd.
    */
    QBrush ItemStyle::brush() const {
        return d_ptr->brush;
    }


    /*!
    Returns the \c QPen used to draw the item's borders.
    */
    QPen ItemStyle::pen() const {
        return d_ptr->border;
    }


    /*!
    Returns the \c QFont used to draw the text.
    */
    QFont ItemStyle::font() const {
        return d_ptr->font;
    }


    /*!
    Returns the \c QColor used to draw the text.
    */
    QColor ItemStyle::textColor() const {
        return d_ptr->textColor;
    }


    /*!
    Returns \c true if the item's text is displayed. Otherwise returns \c false.
    */
    bool ItemStyle::displayText() const {
        return d_ptr->displayText;
    }


    /*!
    Returns the \c Thistle::Shape.
    */
    Thistle::Shape ItemStyle::shape() const {
        return d_ptr->shape;
    }

}