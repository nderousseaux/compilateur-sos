/* Module recensant les opérandes
 et leurs fonctions associées
*/

#ifndef SRC_INCLUDES_INTERMEDIATE_OPERANDS_H_
#define SRC_INCLUDES_INTERMEDIATE_OPERANDS_H_

#include "./symbols.h"

// Type des opérandes
typedef enum Type_operand {
	ID_T,
	CONST_T,
	INTEGER_T,
	EMPTY_T
} Type_operand;

// Opérande
typedef struct Operand {
	Type_operand type;
	Symbol *symbol;	 // Symbole associé à l'opérande (si id ou une constante)
	int value_int;	 // Valeur de l'opérande (si c'est un entier)
} Operand;

/* Crée une opérande de type id */
Operand id(char *value);

/* Crée une opérande de type const */
Operand constant(char *name);

/* Crée une opérande de type int */
Operand integer(int value);

/* Crée une opérande de type empty (pas d'operande) */
Operand empty();

/* Transforme une chaine de caractère en operande entier */
void to_operand_int(Operand *op, char *value);

/* Transforme une chaine de caractère en operande id */
void to_operand_id(Operand *op, char *value);

/* Affiche une opérande */
void print_operand(Operand op);

#endif  // SRC_INCLUDES_INTERMEDIATE_OPERANDS_H_
