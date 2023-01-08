/* Module recensant les opérateurs
 et leurs fonctions associées
*/

#ifndef SRC_INCLUDES_INTERMEDIATE_OPERATORS_H_
#define SRC_INCLUDES_INTERMEDIATE_OPERATORS_H_

typedef enum Operator {
	OP_EXIT,
	OP_ASSIGN,
	OP_ASSIGN_TAB,
	OP_ASSIGN_FROM_TAB,
	OP_ECHO,
	OP_ADD,
	OP_MINUS,
	OP_MULT,
	OP_DIV,
	OP_MOD,
	OP_NOTEMPTY,
	OP_EMPTY,
	OP_EQUAL,
	OP_NEQUAL,
	OP_STSUP,
	OP_SUPEQ,
	OP_STINF,
	OP_INFEQ,
	OP_GOTO
} Operator;

/* Affiche un opérateur */
void print_operator(Operator op);

#endif  // SRC_INCLUDES_INTERMEDIATE_OPERATORS_H_
