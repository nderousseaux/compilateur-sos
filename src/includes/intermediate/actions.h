/* Module regroupant toutes les actions associées aux règles de grammaire
 */

#ifndef SRC_INCLUDES_INTERMEDIATE_ACTIONS_H_
#define SRC_INCLUDES_INTERMEDIATE_ACTIONS_H_

#include "./operands.h"
#include "./quads.h"

/* Génère le quad exit */
void gencode_exit(int exit_code);

/* Génère le quad d'association */
void gencode_assign(char * dst, Op_list * src);

/* Génère le quad d'affichage */
void gencode_echo(Op_list *op_list);

#endif  // SRC_INCLUDES_INTERMEDIATE_ACTIONS_H_
