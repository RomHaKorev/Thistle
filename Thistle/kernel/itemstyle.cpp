#include "itemstyle.h"
#include "itemstyle_p.h"

namespace Thistle
{

ItemStyle::ItemStyle()
{
    d_ptr = new ItemStylePrivate();
}


void ItemStyle::setBrush( const QBrush& brush, bool enabled )
{
	if ( enabled )
		d_ptr->brush = brush;
	else
		d_ptr->brush_disabled = brush;
}


void ItemStyle::setBorderPen( const QPen& pen, bool enabled )
{
	if ( enabled )
		d_ptr->border = pen;
	else
		d_ptr->border_disabled = pen;
}


void ItemStyle::setFont( const QFont& font )
{
    d_ptr->font = font;
}


void ItemStyle::setTextColor( const QColor& color, bool enabled )
{
	if ( enabled )
		d_ptr->textColor = color;
	else
		d_ptr->textColor_disabled = color;
}


void ItemStyle::setDisplayText( bool displayText )
{
    d_ptr->displayText = displayText;
}


void ItemStyle::setShape( Thistle::Shape shape )
{
    d_ptr->shape = shape;
}


QBrush ItemStyle::brush( bool enabled ) const
{
	if ( enabled )
		return d_ptr->brush;
	else
		return d_ptr->brush_disabled;
}


QPen ItemStyle::borderPen( bool enabled ) const
{
	if ( enabled )
		return d_ptr->border;
	else
		return d_ptr->border_disabled;
}



QFont ItemStyle::font() const
{
    return d_ptr->font;
}


QColor ItemStyle::textColor( bool enabled ) const
{
	if ( enabled )
		return d_ptr->textColor;
	else
		return d_ptr->textColor_disabled;
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