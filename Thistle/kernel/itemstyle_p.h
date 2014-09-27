#ifndef ITEMSTYLE_P_H
#define ITEMSTYLE_P_H

#include <QBrush>
#include <QFont>
#include <QPen>

#include "global.h"
namespace Thistle
{
struct ItemStylePrivate
{
    QBrush brush;
    QPen border;
    QFont font;
    QColor textColor;
    bool displayText;
    Thistle::Shape shape;
    ItemStylePrivate()
    {
        brush = QBrush( Thistle::Colors::Blue );
        border = QPen( QColor( Thistle::Colors::DarkBlue ), 1.5 );
        font = QFont();
        textColor = QColor( 0xFAFAFA );
        displayText = true;
        shape = Thistle::Rectangle;

    }
};
}

#endif //ITEMSTYLE_P_H