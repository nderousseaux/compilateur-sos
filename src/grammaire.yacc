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
    Ql * ql; // Liste de quads
    int integer; // Entier
    char * str; // Chaîne de caractères
    Operand * operand; // Opérande
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

%type <ql> liste-instructions instruction; // Les noeuds contiennent des listes de quads
%type <integer> operande-entier
%type <operand> operande concatenation liste-operandes


%%
programme           : liste-instructions                                                { }
            
liste-instructions  : liste-instructions SEMICOLON instruction                          { $$ = create_list(); }
                    | instruction                                                       { $$ = create_list(); } //FIXME :vraiment necessaire ?

instruction         : IDENTIFIER EQUAL concatenation                                    { gencode_assign($1, $3); }
                    | EXIT                                                              { gencode_exit(0); }
                    | EXIT operande-entier                                              { gencode_exit($2); }
                    | ECHO_T liste-operandes                                            { gencode_echo($2); }


liste-operandes     : operande                                                          { $$ = $1; }


concatenation       : operande                                                          { $$ = $1; }

operande            : MOT                                                               { to_operand_int($$, $1); }
                    | CHAINE                                                            { to_operand_const($$, $1); }
                    | DOLLAR OBRACE IDENTIFIER CBRACE                                   { to_operand_id($$, $3); }

operande-entier     : MOT                                                               { $$ = to_int($1); }
%%

void yyerror(const char * error) {
    fprintf(stderr, "%s\n", error);
}
