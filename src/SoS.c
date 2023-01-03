// Module de base du programme
// Contient la boucle principale qui appelera les autres modules

#include "includes/imports.h"

extern int yyparse();
extern int yylval;
extern FILE * yyin;

/* Initialise les structures de mémoire */
void init() {
    symbols_table = init_st();
    quad_list = init_quad_list();
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

    // On parse le fichier (on remplit la liste des quadruplets)
    int ret = yyparse();
    if (ret != 0)
        return ret;

    if (debug) {
        // On affiche la liste des quadruplets et la table des symboles
        print_quad_list(quad_list);
        print_st(symbols_table);
    }

    // On génère le code assembleur
    gen_mips(output);

    // On supprime les structures de mémoire
    destroy();

    return 0;
}
