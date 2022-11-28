// Module de base du programme
// Contient la boucle principale qui appelera les autres modules

#include "includes/SoS.h"
#include "includes/quad.h"
#include "includes/mips.h"

extern int yyparse();
extern int yylval;
extern FILE * yyin;

/* Initialise les structures de mémoire */
void init() {
    quad_list = init_quad_list();
}

/*
* Boucle principale du programme
*/
int SoS(FILE * input, FILE * output) {
    yyin = input;
    (void) output;

    // On initialise les structures de mémoire
    init();

    // On parse le fichier
    int ret = yyparse();
    if (ret != 0)
        return ret;

    // On génère le code assembleur
    gen_mips(output);

    return 0;
}
