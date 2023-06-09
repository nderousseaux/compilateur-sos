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
    Ctrl_ql * ctrl_ql; // Liste de quads de contrôle
    Op_list * op_list; // Liste d'opérandes
    char * str; // Chaîne de caractères
    Operand * operand; // Opérande
    Operator operator; // Type d'opérateur
    int integer; // Entier
    Ctrl_for * ctrl_for; // Liste de quads de contrôle pour la boucle for
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
%token QMARK

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

%type <quad_list> liste-instructions instruction else-part N; // Ces noeuds sont des listes de quads
%type <ctrl_ql> test-instruction test-expr test-expr2 test-expr3 test-block; // Ces noeuds sont des listes de quads de contrôle
%type <op_list> liste-operandes; // Ces noeuds sont des listes d'opérandes
%type <operand> operande operande-entier somme-entiere produit-entier concatenation; // Ces noeuds sont des opérandes
%type <operator> fois-div-mod plus-ou-moins operateur1 operateur2// Ces noeuds sont des opérations
%type <integer> M; // Ces noeuds sont des entiers
%type <str> id;
%type <ctrl_for> start-for;


// Priorités
%left PLUS MINUS
%left STAR SLASH PERCENT

%%
programme           : liste-instructions                                                { }
            
liste-instructions  : liste-instructions SEMICOLON instruction                          { }
                    | instruction                                                       { }

id                  : MOT                                                               { $$ = copy_string($1); }

instruction         : id EQUAL concatenation                                            { gencode_assign($1, $3); }
                    | id OSQUARE operande-entier CSQUARE EQUAL concatenation            { gencode_assign_tab($1, $3, $6); }
                    | DECLARE id OSQUARE MOT CSQUARE                                    { gencode_decla_tab($2, $4); }
                    | IF test-block THEN M liste-instructions N else-part FI            { gencode_if($2, $4, $6, $7); }
                    | FOR id DO liste-instructions DONE                                 { /*TODO*/ }
                    | start-for DO liste-instructions DONE                              { gencode_for($1); }
                    | WHILE M test-block DO M liste-instructions DONE                   { $$ = gencode_while($3, $2, $5); }
                    | UNTIL M test-block DO M liste-instructions DONE                   { $$ = gencode_until($3, $2, $5); }
                    | CASE operande IN liste-case ESAC                                  { /*TODO*/ }
                    | ECHO_T liste-operandes                                            { gencode_echo($2); }
                    | READ id                                                           { /*TODO*/ }
                    | declaration-fonction                                              { /*TODO*/ }
                    | appel-fonction                                                    { /*TODO*/ }
                    | RETURN                                                            { /*TODO*/ }
                    | RETURN operande                                                   { /*TODO*/ }
                    | EXIT                                                              { gencode_exit(0); }
                    | EXIT operande-entier                                              { gencode_exit($2); }

start-for           : FOR id IN liste-operandes                                         { $$ = gencode_start_for($2, $4); }

else-part           : ELIF test-block THEN M liste-instructions N else-part             { $$ = gencode_elif($2, $4, $6, $7); }
                    | ELSE liste-instructions                                           { $$ = init_quad_list(); }
                    | /* empty */                                                       { $$ = init_quad_list(); }

liste-case          : liste-case filtre CPARA liste-instructions SEMICOLON SEMICOLON    { /*TODO*/ }
                    | filtre CPARA liste-instructions SEMICOLON SEMICOLON               { /*TODO*/ }

filtre              : MOT                                                               { /*TODO*/ }
                    | CHAINE                                                            { /*TODO*/ }
                    | filtre PIPE MOT                                                   { /*TODO*/ }
                    | filtre PIPE CHAINE                                                { /*TODO*/ }
                    | STAR                                                              { /*TODO*/ }

liste-operandes     : liste-operandes operande                                          { $$ = add_op($1, $2); }
                    | operande                                                          { $$ = create_list_op($1); }
                    | STAR OBRACE id OSQUARE STAR CSQUARE CBRACE                        { /*TODO*/ }

concatenation       : concatenation operande                                            { $$ = gencode_concat($1, $2); }
                    | operande                                                          { $$ = copy_operand($1); }
                    | DOLLAR OBRACE id OSQUARE operande-entier CSQUARE CBRACE           { $$ = gencode_tab_to_temp($3, $5); } //TODO supprimer

test-block		    : TEST test-expr                                                    { $$ = $2; }

test-expr			: test-expr OR_COMP M test-expr2                                    { gencode_or($1, $4, $3, $$); }
                    | test-expr2                                                        { $$ = $1;}

test-expr2          : test-expr2 AND_COMP M test-expr3                                  { gencode_and($1, $4, $3, $$); }
                    | test-expr3                                                        { $$ = $1;}
                
test-expr3          : EXCLA test-expr3                                                  { gencode_not($2, $$); }
                    | EXCLA OPARA test-expr3 CPARA                                      { gencode_not($3, $$); }
                    | OPARA test-expr3 CPARA                                            { $$ = $2;}
                    | test-instruction                                                  { $$ = $1; }

test-instruction	: operateur1 concatenation                                          { $$ = gencode_test($1, $2, NULL); }
                    | operande-entier operateur2 operande-entier                        { $$ = gencode_test($2, $1, $3); }
                    | concatenation EQUAL concatenation                                 { $$ = gencode_test(OP_EQUAL, $1, $3); }
                    | concatenation NEQUAL concatenation                                { $$ = gencode_test(OP_NEQUAL, $1, $3); }

operande            : DOLLAR OBRACE id CBRACE                                           { to_operand_id($$, $3, 0); }
                    | DOLLAR OBRACE id OSQUARE operande-entier CSQUARE CBRACE           { $$ = gencode_tab_to_temp($3, $5); }
                    | MOT                                                               { to_operand_const($$, $1); }
                    | DOLLAR MOT                                                        { /*TODO*/ }
                    | DOLLAR STAR                                                       { /*TODO*/ }
                    | DOLLAR QMARK                                                      { /*TODO*/ }
                    | CHAINE                                                            { to_operand_const($$, $1); }
                    | DOLLAR OPARA EXPR somme-entiere CPARA                             { $$ = $4;}
                    | DOLLAR OPARA appel-fonction CPARA                                 { /*TODO*/ }                                                  

operateur1          : EMPTY_COMP                                                        { $$ = OP_EMPTY; }
                    | NOEMPTY_COMP                                                      { $$ = OP_NOTEMPTY; }

operateur2			: EQUAL_COMP     									                { $$ = OP_EQUAL; }
					| NEQUAL_COMP											            { $$ = OP_NEQUAL; }
					| STSUP_COMP											            { $$ = OP_STSUP; }
					| SUPEQ_COMP											            { $$ = OP_SUPEQ; }
					| STINF_COMP											            { $$ = OP_STINF; }
					| INFEQ_COMP											            { $$ = OP_INFEQ; }

somme-entiere		: somme-entiere plus-ou-moins produit-entier                        { $$ = gencode_operation($2, $1, $3); }
                    | produit-entier                                                    { $$ = $1; }
                
produit-entier      : produit-entier fois-div-mod operande-entier                       { $$ = gencode_operation($2, $1, $3); }
                    | operande-entier                                                   { $$ = $1; }

operande-entier     : DOLLAR OBRACE id CBRACE                                           { to_operand_id($$, $3, 1); }
                    | DOLLAR OBRACE id OSQUARE operande-entier CSQUARE CBRACE           { $$ = gencode_tab_to_temp($3, $5); }
                    | DOLLAR MOT                                                        { /*TODO*/ }
                    | plus-ou-moins operande-entier                                     { $$ = gencode_operation($1, NULL, $2); }
                    | MOT                                                               { to_operand_int($$, $1); }
                    | OPARA somme-entiere CPARA                                         { $$ = $2; }
                    | DOLLAR OPARA EXPR somme-entiere CPARA                             { $$ = $4; }           

plus-ou-moins       : PLUS                                                              { $$ = OP_ADD; }
                    | MINUS                                                             { $$ = OP_MINUS; }

fois-div-mod        : STAR                                                              { $$ = OP_MULT; }
                    | SLASH                                                             { $$ = OP_DIV; }
                    | PERCENT                                                           { $$ = OP_MOD; }


declaration-fonction: id OPARA CPARA OBRACE decl-loc liste-instructions CBRACE          { /*TODO*/ }

decl-loc            : decl-loc LOCAL id EQUAL concatenation SEMICOLON                   { /*TODO*/ }
                    | /*Empty*/                                                         { /*TODO*/ }

appel-fonction      : id liste-operandes                                                { /*TODO*/ }
                    | id                                                                { /*TODO*/ }

M                   : /*Empty*/                                                         { $$ = nextquad(); }

N                   : /*Empty*/                                                         { $$ = init_goto();}

%%

void yyerror(const char * error) {
    fprintf(stderr, "%s\n", error);
}
