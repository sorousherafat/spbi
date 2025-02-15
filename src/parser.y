%{
#include <cc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bill.h"
#include "log.h"
#include "math.h"

extern int yylex();
extern int yyparse();
extern int yyerror(const char *str);

extern FILE *yyin;

void change_balance(char *id, int change);

static balance_map_t *parsed_balances;
%}

%union {
    int ival;
    char *sval;
    set(char *) *sset;
}

%token <ival> NUMBER
%token <sval> IDENTIFIER
%token COUNT
%token PLUSEQ COLON PLUS MINUS MULT DIV EXP LPAREN RPAREN ENDL

%left PLUS MINUS
%left MULT DIV
%right EXP

%type <ival> expression
%type <sset> identifier_list

%%

spbi:
    statements
    ;
statements:
    statements statement
    | statement
    ;
statement:
    ENDL
    | IDENTIFIER PLUSEQ expression COLON identifier_list ENDL {
        change_balance($1, $3);
        free($1);
        int share = $3 / size($5);
        for_each($5, id) {
            change_balance(*id, -share);
            free(*id);
        }
        cleanup($5);
        free($5);
    }
    | error ENDL { yyerrok; }
    ;
expression:
    NUMBER { $$ = $1; }
    | expression PLUS expression { $$ = $1 + $3; }
    | expression MINUS expression { $$ = $1 - $3; }
    | expression MULT expression { $$ = $1 * $3; }
    | expression DIV expression { $$ = $1 / $3; }
    | expression EXP expression { $$ = powi($1, $3); }
    | LPAREN expression RPAREN { $$ = $2; }
    | COUNT LPAREN identifier_list RPAREN {
        $$ = size($3);
        for_each($3, id) {
            free(*id);
        }
        cleanup($3);
        free($3);
    }
identifier_list:
    IDENTIFIER {
        $$ = malloc(sizeof(set(char *)));
        init($$);
        insert($$, $1);
    }
    | identifier_list PLUS IDENTIFIER {
        insert($$, $3);
    }
    | identifier_list MINUS IDENTIFIER {
        char **existing = get($$, $3);
        if (existing) {
            erase($$, $3);
            free(*existing);
        }
        free($3);
    }
    ;

%%

void parse_balances(balance_map_t *balances, char *path) {
    char command[strlen("cpp -P ") + strlen(path) + 1];
    snprintf(command, sizeof(command), "cpp -P %s", path);
    yyin = popen(command, "r");
    if (!yyin) {
        error("cannot open file");
    }
    parsed_balances = balances;
    yyparse();
    pclose(yyin);
}

int yyerror(const char *str) {
    error(str);
    return 1;
}

void change_balance(char *id, int change) {
    amount_t *amount = get(parsed_balances, id);
    if (amount == NULL) 
        insert(parsed_balances, strdup(id), change);
    else *amount += change;
}
