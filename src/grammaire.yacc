%{
    #include <stdio.h>
    extern int yylex();
    void yyerror(const char * error);
%}

%start programme //TODO : Les tokens ici ?? Plutot dans un fichier spécifique
%token ENTIER
%token EXIT

%%
programme           : %empty
                    | liste-instructions
            
liste-instructions  : instruction
                    | liste-instructions instruction

instruction         : EXIT operande-entier              { printf("exit %d\n", $2); }

operande-entier     : ENTIER                            { $$ = $1; }
%%


void yyerror(const char * error)
{
    fprintf(stderr, "Erreur yacc: %s\n", error);
}
