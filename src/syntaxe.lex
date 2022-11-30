%option nounput
%option noinput
%option noyywrap

%{
    #include <stdio.h>
    #include "grammaire.tab.h"
%}

integer                 [0-9]+

%%

{integer}                                           {  
                                                        yylval.integer=atoi(yytext);
                                                        return INTEGER;
                                                    }

exit                                                { return EXIT; }

#(.*)\n                                             {} //Commentaires

[[:space:]]                                         {}



. fprintf(stderr, "Erreur lex: %s\n", yytext);

%%