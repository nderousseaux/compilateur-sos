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
    Bool booleen;
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

%token <operand> IDENTIFIER
%token <operand> MOT
%token <operand> CHAINE


//Types des noeuds
%type <operand> concatenation
%type <operand> operande
%type <operand> liste-operandes
%type <integer> operande-entier
%type <operateur> operateur2
%type <operateur> operateur1
%type <booleen> test-instruction
%type <integer> M
%type <booleen> test-expr
%type <booleen> test-block
%type <booleen> instruction

%%
programme           : liste-instructions                                {  }
            
liste-instructions  : liste-instructions SEMICOLON instruction          { }
                    | instruction										{ }
                    
instruction         : IDENTIFIER EQUAL concatenation        			{ quad_assign($3.str, $1.str, $3.type); }
                    | ECHO_T liste-operandes                            { op_all_operand(&$2, OP_ECHO); }
                    | EXIT                                              { quad_exit(0); }
					| EXIT operande-entier                              { quad_exit($2); }
                    | IF test-block THEN M liste-instructions FI          { complete($2.tru,$4.quad);
                                                                            $$.next = creelist(NULL);
                                                                            $$.next = concat()
                                                                            }

liste-operandes     : liste-operandes operande                          { $$ = *add_operand(&$1, &$2); }
                    | operande                                          { $$ = $1; }

concatenation		: operande						        			{ $$ = $1; }

operande            : MOT                                               { $$ = $1; $$.type = O_INT;}
                    | CHAINE                                            { $$.str = create_const($1.str); $$.type = O_VAR; }
                    | DOLLAR OBRACE IDENTIFIER CBRACE                   { $$.str = $3.str; $$.type = O_ID; }


operande-entier     : MOT                                               { $$ = to_int($1.str); }

test-block		    : TEST test-expr                                    { $$ = $2;}

test-expr			: test-instruction                                  { $$ = $1;}

test-instruction	: operande operateur2 operande						{
                                                                            $$.tru = creelist(NULL);
                                                                            $$.fals = creelist(NULL);
                                                                            switch($2.type){
                                                                                case O_EQUAL:
                                                                                    break;
                                                                                case O_NEQUAL:
                                                                                    break;
                                                                                case O_STSUP:
                                                                                    break;
                                                                                case O_SUPEQ:
                                                                                    break;
                                                                                case O_STINF:
                                                                                    break;
                                                                                case O_INFEQ:
                                                                                    break;
                                                                                default:
                                                                            }
                                                                        }

operateur1			: '-' 'n'												{ $$.type = O_NOTEMPTY; }
					| '-' 'z'												{ $$.type = O_EMPTY; }

operateur2			: '-' 'e' 'q'									        { $$.type = O_EQUAL; }
					| '-' 'n' 'e'											{ $$.type = O_NEQUAL;}
					| '-' 'l' 't'											{ $$.type = O_STSUP; }
					| '-' 'l' 'e'											{ $$.type = O_SUPEQ; }
					| '-' 'g' 't'											{ $$.type = O_STINF; }
					| '-' 'g' 'e'											{ $$.type = O_INFEQ; }

M                   : /*Empty*/                                             { M.quad = nextquad;}
%%


void yyerror(const char * error)
{
    fprintf(stderr, "%s\n", error);
}
