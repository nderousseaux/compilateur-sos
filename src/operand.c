// Module contenant le code relatif aux opérandes
// et aux operateurs. On pourra aussi trouver des fonctions
// pour créer des operandes courrantes (variable, constante, vide, etc...)

#include "includes/operand.h"

/* Crée une opérande de type var */
Operand var(int value) {
    Operand o = { VAR, value };
    return o;
}

/* Crée une opérande de type empty (pas d'operande) */
Operand empty() {
    Operand o = { EMPTY, 0 };
    return o;
}
