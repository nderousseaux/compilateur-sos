/* Module recensant les opérateurs
 et leurs fonctions associées
*/

#include "../includes/imports.h"

/* Affiche un opérateur */
void print_operator(Operator op) {
	switch (op) {
		case OP_EXIT:
			printf("EXIT");
			break;
		case OP_ASSIGN:
			printf("ASSIGN");
			break;
		case OP_ECHO:
			printf("ECHO");
			break;
		default:
			printf("\nOpérateur inconnu (pensez à l'ajouter dans print_quad...)\n");
			break;
	}
}