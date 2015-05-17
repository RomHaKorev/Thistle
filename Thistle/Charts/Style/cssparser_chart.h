#ifndef THISTLE_CSSPARSER_CHART_H
#define THISTLE_CSSPARSER_CHART_H

#include <stdlib.h>
#include <string>
#include <iostream>
#include <map>

#include "../base/serieformat.h"

#define YYSTYPE int
int yyparse();
int yylex();

int yyerror( const char *s );
bool getSerieId( const std::string& str, int& out );


class SerieFormatDigester
{
private:
	Thistle::SerieFormat defaultFormat;
	std::map<int, Thistle::SerieFormat> formats;
	Thistle::SerieFormat& currentFormat;

public:
	explicit SerieFormatDigester();

	void selectSerie( int i );

	QPen& currentPen();
	void setBrush( int color );


	static SerieFormatDigester* instance;
};

#endif // THISTLE_CSSPARSER_CHART_H