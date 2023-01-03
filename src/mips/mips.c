// Module permettant de passer du code intermédiaire au code MIPS

#include "../includes/imports.h"

FILE *f;  // Fichier de sortie


/* Génére code assembleur final dans le .asm */
void gen_mips(FILE *output) {
    f = output;

    // On intègre les données
    gen_data();

    // On intègre le code
    gen_code();
}
