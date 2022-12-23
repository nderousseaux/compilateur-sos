%option nounput
%option noinput
%option noyywrap

%{
    #include "imports.h"
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
do				        {return DO;}
done			        {return DONE;}
in				        {}
until			        {return UNTIL;}
while			        {return WHILE;}
case			        {}
esac			        {}
echo			        { return ECHO_T; }
read			        {}
return			        {}
exit			        { return EXIT; }
local			        {}
elif			        {return ELIF;}
else			        {return ELSE;}
fi				        {return FI;}
declare			        {}
test			        {return TEST;}
expr			        {return EXPR;}

;				        { return SEMICOLON; } //Caractères spéciaux
\[				        {}
\]				        {}
\{				        { return OBRACE; }
\}				        { return CBRACE;}
\|				        {}
\(				        { return OPARA;}
\)				        { return CPARA;}
=				        { return EQUAL; }
!=				        {}
!				        { return EXCLA;}
&				        {}
\*				        { return FOIS; }
\"				        { return DQUOTE; }
'				        { return QUOTE; }
\$				        { return DOLLAR; }

\+  			        { return PLUS;} // Opérateurs
\-				        { return MOINS;}
\/				        { return DIVISION;}
\%				        { return MODULO;}
\<				        {}
\>				        {}
\<=				        {}
\>=				        {}
\&\&			        {}
\|\|			        {}

-eq                     { return EQUAL_COMP;}
-ne                     { return NEQUAL_COMP;}
-lt                     { return STINF_COMP;}
-le                     { return INFEQ_COMP;}
-gt                     { return STSUP_COMP;}
-ge                     { return SUPEQ_COMP;}
-n                      { return NOEMPTY_COMP;}
-z                      { return EMPTY_COMP;}
-o                      { return OR_COMP;}
-a                      { return AND_COMP;}

{com}			        {  } // Commentaires

[{com} \n\t]	        {  } // Blancs


(\"[^"]*\")|(\'[^']*\') {   // Chaine
                            assign_string(yytext);
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