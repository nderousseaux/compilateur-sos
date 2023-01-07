%option nounput
%option noinput
%option noyywrap

%{
    #include "imports.h"
    #include "grammaire.tab.h"
%}

com				#.*\n
blanc           [ \t\n{com}]
car             !|[#-&]|[\(-\[]|[\]-~]|(\\[\\'"tn])
mot             [^ \t\r\n=*/%&|!<>;{}()\[\]\"\'\$]+
chaine          (\"({car}|{blanc})*\")|(\'({car}|{blanc})*\')


%%


if				        { return IF; } // Mots reservés
then			        { return THEN; }
for				        { return FOR; }
do				        { return DO; }
done			        { return DONE; }
in				        { return IN; }
until			        { return UNTIL; }
while			        { return WHILE;}
case			        { return CASE; }
esac			        { return ESAC; }
echo			        { return ECHO_T; }
read			        { return READ; }
return			        { return RETURN; }
exit			        { return EXIT; }
local			        { return LOCAL; }
elif			        { return ELIF; }
else			        { return ELSE; }
fi				        { return FI; }
declare			        { return DECLARE; }
test			        { return TEST; }
expr			        { return EXPR; }

;				        { return SEMICOLON; } //Caractères spéciaux
\[				        { return OSQUARE; }
\]				        { return CSQUARE; }
\{				        { return OBRACE; }
\}				        { return CBRACE;}
\|				        { return PIPE; }
\(				        { return OPARA;}
\)				        { return CPARA;}
=				        { return EQUAL; }
!=				        { return NEQUAL; }
!				        { return EXCLA;}
&				        { return AND; }
\*				        { return STAR; }
\"				        { return DQUOTE; }
'				        { return QUOTE; }
\$				        { return DOLLAR; }

\+				        { return PLUS; } // Opérateurs
\-				        { return MINUS; }
\/				        { return SLASH; }
\%				        { return PERCENT; }
\<				        { return INF; }
\>				        { return SUP; }
\<=				        { return INFEQ; }
\>=				        { return SUPEQ; }
\&\&			        { return ANDAND; }
\|\|			        { return PIPEPIPE; }

-eq                     { return EQUAL_COMP;} // Comparateurs
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

{blanc}     	        {  } // Blancs

{mot}                   {   // Mot
                            yylval.str = copy_string(yytext);
                            return MOT;
                        }

{chaine}                {   // Chaine
                            yylval.str = copy_string(yytext);
                            return CHAINE;
                        }




. fprintf(stderr, "Unexpected character: %s\n", yytext);
%%