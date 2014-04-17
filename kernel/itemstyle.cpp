#include "itemstyle.h"

ItemStyle::ItemStyle() {
    d = new ItemStylePrivate();
}

void ItemStyle::setBackground( const QBrush& brush ) {
    d->brush = brush;
}

void ItemStyle::setBorder( const QPen& pen ) {
    d->border = pen;
}

void ItemStyle::setFont( const QFont& font ) {
    d->font = font;
}

void ItemStyle::setTextColor( const QColor& color ) {
    d->textColor = color;
}

void ItemStyle::setDisplayText( bool displayText ) {
    d->displayText = displayText;
}

void ItemStyle::setShape( Marb::Shape shape ) {
    d->shape = shape;
}

QBrush ItemStyle::background() const {
    return d->brush;
}

QPen ItemStyle::border() const {
    return d->border;
}

QFont ItemStyle::font() const {
    return d->font;
}

QColor ItemStyle::textColor() const {
    return d->textColor;
}

bool ItemStyle::displayText() const {
    return d->displayText;
}

Marb::Shape ItemStyle::shape() const {
    return d->shape;
}