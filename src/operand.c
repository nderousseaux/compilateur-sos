// Module contenant le code relatif aux opérandes
// et aux operateurs. On pourra aussi trouver des fonctions
// pour créer des operandes courrantes (variable, constante, vide, etc...)

#include "includes/operand.h"
#include "includes/symbols.h"

/* Crée une opérande de type var */
Operand var(char * value) {
    Operand o = { VAR, value, 0};
    return o;
}

/* Crée une opérande de type int */
Operand integer(int value) {
    Operand o = { INTEGER, 0, value };
    return o;
}

/* Crée une opérande de type id */
Operand id(char * name) {
    
    Operand o = { ID, name, 0};
    return o;
}

/* Crée une opérande de type empty (pas d'operande) */
Operand empty() {
    Operand o = { EMPTY, 0, 0};
    return o;
}
