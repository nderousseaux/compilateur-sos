// Module de base du programme
// Contient la boucle principale qui appelera les autres modules

#include "includes/SoS.h"
#include "includes/mips.h"
#include "includes/quad.h"
#include "includes/symbols.h"

extern int yyparse();
extern int yylval;
extern FILE * yyin;

/* Initialise les structures de mémoire */
void init() {
    quad_list = init_quad_list();
    symbols_table = init_st();
}

/* Supprime les structure de mémoire */
void destroy() {
    destroy_quad_list(quad_list);
    destroy_st(symbols_table);
}

/*
* Boucle principale du programme
*/
int SoS(FILE * input, FILE * output, char debug) {
    yyin = input;

    // On initialise les structures de mémoire
    init();

    // On parse le fichier
    int ret = yyparse();
    if (ret != 0)
        return ret;

    // On génère le code assembleur
    gen_mips(output, debug);

    //On supprime les structures de mémoire
    destroy();

    return 0;
}
