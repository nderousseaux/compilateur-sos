#include "includes/SoS.h"

extern int yyparse();
extern int yylval;
extern FILE * yyin;

/*
* Boucle principale du programme
*/
int SoS(FILE * input, FILE * output) {
    yyin = input;

    int r;
    // On parcourt le fichier
    do {
        r = yyparse();
    } while (r != 0);

    (void) output;

    return r;
}
