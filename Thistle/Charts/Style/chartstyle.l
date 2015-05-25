%{
#include "chartstyle.y.h"

#include <string>

#include <QtGui/QPen>

#include "serieformatdigester.h"

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

"pen-style"				return P_PENSTYLE;
"pen-color"				return P_PENCOLOR;

"SolidLine"				{ yylval = Qt::SolidLine; return V_PEN_STYLE; }
"DashLine"				{ yylval = Qt::DashLine; return V_PEN_STYLE; }
"DotLine"				{ yylval = Qt::DotLine; return V_PEN_STYLE; }
"DashDotLine"			{ yylval = Qt::DashDotLine; return V_PEN_STYLE; }
"DashDotDotLine"		{ yylval = Qt::DashDotDotLine; return V_PEN_STYLE; }

"MiterJoin"				{ yylval = Qt::MiterJoin; return V_PEN_JOINSTYLE; }
"BevelJoin"				{ yylval = Qt::BevelJoin; return V_PEN_JOINSTYLE; }
"RoundJoin"				{ yylval = Qt::RoundJoin; return V_PEN_JOINSTYLE; }

"FlatCap"				{ yylval = Qt::FlatCap; return V_PEN_CAPSTYLE; }
"SquareCap"				{ yylval = Qt::SquareCap; return V_PEN_CAPSTYLE; }
"RoundCap"				{ yylval = Qt::RoundCap; return V_PEN_CAPSTYLE; }


"style"					return P_SERIETYPE;

"Dot"					{ return V_SERIETYPE; }
"Line"					{ return V_SERIETYPE; }
"Bar"					{ return V_SERIETYPE; }
"DotLine"				{ return V_SERIETYPE; }


"Blue"			        { yylval = 0x6090E4; return COLOR; }
"Green"			        { yylval = 0x9AEC7A; return COLOR; }
"Purple"			    { yylval = 0x8600C8; return COLOR; }
"Red"			        { yylval = 0xDB0000; return COLOR; }
"Orange"			    { yylval = 0xFF8000; return COLOR; }
"Yellow"			    { yylval = 0xFDEF46; return COLOR; }
"Gray"			        { yylval = 0x868686; return COLOR; }
"LightBlue"			    { yylval = 0x68B9FF; return COLOR; }
"LightGreen"			{ yylval = 0xC9FF71; return COLOR; }
"LightPurple"			{ yylval = 0x8274B0; return COLOR; }
"LightRed"			    { yylval = 0xFF5656; return COLOR; }
"LightOrange"			{ yylval = 0xFFB366; return COLOR; }
"LightYellow"			{ yylval = 0xFDF285; return COLOR; }
"LightGray"			    { yylval = 0xBEBEBE; return COLOR; }
"DarkBlue"			    { yylval = 0x0839A1; return COLOR; }
"DarkGreen"			    { yylval = 0x8CD01E; return COLOR; }
"DarkPurple"			{ yylval = 0x7300AB; return COLOR; }
"DarkRed"			    { yylval = 0xBA0000; return COLOR; }
"DarkOrange"			{ yylval = 0xff5100; return COLOR; }
"DarkYellow"			{ yylval = 0xFFF219; return COLOR; }
"DarkGray"			    { yylval = 0x505050; return COLOR; }

.                       {yyerror("Illegal character");}

