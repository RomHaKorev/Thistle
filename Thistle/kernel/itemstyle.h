#ifndef ITEMSTYLE_H
#define ITEMSTYLE_H

#include <QBrush>
#include <QFont>
#include <QPen>

#include "global.h"

namespace Thistle
{

class ItemStylePrivate;

class ItemStyle
{
    ItemStylePrivate* d_ptr;

public:
    ItemStyle();

    void setBrush( const QBrush& brush, bool enabled = true );
	void setPen( const QPen& pen, bool enabled = true );
    void setFont( const QFont& font );
	void setTextColor( const QColor& color, bool enabled = true );
    void setDisplayText( bool displayText = true );
    void setShape( Thistle::Shape shape );
	QBrush brush( bool enabled = true ) const;
	QPen pen( bool enabled = true ) const;
    QFont font() const;
	QColor textColor( bool enabled = true ) const;
    bool displayText() const;
    Thistle::Shape shape() const;
};

}
#endif // ITEMSTYLE_H
