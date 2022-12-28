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

%token EQUAL_COMP
%token NEQUAL_COMP
%token STSUP_COMP
%token SUPEQ_COMP
%token STINF_COMP
%token INFEQ_COMP
%token NOEMPTY_COMP
%token EMPTY_COMP

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
                    |

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
                                                                            $$.tru = creelist(-1);
                                                                            $$.fals = creelist(-1);
                                                                            $$.next = creelist(-1);
                                                                            switch($2.type){
                                                                                case O_EQUAL:
                                                                                    quad_equal($1, $3, nextquad+2);
                                                                                    break;
                                                                                case O_NEQUAL:
                                                                                    quad_equal($1, $3, nextquad+2);
                                                                                    break;
                                                                                case O_STSUP:
                                                                                    quad_equal($1, $3, nextquad+2);
                                                                                    break;
                                                                                case O_SUPEQ:
                                                                                    quad_equal($1, $3, nextquad+2);
                                                                                    break;
                                                                                case O_STINF:
                                                                                    quad_equal($1, $3, nextquad+2);
                                                                                    break;
                                                                                case O_INFEQ:
                                                                                    quad_equal($1, $3, nextquad+2);
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


