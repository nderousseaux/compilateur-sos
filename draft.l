%option nounput
%option noinput
%option noyywrap

%{
	#include <stdio.h>
	#include "grammaire.tab.h"
%}

com				#.*\n
car       		"\\\""|"\\\\"|"\\'"|"\\t"|"\\n"|[ -~]
mot				[^ \t\r\n]+

[[:^space:]]*[^[:space:]|;|\[|\]|\(|\)|=|!=|\||!|$|\*|{|}|+|-|*|/|<|>|<=|>=|&&|\|\||if|then|for|do|done|in|while|until|case|esac|echo|read|return|exit|local|elif|else|fi|declare|test|expr]+[[:^space:]]*


%%

if				{} // Mots reservés
then			{}
for				{}
do				{}
done			{}
in				{}
until			{}
while			{}
case			{}
esac			{}
echo			{}
read			{}
return			{}
exit			{}
local			{}
elif			{}
else			{}
fi				{}
declare			{}
test			{}
expr			{}

;				{} //Caractères spéciaux
\[				{}
\]				{}
\{				{}
\}				{}
\|				{}
\(				{}
\)				{}
=				{}
!=				{}
!				{}
&				{}
\*				{}
\"				{}
'				{}
\$				{}
\+				{} // Opérateurs
\-				{}
\/				{}
\%				{}
\<				{}
\>				{}
\<=				{}
\>=				{}
\&\&			{}
\|\|			{}


{com}			{} // Commentaires

[{com} \n\t]	{} // Blancs

(\+|-)?[0-9]+	{ //Entier
					yylval.integer=atoi(yytext);
					return INTEGER;
				}
{mot}			{ // Mot
					yylval.string=yytext;
					return MOT;
				}
{car}+			{ // Chaine
					yylval.string=yytext;
					return CHAINE;
				}



. fprintf(stderr, "Unexpected character: %s\n", yytext);

%%