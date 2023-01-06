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
		case OP_ADD:
			printf("PLUS");
			break;
		case OP_MINUS:
			printf("MINUS");
			break;
		case OP_MULT:
			printf("MULT");
			break;
		case OP_DIV:
			printf("DIV");
			break;
		case OP_MOD:
			printf("MOD");
			break;
		case OP_NOTEMPTY:
			printf("NOTEMPTY");
			break;
		case OP_EMPTY:
			printf("EMPTY");
			break;
		case OP_EQUAL:
			printf("EQUAL");
			break;
		case OP_NEQUAL:
			printf("NEQUAL");
			break;
		case OP_STSUP:
			printf("STSUP");
			break;
		case OP_SUPEQ:
			printf("SUPEQ");
			break;
		case OP_STINF:
			printf("STINF");
			break;
		case OP_INFEQ:
			printf("INFEQ");
			break;
		case OP_GOTO:
			printf("GOTO");
			break;
		default:
			printf("\nOpérateur inconnu (pensez à l'ajouter dans print_quad...)\n");
			break;
	}
}
