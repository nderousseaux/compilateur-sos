// Module contenant le code relatif aux opérandes
// et aux operateurs. On pourra aussi trouver des fonctions
// pour créer des operandes courrantes (variable, constante, vide, etc...)

#ifndef SRC_INCLUDES_OPERAND_H_
#define SRC_INCLUDES_OPERAND_H_

// Définit le type d'un operateur
typedef enum Operator
{
    OP_GOTO,
    OP_EXIT,
    OP_ASSIGN,
    OP_ECHO,
    OP_EQUAL,
    OP_NEQUAL,
    OP_STSUP,
    OP_SUPEQ,
    OP_STINF,
    OP_INFEQ,
    OP_IF
    OP_PLUS,
    OP_MOINS,
    OP_FOIS,
    OP_DIVISION,
    OP_MODULO,
} Operator;

// Définit le type d'une operande
typedef enum Type_operand
{
    ID,
    VAR,
    INTEGER,
    TEMP,
    EMPTY
} Type_operand;

// Définit un operande
typedef struct Operand
{
    Type_operand type; // Type de l'operande
    char *value;       // Valeur de l'operande (si c'est une variable)
    int integer_value; // Valeur de l'operande (si c'est une constante)
} Operand;

/* Crée une opérande de type var */
Operand var(char *value);

/* Crée une opérande de type int */
Operand integer(int value);

/* Crée une opérande de type id */
Operand id(char *name);

/* Crée une opérande de type temp */
Operand temp();

/* Crée une opérande de type empty (pas d'operande) */
Operand empty();

#endif // SRC_INCLUDES_OPERAND_H_
