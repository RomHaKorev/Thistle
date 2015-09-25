%{
#include <stdlib.h>
#include <stdio.h>
#define YYSTYPE int
int yyparse();
int yylex();
int yyerror(const char *s);
%}

%expect 2
/*
   There are two shift/reduce conflicts in more_terms,
   because op can be empty
 */
%error-verbose

%token IMPORT DEFINE ARCHFORM IMPORTANT LEGAL
%token INTERPOLATEHI INTERPOLATELO PLUS_EQ MINUS_EQ STAR_EQ SLASH_EQ
%token STRING HEXCOLOR
%token IDENT ENVVAR
%token NUMBER LENGTH EMS NCHARS PERCENTAGE

%%

stylesheet
 : imports defs rules
 ;
imports
 : imports IMPORT url
 | /* empty */
 ;
url
 : STRING
 ;
defs
 : defs constant_def
 | defs archform_def
 | /* empty */
 ;
constant_def
 : DEFINE unique_id '=' value ';'
 ;
unique_id
 : IDENT
 ;
archform_def
 : ARCHFORM attrname
 ;
attrname
 : IDENT
 ;
rules
 : rule_seq media_rules
 ;
media_rules
 : media_rules medium rule_seq
 | /* empty */
 ;
medium
 : ':' IDENT other_media ':'
 ;
other_media
 : other_media '&' IDENT
 | /* empty */
 ;
rule_seq
 : rule_seq rule
 | /* empty */
 ;
rule
 : selector_seq '{' declaration_seq '}'
 ;
selector_seq
 : selector_seq ',' selector
 | selector
 ;
selector
 : toplevel
 | ancestors predecessor_opt elemspec
 ;
toplevel
 : '*'
 ;
ancestors
 : ancestors predecessor_opt ancestor
 | /* empty */
 ;
ancestor
 : '(' elemspec ')'
 ;
predecessor_opt
 : predecessor
 | /* empty */
 ;
predecessor
 : '/' elemname '/'
 ;
elemname
 : IDENT
 | IDENT '.' IDENT
 ;
elemspec
 : elemname attrspec_seq
 | attrspec_seq
 ;
attrspec_seq
 : attrspec_seq attrspec
 | /* empty */
 ;
attrspec
 : '[' attrname constval_opt ']'
 ;
constval_opt
 : '=' constval
 | /* empty */
 ;
constval
 : NUMBER
 | STRING
 | IDENT
 ;
declaration_seq
 : declaration_seq ',' declaration
 | declaration
 ;
declaration
 : property ':' value priority_opt
 | property op_eq value priority_opt
 ;
priority_opt
 : IMPORTANT
 | LEGAL STRING
 | /* empty */
 ;
property
 : IDENT
 ;

/*
 * Operator precedence: high to low
 * 5:  + -      (monadic plus, monadic minus)
 * 4:  * / %    (multiply, divide, remainder)
 * 3:  + -      (plus, minus)
 * 2:  << >>    (interpolate low, interpolate high)
 * 1:           ([=space] concatenate)
 */
value
 : value1 value
 | value1
 ;
value1
 : value2 INTERPOLATELO value1
 | value2 INTERPOLATEHI value1
 | value2
 ;
value2
 : value3 '+' value2
 | value3 '-' value2
 | value3
 ;
value3
 : value4 '*' value3
 | value4 '/' value3
 | value4 '%' value3
 | value4
 ;
value4
 : '+' term
 | '-' term
 | term
 ;
term
 : LENGTH
 | EMS
 | NCHARS
 | STRING
 | HEXCOLOR
 | NUMBER
 | PERCENTAGE
 | IDENT
 | attref
 | envref
 | '(' value ')'
 ;
attref
 : '[' attrname ']'
 ;
envref
 : ENVVAR
 ;
op_eq
 : PLUS_EQ
 | MINUS_EQ
 | STAR_EQ
 | SLASH_EQ
 ;

 %%

int yyerror (const char *s)
{                                                     
    printf("parser error %s \n ",s);
    return 0;
}

int main(void) {
    yyparse();
    return 0;
}