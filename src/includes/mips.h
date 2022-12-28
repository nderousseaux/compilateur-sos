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

/* Génére le code pour un test d'égalité*/
void gen_equal(Quad quad);

/* On génère le code de terminaison */
void gen_end(int exit_code);

void gen_goto(Quad quad);

char * gen_flag_quad(int idx_quad);

/* Génére code assembleur final dans le .asm */
void gen_mips(FILE *output, char debug);

#endif // SRC_INCLUDES_MIPS_H_
