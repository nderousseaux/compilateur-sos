%option nounput
%option noinput
%option noyywrap

%{
    #include <stdio.h>
    #include "grammaire.tab.h"
%}

entier                 [0-9]+
s_entier_simple        \'(\+?|\-?){entier}\'
s_entier_double        \"(\+?|\-?){entier}\"
s_simple               \'([^\'\\]|\\.)*\'
s_double               \"([^\"\\]|\\.)*\"
s_normal               [^[[:space:]]]*
s_mot                  [{s_normal}^[{s_normal}^\(\)\{\}{:=}\[\]=;!$\*\+-/%(<<)(>>)~\|(!=)(&&)(\|\|)\.(==)(<=)(>=)\^(-=)(+=)(*=)(/=)(&=)(^=)(|=)(<<=)(>>=),(if)(then)(for)(do)(done)(in)(while)(until)(case)(esac)(echo)(read)(return)(exit)(local)(elif)(else)(fi)(declare)(test)(expr)

%%


{entier}|{s_entier_simple}|{s_entier_double}        { return ENTIER; }
{s_normal}                                          { return ID; }
{s_mot}                                             { return MOT; }
{s_simple}|{s_double}|{s_normal}                    { return CHAINE; }
if                                                  { return IF; }
then                                                { return THEN; }
for                                                 { return FOR; }
do                                                  { return DO; }
done                                                { return DONE; }
in                                                  { return IN; }
while                                               { return WWHILE; }
until                                               { return UNTIL; }
case                                                { return CASE; }
esac                                                { return ESAC; }
echo                                                { return EKHO; }
read                                                { return READ; }
return                                              { return RETURN; }
local                                               { return LOCAL; }
elif                                                { return ELIF; }
else                                                { return ELSE; }
declare                                             { return DECLARE; }
test                                                { return TEST; }
expr                                                { return EXPR; }
exit                                                { return EXIT; }
\;                                                  { return SEMI_COLON;}
=                                                   { return ASSIGN; } 
\|                                                  { return TUBE; }
?                                                   { return AUTRE; }
-n                                                  { return N; }
-z                                                  { return Z; }
-eq                                                 { return EQUAL; }
-ne                                                 { return NEQUAL; }
-gt                                                 { return SUPSTRC; }
-ge                                                 { return SUPEQUAL; }
-lt                                                 { return INFSTRC; }                         
-le                                                 { return INFEQUAL; }
-a                                                  { return ET; }
-o                                                  { return OU; }
!                                                   { return NOT; }
*                                                   { return FOIS; }
/                                                   { return DIV; }
%                                                   { return MOD; }
$                                                   { return DOLLAR; }
'                                                   { return APOSTROPHE; }
\"                                                   { return GUILL; }
\+                                                   { return PLUS; }
\-                                                   { return LESS; }
\{                                                  { return CCROCH; }
\(                                                  { return OPAR; }
\)                                                  { return CPAR; }
\[                                                  { return OCROCH; }
\]                                                  { return OBRACE; }
\}                                                  { return CBRACE; }
#(.*)\n                                             {               } //Commentaires

[[:space:]]                                         {               }



. fprintf(stderr, "Erreur lex: %s\n", yytext);

%%