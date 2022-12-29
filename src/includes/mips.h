// Module permettant de passer du code intermédiaire au code MIPS

#ifndef SRC_INCLUDES_MIPS_H_
#define SRC_INCLUDES_MIPS_H_

/* Récupère l'index de la variable dans la pile */
int get_address(char *name);

/* Génère les données */
void gen_data();

/* Génère le code */
void gen_code();

/* Génère la pile */
void gen_stack();

/* Génère le code d'affectation */
void gen_assign(Quad quad);

/* Génére le code d'affichage */
void gen_echo(Quad quad);

void gen_plus(Quad quad);
void gen_plus2(Quad quad);

/* Génére le code pour un test d'égalité*/
void gen_equal(Quad quad);

/* Génére le code pour un test d'inégalité*/
void gen_nequal(Quad quad);

/* Génére le code pour un test strict supérieur*/
void gen_stsup(Quad quad);

/* Génére le code pour un test supérieur ou égal*/
void gen_supeq(Quad quad);

/* Génére le code pour un test strict inférieur*/
void gen_stinf(Quad quad);

/* Génére le code pour un test inférieur ou égal*/
void gen_infeq(Quad quad);

/* On génère le code de terminaison */
void gen_end(char * exit_code);

void gen_goto(Quad quad);

char *gen_flag_quad(int idx_quad);

/* Génére code assembleur final dans le .asm */
void gen_mips(FILE *output, char debug);

#endif // SRC_INCLUDES_MIPS_H_
