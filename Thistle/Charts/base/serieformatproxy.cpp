#include "serieformatproxy.h"

namespace Thistle
{

SerieFormatProxy::SerieFormatProxy( QObject* parent ): QObject( parent )
{}


SerieFormatProxy::~SerieFormatProxy()
{}


bool  SerieFormatProxy::contains( int column ) const
{
	return formats.contains( column );
}


SerieFormat SerieFormatProxy::serieFormat( int column ) const
{
	if ( formats.contains( column ) )
	{
		return formats[column];
	}

	Thistle::SerieFormat style;
	QColor c1 = Thistle::Colors::predefinedLightColor( column );
	QColor c2 = Thistle::Colors::predefinedDarkColor( column );
	style.setPen( QPen( QColor( c2 ), 2 ) );
	style.setBrush( QBrush( c1 ) );
	return style;
}

void SerieFormatProxy::setSerieFormat( int column, const Thistle::SerieFormat& style )
{
	formats[column] = style;
}

}