#ifndef ITEMSTYLE_P_H
#define ITEMSTYLE_P_H

#include <QBrush>
#include <QFont>
#include <QPen>

#include "../global.h"
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

	QBrush brush_disabled;
	QPen border_disabled;
	QColor textColor_disabled;

	ItemStylePrivate()
	{
		brush = QBrush( Thistle::Colors::Blue );
		border = QPen( QColor( Thistle::Colors::DarkBlue ), 1.5 );
		font = QFont();
		textColor = QColor( 0xFAFAFA );
		displayText = true;
		shape = Thistle::Rectangle;

		brush_disabled = QBrush( Thistle::Colors::Gray );
		border_disabled = QPen( QColor( Thistle::Colors::LightGray ), 1.5 );
		textColor_disabled = QColor( 0xFAFAFA );
	}
};

}

#endif //ITEMSTYLE_P_H
