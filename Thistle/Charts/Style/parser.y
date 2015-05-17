%{
#include "cssparser_chart.h"
int yyparse();
%}


/*
   There are two shift/reduce conflicts in more_terms,
   because op can be empty
 */

%error-verbose

%token THISTLE
%token SERIE_ID
%token IDENT
%token PEN
%token INTEGER
%token UNIT
%token SERIECHART_TYPE

%token COLOR
%token RGB

%token PEN_STYLE
%token PEN_CAPSTYLE
%token PEN_JOINSTYLE
%%

stylesheet_serieChart
: def rules
;

def
: THISTLE SERIECHART_TYPE ':' SERIE_ID	{ SerieFormatDigester::instance->selectSerie( yylval ); }
| THISTLE SERIECHART_TYPE
;


rules
: '{' declaration_seq '}'
;

declaration_seq
: declaration declaration_seq
| declaration
;

declaration
: pen_prop
;

pen_prop
: PEN ':' pen_value ';'
: 'pen-style' ':' ';'
;

pen_value
: brush
| brush pen_size
| brush pen_size PEN_STYLE
| brush pen_size PEN_STYLE PEN_CAPSTYLE
| brush pen_size PEN_STYLE PEN_CAPSTYLE PEN_JOINSTYLE
;

brush
: COLOR
| rgb_color
;

rgb_color
: '#' RGB
;

pen_size
: INTEGER UNIT { SerieFormatDigester::instance->currentPen().setWidth( yylval ); }
;



%%

 int main( void )
{
	yyparse();
	return 0;
}
