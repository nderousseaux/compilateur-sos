// Module permettant de passer du code intermédiaire au code MIPS

#ifndef SRC_INCLUDES_MIPS_H_
#define SRC_INCLUDES_MIPS_H_

#include <stdio.h>
#include "./quad.h"

FILE * f;  // Fichier de sortie

/* Génère les données */
void gen_data();

/* Génère le code */
void gen_code();

/* On génère le code de terminaison */
void gen_end(int exit_code);

/* Génére code assembleur final dans le .asm */
void gen_mips(FILE * output);

#endif  // SRC_INCLUDES_MIPS_H_
