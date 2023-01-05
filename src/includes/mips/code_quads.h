/* Génère le code mips de chaque quad
*/

#ifndef SRC_INCLUDES_MIPS_CODE_QUAD_H_
#define SRC_INCLUDES_MIPS_CODE_QUAD_H_

FILE *f;  // Fichier de sortie

/* Traitement du quad OP_EXIT */
void gen_end(Quad quad);

/* Traitement du quad OP_ASSIGN */
void gen_assign(Quad quad);

/* Traitement du quad OP_ECHO */
void gen_echo(Quad quad);

/* Traitement du quad OP_ADD */
void gen_add(Quad quad);

/* Traitement du quad OP_SUB */
void gen_sub(Quad quad);

/* Traitement du quad OP_MUL */
void gen_mul(Quad quad);

/* Traitement du quad OP_DIV */
void gen_div(Quad quad);

/* Traitement du quad OP_MOD */
void gen_mod(Quad quad);

#endif  // SRC_INCLUDES_MIPS_CODE_QUAD_H_
