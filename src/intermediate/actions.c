/* Toutes les actions associées aux règles de grammaire */

#include "../includes/imports.h"

/* Génère le quad exit */
void gencode_exit(Operand *op) {
	if (op == NULL)
		gencode(OP_EXIT, empty(), empty(), integer(0));
	else
		gencode(OP_EXIT, empty(), empty(), *op);
}

/* Génère le quad d'association */
void gencode_assign(char * dst, Operand * src) {
	check_id(dst);

	add_var_st(dst, src->type);

	Operand id_dst = id(dst);

	// Si l'opérande est une constante, on stocke vers quelle constante pointe l'id
	if(src->type == CONST_T) {
		// Si on peut le convertir en entier, on le fait
		if (is_int(src->symbol->data)) {
			to_operand_int(src, src->symbol->data);
			id_dst.symbol->type_data = INTEGER_T;
		} else {
			id_dst.symbol->constant = src->symbol;
		}
	}

	// Si c'est un id pointant vers une constante
	if (src->type == ID_T && src->symbol->type_data == CONST_T) {
		// Si on peut le convertir en entier, on le fait
		if (is_int(src->symbol->constant->data)) {
			to_operand_int(src, src->symbol->constant->data);
			id_dst.symbol->type_data = INTEGER_T;
		} else {
			id_dst.symbol->constant = src->symbol->constant;
			id_dst.symbol->type_data = CONST_T;
		}
	}

	// On génère le quad
	gencode(OP_ASSIGN, *src, empty(), id_dst);
}

/* Génère le code pour associer un tableau */
void gencode_assign_tab(char * dst, Operand * index, Operand * src) {
	Operand tab = empty();
	to_operand_id(&tab, dst, 0);
	tab.type = TAB_T;

	// On transforme src en operande entier
	if (src->type == CONST_T) {
		if (is_int(src->symbol->data))
			to_operand_int(src, src->symbol->data);
	}

	if(index->type != ID_T) {
		to_operand_temp(index);
		gencode(OP_ASSIGN, integer(index->value_int), empty(), *index);
	}


	// On veut avoir la case index, du tableau dst. On y stocke src
	gencode(OP_ASSIGN_TAB, *src, *index, tab);
}

/* Génère le code pour associer un tableau à une var
* renvoie une opérande temporaire contenant la valeur du tableau
*/
Operand * gencode_tab_to_temp(char * name, Operand * index) {
	Operand *op = malloc(sizeof(Operand));
	to_operand_temp(op);
	op->symbol->type_data = CONST_T;

	Operand * tab = malloc(sizeof(Operand));
	tab->type = TAB_T;
	tab->symbol = get_st(symbols_table, name);
	if(index->type != ID_T) {
		to_operand_temp(index);
		gencode(OP_ASSIGN, integer(index->value_int), empty(), *index);
	}
	tab->value_int = index->symbol->position;

	// On assigne la valeur du tableau à l'opérande temporaire
	gencode(OP_ASSIGN, *tab, empty(), *op);

	return op;
}

/* Génère le code pour déclarer un tableau */
void gencode_decla_tab(char * name, char * size) {
	check_id(name);

	int size_i;
	// On vérifie que la value est positif strictement
	if ((size_i = to_int(size)) < 1) {
		fprintf(stderr,
			"Erreur: La taille du tableau doit être supérieure à 0\n");
		exit(EXIT_FAILURE);
	}

	// On ajoute le tableau à la table des symboles
	add_tab_st(name, size_i);
}

/* Génère le quad d'affichage */
void gencode_echo(Op_list *op_list) {
	// Pour chaque opérande de la liste, on génère un quad echo
	for (int i = 0; i < op_list->size; i++) {
		if (op_list->data[i].symbol->type == TAB_S) {
			fprintf(stderr,
				"Erreur: Impossible d'afficher un tableau\n");
			exit(EXIT_FAILURE);
		}


		gencode(OP_ECHO, empty(), empty(), op_list->data[i]);
	}
}

/* Génère le code relatif à une opération */
Operand * gencode_operation(Operator operator, Operand * op1, Operand * op2) {
	Operand *res = malloc(sizeof(Operand));
	to_operand_temp(res);

	// On gère le cas du moins devant un entier
	if (op1 == NULL && operator == OP_MINUS)
		gencode(operator,  empty(), *op2, *res);  // On génère le quad
	else
		gencode(operator, *op1, *op2, *res);  // On génère le quad

	return res;
}

/* Génère le code relatif à une instruction if */
void gencode_if(
	Ctrl_ql * test_block,  // Contient les quads du test
	int first_true,  // Index du premier quad vrai
	Ql * list_false,  // Goto vers le premier quad si faux
	Ql * else_part) {  // Quads de l'instruction else
	list_false = concat(else_part, list_false);
	complete(list_false, nextquad());
	complete(test_block->fal, last_quad_idx(list_false)+1);
	complete(test_block->tru, first_true);
}

/* Génère le code relatif à une instruction elif */
Ql * gencode_elif(
	Ctrl_ql * test_block,  // Contient les quads du test
	int first_true,  // Index du premier quad vrai
	Ql * list_false,  // Goto vers le premier quad si faux
	Ql * else_part) {  // Quads de l'instruction else
	Ql * res = concat(else_part, list_false);
	complete(test_block->fal, last_quad_idx(list_false)+1);
	complete(test_block->tru, first_true);
	return res;
}


/* Génère le code relatif à un test */
Ctrl_ql * gencode_test(
	Operator operator, Operand * op1, Operand * op2) {
	// On génère le quad, la destination (result) sera déterminée plus tard
	Ctrl_ql * res;
	CHECK(res = malloc(sizeof(res)));
	Quad * t;
	if(op2 == NULL)
		t = gencode(operator, *op1, empty(), empty());
	else
		t = gencode(operator, *op1, *op2, empty());
	res->tru = create_list(t);

	// On génère le quad faux, la destination sera déterminée plus tard
	Quad * f = gencode(OP_GOTO, empty(), empty(), empty());
	res->fal = create_list(f);
	return res;
}

/* Génère le code relatif à une opération OR */
void gencode_or(
	Ctrl_ql * test_expr, Ctrl_ql * test_expr2, int first_true, Ctrl_ql * res) {
	complete(test_expr->fal, first_true);
	res->fal = test_expr2->fal;
	res->tru = concat(test_expr->tru, test_expr2->tru);
}

/* Génère le code relatif à une opération AND */
void gencode_and(
	Ctrl_ql * test_expr, Ctrl_ql * test_expr2, int first_true, Ctrl_ql * res) {
	complete(test_expr->tru, first_true);
	res->fal = concat(test_expr->fal, test_expr2->fal);
	res->tru = test_expr2->tru;
}

/* Génère le code relatif à une opération NOT */
void gencode_not(Ctrl_ql * test_expr, Ctrl_ql * res) {
	res->fal = test_expr->tru;
	res->tru = test_expr->fal;
}

/* Génère le code relatif à une instruction while */
Ql * gencode_while(
	Ctrl_ql * test_block,  // Contient les quads du test
	int first_cond,  // index du premier quad de la condition
	int first_true) {  // Index du premier quad vrai
	gencode(OP_GOTO, empty(), empty(), integer(first_cond));
	complete(test_block->fal, nextquad());
	complete(test_block->tru, first_true);
	return test_block->fal;
}

/* Génère le code relatif à une instruction until */
Ql * gencode_until(
	Ctrl_ql * test_block,  // Contient les quads du test
	int first_cond,  // index du premier quad de la condition
	int first_true) {  // Index du premier quad vrai
	gencode(OP_GOTO, empty(), empty(), integer(first_cond));
	complete(test_block->tru, nextquad());
	complete(test_block->fal, first_true);
	return test_block->tru;
}

/* Concatène deux opérandes de string */
Operand * gencode_concat(Operand * op1, Operand * op2) {
	// On concatène les deux strings
	char * str;
	CHECK(str = calloc(
		sizeof(char),
		(strlen(op1->symbol->name) + strlen(op2->symbol->name) + 1)));
	sprintf(str, "%s%s", str_of_const(op1), str_of_const(op2));

	// On crée un nouveau symbole
	Operand * res = malloc(sizeof(Operand));
	to_operand_const(res, str);
	return res;
}

/* Génère le code pour le début de for */
Ctrl_for * gencode_start_for(char *id_name, Op_list * op_list) {
	Ctrl_for *list = malloc(sizeof(Ctrl_for));
	CHECK(list);

	// On crée le tableau
	int size = op_list->size;
	Symbol *temp_id = add_temp_st();
	add_tab_st(temp_id->name, size);
	for (int i = 0; i < size; i++) {
		Operand op = integer(i);
		gencode_assign_tab(temp_id->name, &op, &op_list->data[i]);
	}

	list->temp_name = calloc(100, sizeof(char));
	CHECK(list->temp_name);
	Symbol *compteur = add_temp_st();
	Operand compteur_op = id(compteur->name);
	strcpy(list->temp_name, compteur->name);

	gencode(OP_ASSIGN, integer(0), empty(), compteur_op);

	// On crée le mot
	add_var_st(id_name, CONST_T);

	list->q_index = nextquad();
	Operand idd = id(list->temp_name);
	Quad * test_for = gencode(
		OP_STSUP, idd, integer(op_list->size-1), empty());


	Ql * need_complete = init_quad_list();
	add_quad(need_complete, test_for);

	list->q_test = need_complete;

	// On assigne tab[i] à mot ---> mot = tab[i]
	Operand *op_tab = malloc(sizeof(Operand));
	op_tab->symbol = get_st(symbols_table, temp_id->name);  // tableau
	op_tab->value_int = compteur->position;			   // index
	op_tab->type = TAB_T;						   // type

	Operand d = id(id_name);
	d.symbol->type_data = TAB_T;
	gencode(OP_ASSIGN, *op_tab, empty(), d);

	// i += 1
	gencode(OP_ADD, id(list->temp_name), integer(1), id(list->temp_name));

	return list;
}

void gencode_for(Ctrl_for *list) {
	gencode(OP_GOTO, empty(), empty(), integer(list->q_index));
	complete(list->q_test, nextquad());
}
