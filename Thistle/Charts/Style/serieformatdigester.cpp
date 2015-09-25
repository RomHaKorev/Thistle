#include "serieformatdigester.h"

#include <QtCore/QDebug>

int yyerror( const char *s )
{
	std::cout << "Parser error: " << s << std::endl;
	return 0;
}

bool getSerieId( const std::string& str, int& out )
{
	int ok = sscanf_s( str.c_str(), "serie%d", &out );
	return (ok == 1) && ( out >= 0 );
}


SerieFormatDigester* SerieFormatDigester::instance = new SerieFormatDigester();

SerieFormatDigester::SerieFormatDigester():
defaultFormat(), currentFormat( defaultFormat )
{}

void SerieFormatDigester::selectSerie( int i )
{
	if ( i == -1 )
		currentFormat = defaultFormat;
	else
	{
		std::map<int, Thistle::SerieFormat>::iterator it = formats.find( i );
		if ( it != formats.end() )
			currentFormat = it->second;
	}
}

QPen& SerieFormatDigester::currentPen()
{
	return currentFormat.d->pen;
}

void SerieFormatDigester::setShape( Thistle::Shape shape )
{
	currentFormat.d->shape = shape;
}

void SerieFormatDigester::setType( Thistle::Types type )
{
	currentFormat.d->type = type;
}