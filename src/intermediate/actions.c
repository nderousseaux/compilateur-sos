/* Toutes les actions associées aux règles de grammaire */

#include "../includes/imports.h"

/* Génère le quad exit */
void gencode_exit(int exit_code) {
	gencode(OP_EXIT, empty(), empty(), integer(exit_code));
}

/* Génère le quad d'association */
void gencode_assign(char * dst, Operand * src) {
	// On crée l'identifiant dans la table des symboles
	add_var_st(dst);

	// On génère le quad
	gencode(OP_ASSIGN, *src, empty(), id(dst));
}

/* Génère le quad d'affichage */
void gencode_echo(Operand * op) {
	gencode(OP_ECHO, empty(), empty(), *op);
}