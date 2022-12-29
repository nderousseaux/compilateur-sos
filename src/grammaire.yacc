%{
    #include "imports.h"
    extern int yylex();
    void yyerror(const char * error);
    extern int nextquad;
%}

%code requires {
    #include "imports.h"
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
%token ELIF
%token OPARA
%token CPARA
%token EXCLA
%token DONE
%token WHILE
%token DO
%token UNTIL

%token EQUAL_COMP
%token NEQUAL_COMP
%token STSUP_COMP
%token SUPEQ_COMP
%token STINF_COMP
%token INFEQ_COMP
%token NOEMPTY_COMP
%token EMPTY_COMP
%token OR_COMP
%token AND_COMP

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
%type <booleen> N
%type <booleen> test-expr
%type <booleen> test-expr2
%type <booleen> test-expr3
%type <booleen> test-block
%type <booleen> instruction
%type <booleen> liste-instructions
%type <booleen> else-part

%%
programme           : liste-instructions                                { }
            
liste-instructions  : liste-instructions SEMICOLON instruction          { $$.next = creelist(-1);
                                                                          $3.next = creelist(-1);}
                    | instruction										{ $$.next = creelist(-1) ; }
                    
instruction         : IDENTIFIER EQUAL concatenation        			{ quad_assign($3.str, $1.str, $3.type); }
                    | EXIT                                              { quad_exit(0); }
					| EXIT operande-entier                              { quad_exit($2); }
                    | IF test-block THEN M liste-instructions N else-part FI        {
                                                                            $6.next = concat($7.next,$6.next);
                                                                            complete($6.next,nextquad);
                                                                            complete($2.fals, $6.idx);
                                                                            complete($2.tru, $4);                                                                            

                                                                        }
                    | WHILE M test-block DO M liste-instructions DONE   { quad_goto($2);
                                                                          complete($3.tru,$5);
                                                                          complete($3.fals,nextquad);
                                                                          $$.next = $3.fals;
                                                                        }
                    | UNTIL M test-block DO M liste-instructions DONE   { quad_goto($2);
                                                                          complete($3.fals,$5);
                                                                          complete($3.tru,nextquad);
                                                                          $$.next = $3.tru;                                                      
                                                                          }
                    | ECHO_T liste-operandes                            { op_all_operand(&$2, OP_ECHO); }

else-part           : ELIF test-block THEN M liste-instructions N else-part         {
                                                                            $$.next = creelist(-1);
                                                                            $$.next = concat($$.next,$7.next);
                                                                            $$.next = concat($$.next,$6.next); 
                                                                            complete($2.fals, $6.idx);
                                                                            complete($2.tru, $4);
                                                                                     }
					| ELSE liste-instructions                                       {
                                                                                    $$.next = creelist(-1);
                                                                                    }
                    |                                                               {$$.next = creelist(-1);}

liste-operandes     : liste-operandes operande                          { $$ = *add_operand(&$1, &$2); }
                    | operande                                          { $$ = $1; }

concatenation		: operande						        			{ $$ = $1; }

operande            : MOT                                               { $$ = $1; $$.type = O_INT;}
                    | CHAINE                                            { $$.str = create_const($1.str); $$.type = O_VAR; }
                    | DOLLAR OBRACE IDENTIFIER CBRACE                   { $$.str = $3.str; $$.type = O_ID; }


operande-entier     : MOT                                               { $$ = to_int($1.str); }

test-block		    : TEST test-expr                                    { $$ = $2;}

test-expr			: test-expr OR_COMP M test-expr2                    { complete($1.fals,$3);
                                                                          $$.fals = $4.fals;
                                                                          $$.tru = concat($1.tru,$4.tru);
                                                                        }
                    | test-expr2                                        { $$ = $1;}

test-expr2          : test-expr2 AND_COMP M test-expr3                  { complete($1.tru,$3);
                                                                          $$.fals = concat($1.fals,$4.fals);
                                                                          $$.tru = $4.tru;
                                                                        }
                    | test-expr3                                        { $$ = $1;}
                
test-expr3          : OPARA test-expr3 CPARA                            { $$=$2;}
                    | EXCLA OPARA test-expr3 CPARA                      { $$.tru = $3.fals;
                                                                          $$.fals = $3.tru;}
                    | test-instruction                                  { $$ = $1;}
                    | EXCLA test-instruction                            { $$.tru = $2.fals;
                                                                          $$.fals = $2.tru;
                                                                          }

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

operateur1			: NOEMPTY_COMP											{ $$.type = O_NOTEMPTY; }
					| EMPTY_COMP											{ $$.type = O_EMPTY; }

operateur2			: EQUAL_COMP     									    { $$.type = O_EQUAL; }
					| NEQUAL_COMP											{ $$.type = O_NEQUAL;}
					| STSUP_COMP											{ $$.type = O_STSUP; }
					| SUPEQ_COMP											{ $$.type = O_SUPEQ; }
					| STINF_COMP											{ $$.type = O_STINF; }
					| INFEQ_COMP											{ $$.type = O_INFEQ; }

M                   : /*Empty*/                                             { $$ = nextquad;}

N                   : /*Empty*/                                             { $$.next = creelist(nextquad);
                                                                              quad_goto(-1);
                                                                              $$.idx = nextquad;
                                                                            }
%%


void yyerror(const char * error)
{
    fprintf(stderr, "%s\n", error);
}

// #brouillax
// idx     
// 0       equal si vrai go 2 
// 1       goto next test 10        
// 2       bonjour

// 3       test egal goto5
// 4       goto 7
// 5       une phrase
// 6       goto test 10
// 7       goto empty
// 8       echo phrase 2
// 9       goto 10
// 10      exit


