/* Toutes les actions associées aux règles de grammaire */

#include "../includes/imports.h"

/* Génère le quad exit */
void gencode_exit(int exit_code) {
	gencode(OP_EXIT, empty(), empty(), integer(exit_code));
}

/* Génère le quad d'association */
void gencode_assign(char * dst, Op_list * src) {
	// On vérifie que la liste ne contient qu'une seule opérande
	if (src->size > 1) {
		fprintf(stderr,
		"Erreur: l'opérande de droite de l'assignation contient plus d'une opérande");
		exit(EXIT_FAILURE);
	}

	// On crée l'identifiant dans la table des symboles
	add_var_st(dst, src->data[0].type);

	// On génère le quad
	gencode(OP_ASSIGN, src->data[0], empty(), id(dst));
}

/* Génère le quad d'affichage */
void gencode_echo(Op_list *op_list) {
	// Pour chaque opérande de la liste, on génère un quad echo
	for (int i = 0; i < op_list->size; i++) {
		gencode(OP_ECHO, empty(), empty(), op_list->data[i]);
	}
}