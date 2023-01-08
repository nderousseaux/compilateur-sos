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
void gencode_assign(char * dst, Operand * src);

/* Génère le code pour associer un tableau */
void gencode_assign_tab(char * dst, Operand * index, Operand * src);

/* Génère le code pour associer un tableau à une var
* renvoie une opérande temporaire contenant la valeur du tableau
*/
Operand * gencode_tab_to_temp(char * name, Operand * index);

/* Génère le code pour déclarer un tableau */
void gencode_decla_tab(char * name, char * size);

/* Génère le quad d'affichage */
void gencode_echo(Op_list *op_list);

/* Génère le code relatif à une opération */
Operand * gencode_operation(
	Operator operator, Operand * op1, Operand * op2);

/* Génère le code relatif à une instruction if */
void gencode_if(
	Ctrl_ql * test_block,  // Contient les quads du test
	int first_true,  // Index du premier quad vrai
	Ql * list_false,  // Goto vers le premier quad si faux
	Ql * else_part);  // Quads de l'instruction else

/* Génère le code relatif à une instruction elif */
Ql * gencode_elif(
	Ctrl_ql * test_block,  // Contient les quads du test
	int first_true,  // Index du premier quad vrai
	Ql * list_false,  // Goto vers le premier quad si faux
	Ql * else_part);  // Quads de l'instruction else

Ctrl_ql * gencode_test(
	Operator operator, Operand * op1, Operand * op2);

/* Génère le code relatif à une opération OR */
void gencode_or(
	Ctrl_ql * test_expr, Ctrl_ql * test_expr2, int first_true, Ctrl_ql * res);

/* Génère le code relatif à une opération AND */
void gencode_and(
	Ctrl_ql * test_expr, Ctrl_ql * test_expr2, int first_true, Ctrl_ql * res);

/* Génère le code relatif à une opération NOT */
void gencode_not(Ctrl_ql * test_expr, Ctrl_ql * res);

/* Génère le code relatif à une instruction while */
Ql* gencode_while(
	Ctrl_ql * test_block,  // Contient les quads du test
	int first_cond,  // index du premier quad de la condition
	int first_true);  // Index du premier quad vrai

/* Génère le code relatif à une instruction until */
Ql * gencode_until(
	Ctrl_ql * test_block,  // Contient les quads du test
	int first_cond,  // index du premier quad de la condition
	int first_true);  // Index du premier quad vrai

/* Concatène deux opérandes de string */
Operand * gencode_concat(Operand * op1, Operand * op2);

#endif  // SRC_INCLUDES_INTERMEDIATE_ACTIONS_H_
