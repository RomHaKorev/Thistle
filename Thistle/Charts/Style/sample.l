%{
#include "cssparser.h"
#define WEEK (7 * DAY)
#define DAY (24 * HOUR)
#define HOUR (60 * MINUTE)
#define MINUTE (60 * SECOND)
#define SECOND 1
#define INCH (25.4 * MM)
#define CM (10 * MM)
#define MM 1
#define PICA (12 * INCH/72 * MM)
#define POINT (INCH/72 * MM)

int yywrap(void)
{ return 1;}

%}

%a 3000
%o 4000

urlchar         [a-zA-Z0-9:/_%~!@#$?*+{};.,|=`'-]
d               [0-9]
notnm           [^a-zA-Z0-9-]
nmchar          [a-zA-Z0-9-]|\\\.
nmstrt          [a-zA-Z]
w               [ \t\n]*
num             {d}+|{d}*\.{d}+
h               [0-9a-fA-F]
hexcolor        #{h}{h}{h}|#{h}{h}{h}{h}{h}{h}|#{h}{h}{h}{h}{h}{h}{h}{h}{h}
ident           {nmstrt}{nmchar}*

%x COMMENT

%%

"--"                    {BEGIN(COMMENT);}

<COMMENT>"--"           {BEGIN(INITIAL);}
<COMMENT>\n             {/* ignore */}
<COMMENT>.              {/* ignore */}

@import                 return IMPORT;
@archform               return ARCHFORM;
@define                 return DEFINE;
"!"{w}legal             {return LEGAL;}
"!"{w}important         {return IMPORTANT;}
"$"{ident}              {return ENVVAR;}
{hexcolor}              {return HEXCOLOR;}
{ident}                 {return IDENT;}
{num}                   {yylval = atof(yytext); return NUMBER;}
{num}{w}"%"             {yylval = atof(yytext); return PERCENTAGE;}
{num}{w}pt/{notnm}      {yylval = atof(yytext) * POINT; return LENGTH;}
{num}{w}mm/{notnm}      {yylval = atof(yytext); return LENGTH;}
{num}{w}cm/{notnm}      {yylval = atof(yytext) * CM; return LENGTH;}
{num}{w}pc/{notnm}      {yylval = atof(yytext) * PICA; return LENGTH;}
{num}{w}inch/{notnm}    {yylval = atof(yytext) * INCH; return LENGTH;}
{num}{w}px/{notnm}      {yylval = atof(yytext); return LENGTH;}
{num}{w}em/{notnm}      {yylval = atof(yytext); return EMS;}
{num}{w}ch/{notnm}      {yylval = atof(yytext); return NCHARS;}
{num}{w}d/{notnm}       |
{num}{w}day/{notnm}     |
{num}{w}days/{notnm}    {yylval = atof(yytext) * DAY; return NUMBER;}
{num}{w}w/{notnm}       |
{num}{w}week/{notnm}    |
{num}{w}weeks/{notnm}   {yylval = atof(yytext) * WEEK; return NUMBER;}
{num}{w}h/{notnm}       |
{num}{w}hour/{notnm}    |
{num}{w}hours/{notnm}   {yylval = atof(yytext) * HOUR; return NUMBER;}
{num}{w}m/{notnm}       |
{num}{w}min/{notnm}     |
{num}{w}minute/{notnm}  |
{num}{w}minutes/{notnm} {yylval = atof(yytext) * MINUTE; return NUMBER;}
{num}{w}s/{notnm}       |
{num}{w}sec/{notnm}     |
{num}{w}second/{notnm}  |
{num}{w}seconds/{notnm} {yylval = atof(yytext) * SECOND; return NUMBER;}

"<<"                    return INTERPOLATELO;
">>"                    return INTERPOLATEHI;
"+:"                    return PLUS_EQ;
"-:"                    return MINUS_EQ;
"*:"                    return STAR_EQ;
"/:"                    return SLASH_EQ;
":"                     return ':';
"."                     return '.';
"/"                     return '/';
"*"                     return '*';
"+"                     return '+';
"-"                     return '-';
"="                     return '=';
"("                     return '(';
")"                     return ')';
"{"                     return '{';
"}"                     return '}';
"["                     return '[';
"]"                     return ']';
";"                     return ';';
"&"                     return '&';
","                     return ',';
[ \t]+                  {/* ignore whitespace */}
\n                      {/* ignore whitespace */}

.                       {yyerror("Illegal character");}
