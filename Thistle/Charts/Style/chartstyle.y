%{
#include "serieformatdigester.h"
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


%token P_PENSTYLE
%token P_PENCOLOR

%token V_PEN_STYLE
%token V_PEN_CAPSTYLE
%token V_PEN_JOINSTYLE

%token P_SERIETYPE
%token V_SERIETYPE
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
| style_prop
;

pen_prop
: PEN ':' pen_value ';'
| P_PENSTYLE ':' value_pen_style ';'
| P_PENCOLOR ':' brush ';'
;

pen_value
: pen_color
| brush pen_size
| brush pen_size value_pen_style
| brush pen_size value_pen_style value_pen_capstyle
| brush pen_size value_pen_style value_pen_capstyle value_pen_joinstyle
;

pen_size
: INTEGER UNIT { SerieFormatDigester::instance->currentPen().setWidth( yylval ); }
;

value_pen_style
: V_PEN_STYLE { SerieFormatDigester::instance->currentPen().setStyle( static_cast<Qt::PenStyle>(yylval) ); }
;

value_pen_capstyle
: V_PEN_CAPSTYLE { SerieFormatDigester::instance->currentPen().setCapStyle( static_cast<Qt::PenCapStyle>(yylval) ); }
;

value_pen_joinstyle
: V_PEN_JOINSTYLE { SerieFormatDigester::instance->currentPen().setJoinStyle( static_cast<Qt::PenJoinStyle>(yylval) ); }
;

pen_color
: brush { SerieFormatDigester::instance->currentPen().setColor( yylval ); }
;


style_prop
: P_SERIETYPE ':' V_SERIETYPE ';' { SerieFormatDigester::instance->setType( static_cast<Thistle::Types>(yylval) ); }


brush
: COLOR
| rgb_color
;

rgb_color
: '#' RGB
;


%%

 int main( void )
{
	yyparse();
	return 0;
}
