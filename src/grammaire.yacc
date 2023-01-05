/* Fichier de définition de la grammaire
    Pour des raisons de lisibilité, les actions associées aux règles sont
    définies dans le fichier intermediate/actions.c
*/

%{
    #include "imports.h"
    extern int yylex();
    void yyerror(const char * error);
%}

%code requires {
    #include "imports.h"
}

// Type des noeuds
%union {
    Ql * quad_list; // Liste de quads
    Op_list * op_list; // Liste d'opérandes
    char * str; // Chaîne de caractères
    Operand * operand; // Opérande
    Operator operator; // Type d'opérateur
}

%start programme

// Liste des tokens
%token IF		// Mots réservés
%token THEN
%token FOR
%token DO
%token DONE
%token IN
%token UNTIL
%token WHILE
%token CASE
%token ESAC
%token ECHO_T
%token READ
%token RETURN
%token EXIT
%token LOCAL
%token ELIF
%token ELSE
%token FI
%token DECLARE
%token TEST
%token EXPR

%token SEMICOLON  // Caractères spéciaux
%token OSQUARE 
%token CSQUARE 
%token OBRACE 
%token CBRACE 
%token PIPE 
%token OPARA 
%token CPARA 
%token EQUAL 
%token NEQUAL 
%token EXCLA 
%token AND 
%token STAR 
%token DQUOTE 
%token QUOTE 
%token DOLLAR 

%token PLUS		// Opérateurs
%token MINUS
%token SLASH
%token PERCENT
%token INF
%token SUP
%token INFEQ
%token SUPEQ
%token ANDAND
%token PIPEPIPE

%token EQUAL_COMP	// Comparateurs
%token NEQUAL_COMP
%token STINF_COMP
%token INFEQ_COMP
%token STSUP_COMP
%token SUPEQ_COMP
%token NOEMPTY_COMP
%token EMPTY_COMP
%token OR_COMP
%token AND_COMP

%token <str> IDENTIFIER
%token <str> MOT
%token <str> CHAINE

%type <quad_list> liste-instructions instruction; // Ces noeuds sont des listes de quads
%type <op_list> liste-operandes concatenation; // Ces noeuds sont des listes d'opérandes
%type <operand> operande operande-entier somme-entiere produit-entier; // Ces noeuds sont des opérandes
%type <operator> fois-div-mod plus-ou-moins // Ces noeuds sont des opérations

// Priorités
%left PLUS MINUS
%left STAR SLASH PERCENT

%%
programme           : liste-instructions                                                { }
            
liste-instructions  : liste-instructions SEMICOLON instruction                          { }
                    | instruction                                                       { }

instruction         : IDENTIFIER EQUAL concatenation                                    { gencode_assign($1, $3); }
                    | EXIT                                                              { gencode_exit(0); }
                    | EXIT operande-entier                                              { gencode_exit($2); }
                    | ECHO_T liste-operandes                                            { gencode_echo($2); }


liste-operandes     : liste-operandes operande                                          { $$ = add_op($1, $2); }
                    | operande                                                          { $$ = create_list_op($1); }


concatenation       : operande                                                          { $$ = create_list_op($1); }

operande            : MOT                                                               { to_operand_int($$, $1); }
                    | CHAINE                                                            { to_operand_const($$, $1); }
                    | DOLLAR OBRACE IDENTIFIER CBRACE                                   { to_operand_id($$, $3); }
                    | DOLLAR OPARA EXPR somme-entiere CPARA                             { $$ = $4;}

operande-entier     : MOT                                                               { to_operand_int($$, $1); }
                    | OPARA somme-entiere CPARA                                         { $$ = $2; }

somme-entiere		: somme-entiere plus-ou-moins produit-entier                        { $$ = gencode_operation($2, $1, $3); }
                    | produit-entier                                                    { $$ = $1; }
                
produit-entier      : produit-entier fois-div-mod operande-entier                       { $$ = gencode_operation($2, $1, $3); }
                    | operande-entier                                                   { $$ = $1; }

test-instruction	: operande operateur2 operande						{
                                                                            $$.tru = creelist(-1);
                                                                            $$.fals = creelist(-1);
                                                                            $$.next = creelist(-1);
                                                                            add_idx_quad($$.tru, nextquad);
                                                                            switch($2.type){
                                                                                case O_EQUAL:
                                                                                    quad_equal($1, $3, -1);
                                                                                    break;
                                                                                case O_NEQUAL:
                                                                                    quad_nequal($1, $3, -1);
                                                                                    break;
                                                                                case O_STSUP:
                                                                                    quad_stsup($1, $3, -1);
                                                                                    break;
                                                                                case O_SUPEQ:
                                                                                    quad_supeq($1, $3, -1);
                                                                                    break;
                                                                                case O_STINF:
                                                                                    quad_stinf($1, $3, -1);
                                                                                    break;
                                                                                case O_INFEQ:
                                                                                    quad_infeq($1, $3, -1);
                                                                                    break;
                                                                                default:
                                                                                    break;
                                                                            }
                                                                            quad_goto(-1);
                                                                            add_idx_quad($$.fals, nextquad-1);
                                                                        }
                    | operateur1 concatenation                              { }

operateur1			: NOEMPTY_COMP											{ $$.type = O_NOTEMPTY; }
					| EMPTY_COMP											{ $$.type = O_EMPTY; }

operateur2			: EQUAL_COMP     									    { $$.type = O_EQUAL; }
					| NEQUAL_COMP											{ $$.type = O_NEQUAL;}
					| STSUP_COMP											{ $$.type = O_STSUP; }
					| SUPEQ_COMP											{ $$.type = O_SUPEQ; }
					| STINF_COMP											{ $$.type = O_STINF; }
					| INFEQ_COMP											{ $$.type = O_INFEQ; }

M                   : /*Empty*/                                             { $$ = nextquad;}

N                   : /*Empty*/                                             { 
                                                                              $$.next = creelist(nextquad);
                                                                              quad_goto(-1);
                                                                              $$.idx = nextquad;
                                                                            }
%%

void yyerror(const char * error) {
    fprintf(stderr, "%s\n", error);
}
