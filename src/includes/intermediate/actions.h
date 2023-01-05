/* Module regroupant toutes les actions associées aux règles de grammaire
 */

#ifndef SRC_INCLUDES_INTERMEDIATE_ACTIONS_H_
#define SRC_INCLUDES_INTERMEDIATE_ACTIONS_H_

#include "./operands.h"
#include "./quads.h"

/* Génère le quad exit */
void gencode_exit(Operand *op);

/* Génère le quad d'association */
void gencode_assign(char * dst, Op_list * src);

/* Génère le quad d'affichage */
void gencode_echo(Op_list *op_list);

/* Génère le code relatif à une opération */
Operand * gencode_operation(
	Operator operator, Operand * op1, Operand * op2);

#endif  // SRC_INCLUDES_INTERMEDIATE_ACTIONS_H_
