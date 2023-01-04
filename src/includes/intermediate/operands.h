/* Module recensant les opérandes
 et leurs fonctions associées
*/

#ifndef SRC_INCLUDES_INTERMEDIATE_OPERANDS_H_
#define SRC_INCLUDES_INTERMEDIATE_OPERANDS_H_

#include "./symbols.h"

#define INIT_OP_LIST_CAPACITY 10

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

// Liste d'opérandes
typedef struct Op_list {
	Operand *data;
	int size;
	int capacity;
} Op_list;

/* Crée une liste d'opérande */
Op_list *init_op_list();

/* Crée une l	iste d'opérande avec une première opérande */
Op_list *create_list_op(Operand *op);

/* Ajoute une opérande dans une liste */
Op_list * add_op(Op_list *op_list, Operand *op);

/* Affiche une liste d'opérande */
void print_op_list(Op_list *op_list);

/* Concatène deux listes d'opérandes */
Op_list *concat_op(Op_list *op_list1, Op_list *op_list2);

/* Supprime une liste d'opérandes */
void destroy_op_list(Op_list *op_list);

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

/* Transforme une chaine de caractère en operande constante */
void to_operand_const(Operand *op, char *value);

/* Affiche une opérande */
void print_operand(Operand op);

#endif  // SRC_INCLUDES_INTERMEDIATE_OPERANDS_H_
