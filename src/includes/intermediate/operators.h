/* Module recensant les opérateurs
 et leurs fonctions associées
*/

#ifndef SRC_INCLUDES_INTERMEDIATE_OPERATORS_H_
#define SRC_INCLUDES_INTERMEDIATE_OPERATORS_H_

typedef enum Operator {
	OP_EXIT,
	OP_ASSIGN,
	OP_ECHO,
	OP_UNKNOWN
} Operator;


/* Affiche un opérateur */
void print_operator(Operator op);

#endif  // SRC_INCLUDES_INTERMEDIATE_OPERATORS_H_
