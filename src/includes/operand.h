// Module contenant le code relatif aux opérandes
// et aux operateurs. On pourra aussi trouver des fonctions
// pour créer des operandes courrantes (variable, constante, vide, etc...)

#ifndef SRC_INCLUDES_OPERAND_H_
#define SRC_INCLUDES_OPERAND_H_

// Définit le type d'un operateur
typedef enum Operator {
    OP_EXIT
} Operator;

// Définit le type d'une operande
typedef enum Type_operand {
    VAR,
    EMPTY
} Type_operand;

// Définit un operande
typedef struct Operand {
    Type_operand type;          // Type de l'operande
    int value;                  // Valeur de l'operande
} Operand;

/* Crée une opérande de type var */
Operand var(int value);

/* Crée une opérande de type empty (pas d'operande) */
Operand empty();

#endif  // SRC_INCLUDES_OPERAND_H_
