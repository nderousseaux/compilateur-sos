/* Module recensant les opérandes
 et leurs fonctions associées
*/

#include "../includes/imports.h"

extern St * symbols_table;

/* Crée une liste d'opérande */
Op_list *init_op_list() {
	Op_list *op_list;
	CHECK(op_list = malloc(sizeof(Op_list)));
	op_list->size = 0;
	op_list->capacity = INIT_OP_LIST_CAPACITY;

	CHECK(op_list->data = calloc(op_list->capacity, sizeof(Operand)));
	return op_list;
}

/* Crée une liste d'opérande avec une première opérande */
Op_list *create_list_op(Operand *op) {
	Op_list *op_list = init_op_list();
	add_op(op_list, op);
	return op_list;
}

/* Ajoute une opérande dans une liste */
Op_list * add_op(Op_list *op_list, Operand *op) {
	if (op_list->size == op_list->capacity) {
		op_list->capacity *= 2;
		CHECK(
			op_list->data = realloc(
				op_list->data, op_list->capacity * sizeof(Operand)));
	}
	op_list->data[op_list->size++] = *op;
	return op_list;
}

/* Affiche une liste d'opérande */
void print_op_list(Op_list *op_list) {
	printf("\n──────────────── Liste des opérandes ────────────────\n");
	printf("Nombre d'éléments : %d\n", op_list->size);
	printf("Taille de la table : %d\n\n", op_list->capacity);

	for (int i = 0; i < op_list->size; i++) {
		printf("Case n°%d:\n", i);
		print_operand(op_list->data[i]);
	}
}

/* Concatène deux listes d'opérandes */
Op_list *concat_op(Op_list *op_list1, Op_list *op_list2) {
	Op_list *op_list = op_list1;
	for (int i = 0; i < op_list2->size; i++) {
		add_op(op_list, &op_list2->data[i]);
	}
	destroy_op_list(op_list2);

	return op_list;
}

/* Supprime une liste d'opérandes */
void destroy_op_list(Op_list *op_list) {
	free(op_list->data);
	free(op_list);
}

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

/* Transforme une chaine de caractère en operande id 
* Si convert_int est 1, on force la tranformation de l'opérande en entier 
*/
void to_operand_id(Operand *op, char *value, char convert_int) {
	op->symbol = get_st(symbols_table, value);
	op->type = ID_T;

	if(op->symbol == NULL) {
		fprintf(stderr, "Erreur : variable %s non déclarée\n", value);
		exit(EXIT_FAILURE);
	}

	// Si id est de type const
	// alors on crée une variable temporaire
	// et on lui assigne la valeur de id
	if (convert_int) {
		if (op->symbol->type_data == CONST_T) {
			char * name_const = op->symbol->constant->data;
			int value = to_int(name_const);
			to_operand_temp(op);
			gencode(OP_ASSIGN, integer(value), empty(), *op);
		}
	}
}

/* Transforme une chaine de caractère en operande constante */
void to_operand_const(Operand *op, char *value) {
	op->symbol = add_const_st(value);
	op->type = CONST_T;
}

/* Crée operande de type variable temporaire */
void to_operand_temp(Operand *op) {
	op->symbol = add_temp_st();
	op->type = TEMP_T;
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
		case TEMP_T:
			printf("TEMP\t%s", op.symbol->name);
			break;
		default:
			printf("\nType de l'opérande inconnu");
			printf(" (pensez à l'ajouter dans print_operand...)\n");
			break;
	}
}

/* Renvoie une valeur printable de l'opérande 
Valeur si entier, nom si autre */
char *printable_operand(Operand op) {
	if (op.type == INTEGER_T) {
		char *str;
		CHECK(str = calloc(sizeof(char), 10));
		sprintf(str, "%d", op.value_int);
		return str;
	} else if (op.type == EMPTY_T) {
		return "";
	} else {
		return op.symbol->name;
	}
}

/* Copie une operande */
Operand * copy_operand(Operand * op) {
	Operand * new_op = malloc(sizeof(Operand));
	new_op->type = op->type;
	new_op->symbol = op->symbol;
	new_op->value_int = op->value_int;
	return new_op;
}

/* Renvoie le string d'une constante
* Directement si c'est une constante
* Si c'est un id, on renvoie la chaine pointée par l'id
*/
char * str_of_const(Operand * op) {
	if (op->type == CONST_T) {
		return op->symbol->data;
	} else if (op->type == ID_T && op->symbol->type_data == CONST_T) {
		return op->symbol->constant->data;
	} else {
		fprintf(stderr, "Erreur : l'opérande n'est pas une constante\n");
		exit(EXIT_FAILURE);
	}
}

/* Renvoie vrai si l'operande est une constante
* Ou une variable de type constante
*/
char is_const(Operand * op) {
	if(op->type == CONST_T)
		return 1;
	else if (op->type == ID_T && op->symbol->type_data == CONST_T)
		return 1;
	else
		return 0;
}
