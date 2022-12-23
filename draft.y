%{
    #include <stdio.h>
    #include "intermediate.h"
    extern int yylex();
    void yyerror(const char * error);
%}

// Type des noeuds
%union {
    int integer;
    char * string;
}

%start programme

// Liste des tokens et leurs type
%token <integer> INTEGER
%token <string> ID
%token <string> MOT
%token <string> CHAINE

// Mots clés
%token CASE
%token DECLARE
%token DO
%token DONE
%token ECH
%token ELIF
%token ELSE
%token ESAC
%token EXIT
%token EXPR
%token FI
%token FOR
%token IF
%token IN
%token LOCAL
%token READ
%token RETURN
%token TEST
%token THEN
%token UNTIL
%token WHILE

// Opérateurs
%token EQUAL

// Symboles
%token SEMICOLON
%token OBRACE
%token QUOTE

%type <integer> operande-entier

%%
programme           : liste-instructions                                        {  }
            
liste-instructions  : liste-instructions SEMICOLON instruction                  {  }
                    | instruction
                    
instruction         : ID EQUAL concatenation                                    {  }
                    | ID '[' operande-entier ']' EQUAL concatenation            {  }
                    | DECLARE ID '[' INTEGER ']'                                {  }
                    | IF test-block THEN liste-instructions else-part FI        {  }
                    | FOR ID liste-instructions DONE                            {  }
                    | WHILE test-block DO liste-instructions DONE               {  }
                    | UNTIL test-block DO liste-instructions DONE               {  }
                    | CASE operande IN liste-cas ESAC                           {  }
                    | ECH liste-operandes                                      	{  }
					| READ ID 						     						{  }
					| READ ID '[' operande-entier ']'                           {  }
					| decla-fonction                            		        {  }
					| appel-de-fonction                                         {  }
					| RETURN													{  }
					| RETURN operande-entier                                    {  }
					| EXIT                                                      { quad_exit(0);  }
					| EXIT operande-entier                                      { quad_exit($2); }

else-part           : ELIF test-block THEN liste-instructions else-part         {  }
					| ELSE liste-instructions                                   {  }
					|                                                           {  }

liste-cas           : liste-cas filtre ')' liste-instructions SEMICOLON SEMICOLON {  }
					| filtre ')' liste-instructions SEMICOLON SEMICOLON			{  }

filtre				: MOT														{  }
					| '"' CHAINE '"'											{  }
					| QUOTE CHAINE QUOTE										{  }
					| filtre '|' MOT											{  }
					| filtre '|' '"' CHAINE '"'									{  }
					| filtre '|' QUOTE CHAINE QUOTE								{  }
					| '*'														{  }

liste-operandes     : liste-operandes operande                         			{  }
					| operande													{  }
					| '$' OBRACE ID '[' '*' ']' '}'								{  }

concatenation	   	: concatenation operande 									{  }
				   	| operande													{  }

test-block		    : TEST test-expr 											{  }

test-expr			: test-expr '-' 'o' test-expr2								{  }
					| test-expr2												{  }

test-expr2			: test-expr2 '-' 'a' test-expr3								{  }
					| test-expr3												{  }

test-expr3			: '(' test-expr3 ')'										{  }
					| '!' '(' test-expr3 ')'									{  }
					| test-instruction											{  }
					| '!' test-instruction										{  }

test-instruction	: concatenation EQUAL concatenation							{  }
					| concatenation '!' EQUAL concatenation						{  }
					| operateur1 concatenation									{  }
					| operande operateur2 operande								{  }

operande   			: '$' OBRACE ID '}'											{  }
					| '$' OBRACE ID '[' operande-entier ']' '}'					{  }
					| MOT														{  }
					| '$' INTEGER												{  }
					| '$' '*'													{  }
					| '$' '?'													{  }
					| '"' CHAINE '"'											{  }
					| QUOTE CHAINE QUOTE										{  }
					| '$' '(' EXPR somme-entiere ')'							{  }
					| '$' '(' appel-de-fonction ')'								{  }

operateur1			: '-' 'n'													{  }
					| '-' 'z'													{  }

operateur2			: '-' 'e' 'q'												{  }
					| '-' 'n' 'e'												{  }
					| '-' 'l' 't'												{  }
					| '-' 'l' 'e'												{  }
					| '-' 'g' 't'												{  }
					| '-' 'g' 'e'												{  }

somme-entiere		: somme-entiere plus-ou-moins produit-entier				{  }
					| produit-entier											{  }

produit-entier		: produit-entier fois-div-mod operande-entier				{  }
					| operande-entier											{  }

operande-entier		: '$' OBRACE ID '}'											{  }
					| '$' OBRACE ID '[' operande-entier ']' '}'					{  }
					| '$' INTEGER												{  }
					| plus-ou-moins '$' OBRACE ID '}'							{  }
					| plus-ou-moins '$' OBRACE ID '[' operande-entier ']' '}'	{  }
					| plus-ou-moins '$' INTEGER									{  }
					| INTEGER													{ $$ = $1;  }
					| plus-ou-moins INTEGER										{  }
					| '(' somme-entiere ')'										{  }

plus-ou-moins		: '+'														{  }
					| '-'														{  }

fois-div-mod		: '*'														{  }
					| '/'														{  }
					| '%'														{  }
			
decla-fonction		: ID '(' ')' OBRACE decla-loc liste-instructions '}'		{  }

decla-loc			: decla-loc LOCAL ID EQUAL concatenation SEMICOLON			{  }
					|      														{  }

appel-de-fonction	: ID liste-operandes										{  }
					| ID														{  }

%%


void yyerror(const char * error)
{
    fprintf(stderr, "%s\n", error);
}
