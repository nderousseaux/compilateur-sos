// Module de base du programme
// Contient la boucle principale qui appelera les autres modules

#ifndef SRC_INCLUDES_SOS_H_
#define SRC_INCLUDES_SOS_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/* Initialise les structures de mémoire */
void init();

/*
* Boucle principale du programme
*/
int SoS(FILE * input, FILE * output);

#endif  // SRC_INCLUDES_SOS_H_
