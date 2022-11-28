// Module permettant la génération du code intermédiaire
// Il contient les fonctions permettant de créer les quadruplets
// depuis la grammaire. Cela aurait pu être directement dans le yacc,
// mais cela aurait rendu le code difficilement lisible.

#ifndef SRC_INCLUDES_INTERMEDIATE_H_
#define SRC_INCLUDES_INTERMEDIATE_H_

#include "./operand.h"
#include "./quad.h"

/* Crée un quadruplet exit */
void quad_exit(int status);

#endif  // SRC_INCLUDES_INTERMEDIATE_H_
