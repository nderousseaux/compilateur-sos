/* Module regroupant toutes les actions associées aux règles de grammaire
 */

#ifndef SRC_INCLUDES_INTERMEDIATE_ACTIONS_H_
#define SRC_INCLUDES_INTERMEDIATE_ACTIONS_H_

#include "./operands.h"
#include "./quads.h"
#include "./ctrl_ql.h"

/* Génère le quad exit */
void gencode_exit(Operand *op);

/* Génère le quad d'association */
void gencode_assign(char * dst, Op_list * src);

/* Génère le quad d'affichage */
void gencode_echo(Op_list *op_list);

/* Génère le code relatif à une opération */
Operand * gencode_operation(
	Operator operator, Operand * op1, Operand * op2);

/* Génère le code relatif à une instruction if */
void gencode_if(Ctrl_ql * test_block, int first_true);

/* Génère le code relatif à un test */
void gencode_test(
	Operator operator, Operand * op1, Operand * op2, Ctrl_ql * res);

#endif  // SRC_INCLUDES_INTERMEDIATE_ACTIONS_H_
