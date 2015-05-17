%{
#include "parser.y.h"

#include <string>

#include <QtGui/QPen>

#include "cssparser_chart.h"

#define YY_NO_UNISTD_H

//#define yyterminate() return token::END

int yyerror( const char* str );

bool getSerieId( const std::string& str, int& out );

%}

%a 3000
%o 4000
%option noyywrap
%option debug
%option case-insensitive
%x COMMENT

integer					[0-9]+
rgba					[0-9A-F]{6}

%%
"Thistle_"				return THISTLE;
"serie1"				{ bool ok = getSerieId(yytext, yylval); return SERIE_ID; }

"serieChart"			return SERIECHART_TYPE;
"linearchart"			return SERIECHART_TYPE;

"pen"					return PEN;

"px"					{ return UNIT; }

{integer}				{ yylval = atoi(yytext); return INTEGER; }
{rgba}					{ return RGB; }

":"                     return ':';
";"						return ';';

[ \t]+                  {/* ignore whitespace */}
\n                      {/* ignore whitespace */}

"{"						return '{';
"}"						return '}';

"SolidLine"				{ SerieFormatDigester::instance->currentPen().setStyle( Qt::SolidLine ); return PEN_STYLE; }
"DashLine"				{ SerieFormatDigester::instance->currentPen().setStyle( Qt::DashLine ); return PEN_STYLE; }
"DotLine"				{ SerieFormatDigester::instance->currentPen().setStyle( Qt::DotLine ); return PEN_STYLE; }
"DashDotLine"			{ SerieFormatDigester::instance->currentPen().setStyle( Qt::DashDotLine ); return PEN_STYLE; }
"DashDotDotLine"		{ SerieFormatDigester::instance->currentPen().setStyle( Qt::DashDotDotLine ); return PEN_STYLE; }

"MiterJoin"				{ SerieFormatDigester::instance->currentPen().setJoinStyle( Qt::MiterJoin ); return PEN_JOINSTYLE; }
"BevelJoin"				{ SerieFormatDigester::instance->currentPen().setJoinStyle( Qt::BevelJoin ); return PEN_JOINSTYLE; }
"RoundJoin"				{ SerieFormatDigester::instance->currentPen().setJoinStyle( Qt::RoundJoin ); return PEN_JOINSTYLE; }

"FlatCap"				{ SerieFormatDigester::instance->currentPen().setCapStyle( Qt::FlatCap ); return PEN_CAPSTYLE; }
"SquareCap"				{ SerieFormatDigester::instance->currentPen().setCapStyle( Qt::SquareCap ); return PEN_CAPSTYLE; }
"RoundCap"				{ SerieFormatDigester::instance->currentPen().setCapStyle( Qt::RoundCap ); return PEN_CAPSTYLE; }

"Blue"			        { SerieFormatDigester::instance->currentPen().setColor( 0x6090E4 ); return COLOR; }
"Green"			        { SerieFormatDigester::instance->currentPen().setColor( 0x9AEC7A ); return COLOR; }
"Purple"			    { SerieFormatDigester::instance->currentPen().setColor( 0x8600C8 ); return COLOR; }
"Red"			        { SerieFormatDigester::instance->currentPen().setColor( 0xDB0000 ); return COLOR; }
"Orange"			    { SerieFormatDigester::instance->currentPen().setColor( 0xFF8000 ); return COLOR; }
"Yellow"			    { SerieFormatDigester::instance->currentPen().setColor( 0xFDEF46 ); return COLOR; }
"Gray"			        { SerieFormatDigester::instance->currentPen().setColor( 0x868686 ); return COLOR; }
"LightBlue"			    { SerieFormatDigester::instance->currentPen().setColor( 0x68B9FF ); return COLOR; }
"LightGreen"			{ SerieFormatDigester::instance->currentPen().setColor( 0xC9FF71 ); return COLOR; }
"LightPurple"			{ SerieFormatDigester::instance->currentPen().setColor( 0x8274B0 ); return COLOR; }
"LightRed"			    { SerieFormatDigester::instance->currentPen().setColor( 0xFF5656 ); return COLOR; }
"LightOrange"			{ SerieFormatDigester::instance->currentPen().setColor( 0xFFB366 ); return COLOR; }
"LightYellow"			{ SerieFormatDigester::instance->currentPen().setColor( 0xFDF285 ); return COLOR; }
"LightGray"			    { SerieFormatDigester::instance->currentPen().setColor( 0xBEBEBE ); return COLOR; }
"DarkBlue"			    { SerieFormatDigester::instance->currentPen().setColor( 0x0839A1 ); return COLOR; }
"DarkGreen"			    { SerieFormatDigester::instance->currentPen().setColor( 0x8CD01E ); return COLOR; }
"DarkPurple"			{ SerieFormatDigester::instance->currentPen().setColor( 0x7300AB ); return COLOR; }
"DarkRed"			    { SerieFormatDigester::instance->currentPen().setColor( 0xBA0000 ); return COLOR; }
"DarkOrange"			{ SerieFormatDigester::instance->currentPen().setColor( 0xff5100 ); return COLOR; }
"DarkYellow"			{ SerieFormatDigester::instance->currentPen().setColor( 0xFFF219 ); return COLOR; }
"DarkGray"			    { SerieFormatDigester::instance->currentPen().setColor( 0x505050 ); return COLOR; }

.                       {yyerror("Illegal character");}

