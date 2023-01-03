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

#endif  // SRC_INCLUDES_MIPS_CODE_QUAD_H_
