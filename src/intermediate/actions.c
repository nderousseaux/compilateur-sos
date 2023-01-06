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
void gencode_assign(char * dst, Op_list * src) {
	// On vérifie que la liste ne contient qu'une seule opérande
	if (src->size > 1) {
		fprintf(stderr,
		"Erreur: l'opérande de droite de l'assignation contient plus d'une opérande");
		exit(EXIT_FAILURE);
	}
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

/* Génère le code relatif à une opération */
Operand * gencode_operation(Operator operator, Operand * op1, Operand * op2) {
	Operand *res = malloc(sizeof(Operand));
	to_operand_temp(res);

	// On génère le quad
	gencode(operator, *op1, *op2, *res);

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
void gencode_test(
	Operator operator, Operand * op1, Operand * op2, Ctrl_ql * res) {
	// On génère le quad, la destination (result) sera déterminée plus tard
	Quad * t = gencode(operator, *op1, *op2, empty());
	res->tru = create_list(t);

	// On génère le quad faux, la destination sera déterminée plus tard
	Quad * f = gencode(OP_GOTO, empty(), empty(), empty());
	res->fal = create_list(f);
}
