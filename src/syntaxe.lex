%option nounput
%option noyywrap

%{
    #include <stdio.h>
    #include "grammaire.tab.h"
%}

%%

[0-9]+                      { return ENTIER; }
exit                        { return EXIT; }

. fprintf(stderr, "Erreur lex: %s\n", yytext);

%%