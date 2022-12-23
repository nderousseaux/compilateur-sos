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
    Operateur_y operateur;
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
%token IF
%token FI
%token ELSE
%token THEN
%token TEST
%token EQ

%token <operand> IDENTIFIER
%token <operand> MOT
%token <operand> CHAINE


//Types des noeuds
%type <operand> concatenation
%type <operand> operande
%type <operand> liste-operandes
%type <integer> operande-entier
%type <operateur> operateur2

%%
programme           : liste-instructions                                {  }
            
liste-instructions  : liste-instructions SEMICOLON instruction          { }
                    | instruction										{ }
                    
instruction         : IDENTIFIER EQUAL concatenation        			{ quad_assign($3.str, $1.str, $3.type); }
                    | ECHO_T liste-operandes                            { op_all_operand(&$2, OP_ECHO); }
                    | EXIT                                              { quad_exit(0); }
					| EXIT operande-entier                              { quad_exit($2); }
                    | test-block                                        {  }

liste-operandes     : liste-operandes operande                          { $$ = *add_operand(&$1, &$2); }
                    | operande                                          { $$ = $1; }

concatenation		: operande						        			{ $$ = $1; }

operande            : MOT                                               { $$ = $1; $$.type = O_INT;}
                    | CHAINE                                            { $$.str = create_const($1.str); $$.type = O_VAR; }
                    | DOLLAR OBRACE IDENTIFIER CBRACE                   { $$.str = $3.str; $$.type = O_ID; }


operande-entier     : MOT                                               { $$ = to_int($1.str); }

test-block		    : TEST test-expr                                    { }

test-expr			: test-instruction                                  { }

test-instruction	: operande operateur2 operande						{ if($2.type == O_EQUAL){
                                                                            quad_equal($1,$3);
                                                                            } }

operateur2			: EQ										        { $$.type = O_EQUAL; }


%%


void yyerror(const char * error)
{
    fprintf(stderr, "%s\n", error);
}
