/* Module recensant les opérandes
 et leurs fonctions associées
*/

#include "../includes/imports.h"

/* Crée une opérande de type id */
Operand id(char *value) {

	// On cherche le symbole dans la table des symboles
	Symbol *s = get_st(symbols_table, value);

	Operand o = {ID_T, s, 0};
	return o;
}

/* Crée une opérande de type const */
Operand constant(char *name) {
	// On cherche le symbole dans la table des symboles
	Symbol *s = get_st(symbols_table, name);
	Operand o = {CONST_T, s, 0};
	return o;
}

/* Crée une opérande de type int */
Operand integer(int value) {
	Operand o = {INTEGER_T, NULL, value};
	return o;
}

/* Crée une opérande de type empty (pas d'operande) */
Operand empty() {
	Operand o = {EMPTY_T, NULL, 0};
	return o;
}

/* Transforme une chaine de caractère en operande entier */
void to_operand_int(Operand *op, char *value) {
	op->value_int = to_int(value);
	op->type = INTEGER_T;
}

/* Transforme une chaine de caractère en operande id */
void to_operand_id(Operand *op, char *value) {
	op->symbol = get_st(symbols_table, value);
	op->type = ID_T;
}

/* Transforme une chaine de caractère en operande constante */
void to_operand_const(Operand *op, char *value) {
	op->symbol = add_const_st(value);
	op->type = CONST_T;
}

/* Affiche une opérande */
void print_operand(Operand op) {
	switch (op.type) {
		case ID_T:
			printf("ID\t%s", op.symbol->name);
			break;
		case CONST_T:
			printf("CONST\t%s", op.symbol->name);
			break;
		case INTEGER_T:
			printf("INT\t%d", op.value_int);
			break;
		case EMPTY_T:
			printf("EMPTY");
			break;
		default:
			printf("\nType de l'opérande inconnu");
			printf(" (pensez à l'ajouter dans print_operand...)\n");
			break;
	}
}
