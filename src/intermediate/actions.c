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
		id_dst.symbol->constant = src->symbol;
	}

	// Si c'est un id pointant vers une constante
	if (src->type == ID_T && src->symbol->type_data == CONST_T) {
		id_dst.symbol->constant = src->symbol->constant;
		id_dst.symbol->type_data = CONST_T;
	}

	// On génère le quad
	gencode(OP_ASSIGN, *src, empty(), id_dst);
}

/* Génère le quad d'affichage */
void gencode_echo(Op_list *op_list) {
	// Pour chaque opérande de la liste, on génère un quad echo
	for (int i = 0; i < op_list->size; i++) {
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
