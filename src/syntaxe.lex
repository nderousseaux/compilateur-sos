%option nounput
%option noinput
%option noyywrap

%{
    #include <stdio.h>
    #include "grammaire.tab.h"

    void assign_string(char *s) {
        char *t = calloc(255, sizeof(char));
        strcpy(t, s);
        yylval.operand.str=t;
    }
%}

com				#.*\n
mot				[^ \t\r\n=+*/%&|!<>;{}()\[\]\"\'\$]+
id              [a-zA-Z_][a-zA-Z0-9_]*


%%


if				        {return IF;} // Mots reservés
then			        {return THEN;}
for				        {}
do				        {}
done			        {}
in				        {}
until			        {}
while			        {}
case			        {}
esac			        {}
echo			        { return ECHO_T; }
read			        {}
return			        {}
exit			        { return EXIT; }
local			        {}
elif			        {}
else			        {return ELSE;}
fi				        {return FI;}
declare			        {}
test			        {return TEST;}
expr			        {}

;				        { return SEMICOLON; } //Caractères spéciaux
\[				        {}
\]				        {}
\{				        { return OBRACE; }
\}				        { return CBRACE;}
\|				        {}
\(				        {}
\)				        {}
=				        { return EQUAL; }
!=				        {}
!				        {}
&				        {}
\*				        {}
\"				        { return DQUOTE; }
'				        { return QUOTE; }
\$				        { return DOLLAR; }

\+				        {} // Opérateurs
\-				        {}
\/				        {}
\%				        {}
\<				        {}
\>				        {}
\<=				        {}
\>=				        {}
\&\&			        {}
\|\|			        {}

-eq                     { return EQUAL_COMP;}


{com}			        {  } // Commentaires

[{com} \n\t]	        {  } // Blancs


(\"[^"]*\")|(\'[^']*\') {   // Chaine
                            assign_string(yytext);
                            printf("%s\n", yytext);
                            return CHAINE;
                        }

{id}        			{   // ID
                            assign_string(yytext);
					        return IDENTIFIER;
                        }

{mot}                   {   // Mot
                            assign_string(yytext);
                            return MOT;
                        }



. fprintf(stderr, "Unexpected character: %s\n", yytext);
%%