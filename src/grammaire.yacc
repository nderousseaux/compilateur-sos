%{
    #include <stdio.h>
    #include "intermediate.h"
    #include "symbols.h"
    extern int yylex();
    void yyerror(const char * error);
%}

%code requires {
    #include "intermediate.h"
    #include "operand.h"
}

// Type des noeuds
%union {
    Operand_y operand;
    int integer;
}

%start programme

// Liste des tokens
// Mots clés
%token ECHO_T
%token EXIT

// Caractères spéciaux
%token SEMICOLON
%token OBRACE
%token CBRACE
%token EQUAL
%token DQUOTE
%token QUOTE
%token DOLLAR

%token <operand> IDENTIFIER
%token <operand> MOT
%token <operand> CHAINE


//Types des noeuds
%type <operand> concatenation
%type <operand> operande
%type <operand> liste-operandes
%type <integer> operande-entier

%%
programme           : liste-instructions                                {  }
            
liste-instructions  : liste-instructions SEMICOLON instruction          { }
                    | instruction										{ }
                    
instruction         : IDENTIFIER EQUAL concatenation        			{ quad_assign($3.str, $1.str, $3.type); }
                    | ECHO_T liste-operandes                            { op_all_operand(&$2, OP_ECHO); }
                    | EXIT                                              { quad_exit(0); }
					| EXIT operande-entier                              { quad_exit($2); }

liste-operandes     : liste-operandes operande                          { $$ = *add_operand(&$1, &$2); }
                    | operande                                          { $$ = $1; }

concatenation		: operande						        			{ $$ = $1; }

operande            : MOT                                               { $$ = $1; $$.type = O_INT;}
                    | CHAINE                                            { $$.str = create_const($1.str); $$.type = O_VAR; }
                    | DOLLAR OBRACE IDENTIFIER CBRACE                   { $$.str = $3.str; $$.type = O_ID; }


operande-entier     : MOT                                               { $$ = to_int($1.str); }
%%


void yyerror(const char * error)
{
    fprintf(stderr, "%s\n", error);
}
