/* S'occupe du code dans le code mips
* La génération du code pour chaque quad se 
* trouve dans le fichier code_quad.c
*/

#include "../includes/imports.h"

extern FILE *f;  // Fichier de sortie

/* Génère la pile */
void gen_stack() {
	// On alloue la mémoire pour la pile
    fprintf(f, "\n\t# On alloue la mémoire pour la pile\n");
    fprintf(f, "\taddiu\t$sp,\t$sp,\t-%d\n", symbols_table->last_pos + 4);
    fprintf(f, "\tsw\t\t$fp,\t%d($sp)\n", symbols_table->last_pos);
    fprintf(f, "\tmove\t$fp,\t$sp\n");
}

/* Génère le code de tout les quads */
void gen_code() {
	fprintf(f, ".text\n");
	fprintf(f, "main:");

	char exit = 0;

	gen_stack();

	// Pour chaque quad
	Quad * quad;
	for (int i = 0; i < quad_list->size; i++) {
		quad = quad_list->data[i];

		// On génère le flag du quad
		fprintf(f, "\n\tquad_%d:\n", i);

		// On génère le code du quad
		switch (quad->op) {
			case OP_EXIT:
				gen_end(quad);
				exit = 1;
				break;
			case OP_ASSIGN:
				gen_assign(quad);
				break;
			case OP_ASSIGN_TAB:
				gen_assign_tab(quad);
				break;
			case OP_ASSIGN_FROM_TAB:
				gen_assign_from_tab(quad);
				break;
			case OP_ECHO:
				gen_echo(quad);
				break;
			case OP_ADD:
				gen_add(quad);
				break;
			case OP_MINUS:
				gen_sub(quad);
				break;
			case OP_MULT:
				gen_mul(quad);
				break;
			case OP_DIV:
				gen_div(quad);
				break;
			case OP_MOD:
				gen_mod(quad);
				break;
			case OP_NOTEMPTY:
				gen_noempty(quad);
				break;
			case OP_EMPTY:
				gen_empty(quad);
				break;
			case OP_EQUAL:
				gen_equal(quad);
				break;
			case OP_NEQUAL:
				gen_nequal(quad);
				break;
			case OP_STSUP:
				gen_stsup(quad);
				break;
			case OP_SUPEQ:
				gen_supeq(quad);
				break;
			case OP_STINF:
				gen_stinf(quad);
				break;
			case OP_INFEQ:
				gen_infeq(quad);
				break;
			case OP_GOTO:
				gen_goto(quad);
				break;
			default:
				printf("Opérateur inconnu (pensez à l'ajouter dans gen_code...)\n");
				break;
		}
	}

	// Si on n'a pas trouvé d'instruction de sortie,
	// on génère le code de terminaison
    if (!exit) {
		fprintf(f, "\n\tquad_%d:\n", quad_list->size);
		Quad quad = {OP_EXIT, empty(), empty(), integer(0), quad_list->size};
        gen_end(&quad);
    }
}
