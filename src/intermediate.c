// Module permettant la génération du code intermédiaire
// Il contient les fonctions permettant de créer les quadruplets
// depuis la grammaire. Cela aurait pu être directement dans le yacc,
// mais cela aurait rendu le code difficilement lisible.

#include "includes/intermediate.h"

/* Crée un quadruplet exit */
void quad_exit(int status) {
    add_quad(OP_EXIT, var(status), empty(), empty());
}
