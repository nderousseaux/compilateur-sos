/* Module pour créer une structure spécialement pour gérer
les boucles for */

#ifndef SRC_INCLUDES_INTERMEDIATE_FOR_STRUCT_H_
#define SRC_INCLUDES_INTERMEDIATE_FOR_STRUCT_H_

#include "quads.h"

typedef struct Ctrl_for
{
    int q_index;     // Un index de quad
    char *temp_name; // Nom de la variable créée pour le for
    Ql *q_test;      // Liste de quad à compléter
} Ctrl_for;

#endif