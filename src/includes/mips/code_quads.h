/* Génère le code mips de chaque quad
 */

#ifndef SRC_INCLUDES_MIPS_CODE_QUADS_H_
#define SRC_INCLUDES_MIPS_CODE_QUADS_H_

/* Traitement du quad OP_EXIT */
void gen_end(Quad *quad);

/* Traitement du quad OP_ASSIGN */
void gen_assign(Quad *quad);

/* Traitement du quad OP_ASSIGN_TAB */
void gen_assign_tab(Quad *quad);

void gen_assign_from_tab(Quad *quad);

/* Traitement du quad OP_ECHO */
void gen_echo(Quad *quad);

/* Traitement du quad OP_ADD */
void gen_add(Quad *quad);

/* Traitement du quad OP_SUB */
void gen_sub(Quad *quad);

/* Traitement du quad OP_MUL */
void gen_mul(Quad *quad);

/* Traitement du quad OP_DIV */
void gen_div(Quad *quad);

/* Traitement du quad OP_MOD */
void gen_mod(Quad *quad);

/* Traitement du quad OP_NOTEMPTY */
void gen_noempty(Quad *quad);

/* Traitement du quad OP_EMPTY */
void gen_empty(Quad *quad);

/* Traitement du quad OP_EQUAL */
void gen_equal(Quad *quad);

/* Traitement du quad OP_NEQUAL */
void gen_nequal(Quad *quad);

/* Traitement du quad OP_STSUP */
void gen_stsup(Quad *quad);

/* Traitement du quad OP_SUPEQ */
void gen_supeq(Quad *quad);

/* Traitement du quad OP_STINF */
void gen_stinf(Quad *quad);

/* Traitement du quad OP_INFEQ */
void gen_infeq(Quad *quad);

/* Traitement du quad OP_GOTO */
void gen_goto(Quad *quad);

/* Traitement du quad OP_READ */
void gen_read(Quad *quad);

#endif // SRC_INCLUDES_MIPS_CODE_QUADS_H_
