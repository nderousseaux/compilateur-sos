// Module contenant le code relatif aux opérandes
// et aux operateurs. On pourra aussi trouver des fonctions
// pour créer des operandes courrantes (variable, constante, vide, etc...)

#include "includes/imports.h"

/* Crée une opérande de type var */
Operand var(char *value)
{
    Operand o = {VAR, value, 0};
    return o;
}

/* Crée une opérande de type int */
Operand integer(char * value) {
    Operand o = { INTEGER, value, 0};
    return o;
}

/* Crée une opérande de type id */
Operand id(char *name)
{

    Operand o = {ID, name, 0};
    return o;
}

Operand temp(char * name) {
    Operand o = { TEMP, name, 0};
    return o;
}


/* Crée une opérande de type empty (pas d'operande) */
Operand empty()
{
    Operand o = {EMPTY, 0, 0};
    return o;
}


char operator_to_string(Operator op){
    switch(op){
        case OP_PLUS:
            return '+';
        case OP_MOINS:
            return '-';
        case OP_FOIS:
            return '*';
        case OP_DIVISION:
            return '/';
        case OP_MODULO:
            return '%';
        default:
            break;
    }
    return '?';
}