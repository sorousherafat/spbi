%{
    #include <cc.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #include "parser.h"

    extern int yylex();
    extern void yyerror(const char *str);
%}

%option noyywrap
%option noinput
%option nounput

%%

[ \t]+                 ;
"count"                { return COUNT; }
"+="                   { return PLUSEQ; }
":"                    { return COLON; }
"+"                    { return PLUS; }
"-"                    { return MINUS; }
"*"                    { return MULT; }
"/"                    { return DIV; }
"^"                    { return EXP; }
"("                    { return LPAREN; }
")"                    { return RPAREN; }
"\n"                   { return ENDL; }
[0-9]+                 { yylval.ival = atoi(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.sval = strdup(yytext); return IDENTIFIER; }
.                      { yyerror("unexpected character"); }

%%
