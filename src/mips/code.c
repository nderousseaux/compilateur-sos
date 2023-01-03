/* S'occupe du code dans le code mips
* La génération du code pour chaque quad se 
* trouve dans le fichier code_quad.c
*/

#include "../includes/imports.h"

FILE *f;  // Fichier de sortie

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
	Quad quad;
	for (int i = 0; i < quad_list->size; i++) {
		quad = quad_list->data[i];

		// On génère le flag du quad
		fprintf(f, "\n\tquad_%d:\n", i);

		// On génère le code du quad
		switch (quad.op) {
			case OP_EXIT:
				gen_end(quad);
				exit = 1;
				break;
			case OP_ASSIGN:
				gen_assign(quad);
				break;
			case OP_ECHO:
				gen_echo(quad);
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
        gen_end(quad);
    }
}
