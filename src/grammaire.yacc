%{
    #include <stdio.h>
    #include "intermediate.h"
    extern int yylex();
    void yyerror(const char * error);
%}

// Type des noeuds
%union {
    int integer;
}

%start programme

// Liste des tokens et leurs type
%token <integer> INTEGER
%token EXIT

%type <integer> operande-entier

%%
programme           : liste-instructions                {  }
            
liste-instructions  : instruction
                    
instruction         : EXIT                              { quad_exit(0);  }
                    | EXIT operande-entier              { quad_exit($2); }

operande-entier     : INTEGER                           { $$ = $1; }

%%


void yyerror(const char * error)
{
    fprintf(stderr, "%s\n", error);
}
