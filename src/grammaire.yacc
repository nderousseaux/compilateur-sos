%{
    #include <stdio.h>
    extern int yylex();
    void yyerror(const char * error);
%}

%start programme
%token APOSTROPHE
%token ASSIGN
%token AUTRE    //?
%token CASE
%token CBRACE
%token CCROCH
%token CHAINE
%token CPAR
%token DECLARE
%token DIV
%token DO
%token DOLLAR  
%token DONE
%token EKHO
%token ELIF
%token ELSE
%token ENTIER
%token EQUAL
%token ESAC
%token ET
%token EXIT
%token EXPR
%token FI
%token FOIS
%token FOR
%token GUILL
%token ID
%token IF
%token IN
%token INFEQUAL
%token INFSTRC
%token LESS
%token LOCAL
%token MOD
%token MOT
%token N        //-n
%token NEQUAL
%token NOT
%token OBRACE
%token OCROCH
%token OPAR
%token OU
%token PLUS
%token READ
%token RETURN
%token SEMI_COLON
%token SUPEQUAL
%token SUPSTRC
%token TEST
%token THEN
%token TUBE
%token UNTIL
%token WWHILE
%token Z        //-z


%%
programme           : %empty
                    | liste-instructions
            
liste-instructions  : instruction
                    | liste-instructions SEMI_COLON instruction
                    

instruction         : ID ASSIGN concatenation
                    | ID OCROCH operande-entier CCROCH ASSIGN concatenation
                    | DECLARE ID OCROCH ENTIER CCROCH
                    | IF test-bloc THEN liste-instructions else-part FI
                    | FOR ID DO liste-instructions DONE
                    | FOR ID IN liste-operandes DO liste-instructions
                    | WWHILE test-bloc DO liste-instructions DONE
                    | UNTIL test-bloc DO liste-instructions DONE
                    | CASE operande IN liste-cas ESAC
                    | EKHO liste-operandes
                    | READ ID
                    | READ ID OCROCH operande-entier CCROCH
                    | declaration-de-fonction
                    | appel-de-fonction
                    | RETURN 
                    | RETURN operande-entier
                    | EXIT
                    | EXIT operande-entier              { printf("exit %d\n", $2); }

else-part           : ELIF test-bloc THEN liste-instructions else-part
                    | ELSE liste-instructions
                    | %empty

liste-cas           : liste-cas filtre CPAR liste-instructions

filtre              : MOT 
                    | GUILL CHAINE GUILL
                    | APOSTROPHE CHAINE APOSTROPHE
                    | filtre TUBE MOT
                    | filtre TUBE GUILL CHAINE GUILL
                    | filtre TUBE APOSTROPHE CHAINE APOSTROPHE
                    | FOIS

liste-operandes     : liste-operandes operande
                    | operande
                    | DOLLAR OBRACE ID OCROCH FOIS CCROCH CBRACE

concatenation       : concatenation operande
                    | operande

test-bloc           : TEST test-expr

test-expr           : test-expr OU test-expr2
                    | test-expr2

test-expr2          : test-expr2 ET test-expr3
                    | test-expr3

test-expr3          : OPAR test-expr CPAR
                    | NOT OPAR test-expr CPAR
                    | test-instruction
                    | NOT test-instruction

test-instruction    : concatenation ASSIGN concatenation
                    | concatenation NOT concatenation
                    | operateur1 concatenation
                    | operande operateur2 operande

operande            : DOLLAR OBRACE ID CBRACE
                    | DOLLAR OBRACE ID OCROCH operande-entier CCROCH CBRACE
                    | MOT
                    | DOLLAR ENTIER
                    | DOLLAR FOIS
                    | DOLLAR AUTRE
                    | GUILL CHAINE GUILL
                    | APOSTROPHE CHAINE APOSTROPHE
                    | DOLLAR OPAR EXPR somme-entiere CPAR
                    | DOLLAR appel-de-fonction

operateur1          : N
                    | Z

operateur2          : EQUAL
                    | NEQUAL
                    | SUPSTRC
                    | SUPEQUAL
                    | INFSTRC
                    | INFEQUAL

somme-entiere       : somme-entiere plus-ou-moins produit-entier
                    | produit-entier

produit-entier      : produit-entier fois-div-mod operande-entier
                    | operande-entier

operande-entier     : DOLLAR OBRACE ID CBRACE
                    | DOLLAR OBRACE ID OCROCH operande-entier CCROCH CBRACE
                    | DOLLAR ENTIER
                    | plus-ou-moins DOLLAR OBRACE ID CBRACE
                    | plus-ou-moins DOLLAR OBRACE ID OCROCH operande-entier CCROCH CBRACE
                    | plus-ou-moins DOLLAR ENTIER
                    | ENTIER                            
                    | plus-ou-moins ENTIER
                    | OPAR somme-entiere CPAR

plus-ou-moins       : PLUS
                    | LESS

fois-div-mod        : FOIS
                    | DIV
                    | MOD

declaration-de-fonction : ID OPAR CPAR OBRACE decl-loc liste-instructions

decl-loc                : decl-loc LOCAL ID ASSIGN concatenation
                        | %empty


appel-de-fonction   : ID liste-operandes
                    | ID
%%


void yyerror(const char * error)
{
    fprintf(stderr, "Erreur yacc: %s\n", error);
}
