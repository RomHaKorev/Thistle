#include "itemstyle.h"
#include "itemstyle_p.h"

namespace Thistle
{

ItemStyle::ItemStyle()
{
    d_ptr = new ItemStylePrivate();
}


void ItemStyle::setBrush( const QBrush& brush )
{
    d_ptr->brush = brush;
}


void ItemStyle::setPen( const QPen& pen )
{
    d_ptr->border = pen;
}


void ItemStyle::setFont( const QFont& font )
{
    d_ptr->font = font;
}


void ItemStyle::setTextColor( const QColor& color )
{
    d_ptr->textColor = color;
}


void ItemStyle::setDisplayText( bool displayText )
{
    d_ptr->displayText = displayText;
}


void ItemStyle::setShape( Thistle::Shape shape )
{
    d_ptr->shape = shape;
}


QBrush ItemStyle::brush() const
{
    return d_ptr->brush;
}


QPen ItemStyle::pen() const
{
    return d_ptr->border;
}



QFont ItemStyle::font() const
{
    return d_ptr->font;
}


QColor ItemStyle::textColor() const
{
    return d_ptr->textColor;
}


bool ItemStyle::displayText() const
{
    return d_ptr->displayText;
}



Thistle::Shape ItemStyle::shape() const
{
    return d_ptr->shape;
}

}