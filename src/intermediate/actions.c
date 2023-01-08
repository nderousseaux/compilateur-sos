/* Toutes les actions associées aux règles de grammaire */

#include "../includes/imports.h"

/* Génère le quad exit */
void gencode_exit(Operand *op)
{
	if (op == NULL)
		gencode(OP_EXIT, empty(), empty(), integer(0));
	else
		gencode(OP_EXIT, empty(), empty(), *op);
}

/* Génère le quad d'association */
void gencode_assign(char *dst, Op_list *src)
{
	// On vérifie que la liste ne contient qu'une seule opérande
	if (src->size > 1)
	{
		fprintf(stderr,
				"Erreur: l'opérande de droite de l'assignation contient plus d'une opérande\n");
		exit(EXIT_FAILURE);
	}

	check_id(dst);

	add_var_st(dst, src->data[0].type);

	Operand id_dst = id(dst);

	// Si l'opérande est une constante, on stocke vers quelle constante pointe l'id
	if (src->data[0].type == CONST_T)
	{
		id_dst.symbol->constant = src->data[0].symbol;
	}

	// Si c'est un id pointant vers une constante
	if (src->data[0].type == ID_T && src->data[0].symbol->type_data == CONST_T)
	{
		id_dst.symbol->constant = src->data[0].symbol->constant;
		id_dst.symbol->type_data = CONST_T;
	}

	// On génère le quad
	gencode(OP_ASSIGN, src->data[0], empty(), id_dst);
}

/* Génère le quad d'affichage */
void gencode_echo(Op_list *op_list)
{
	// Pour chaque opérande de la liste, on génère un quad echo
	for (int i = 0; i < op_list->size; i++)
	{
		gencode(OP_ECHO, empty(), empty(), op_list->data[i]);
	}
}

/* Génère le code relatif à une opération */
Operand *gencode_operation(Operator operator, Operand * op1, Operand *op2)
{
	Operand *res = malloc(sizeof(Operand));
	to_operand_temp(res);

	// On gère le cas du moins devant un entier
	if (op1 == NULL && operator== OP_MINUS)
		gencode(operator, empty(), *op2, *res); // On génère le quad
	else
		gencode(operator, * op1, *op2, *res); // On génère le quad

	return res;
}

/* Génère le code relatif à une instruction if */
void gencode_if(
	Ctrl_ql *test_block, // Contient les quads du test
	int first_true,		 // Index du premier quad vrai
	Ql *list_false,		 // Goto vers le premier quad si faux
	Ql *else_part)
{ // Quads de l'instruction else
	list_false = concat(else_part, list_false);
	complete(list_false, nextquad());
	complete(test_block->fal, last_quad_idx(list_false) + 1);
	complete(test_block->tru, first_true);
}

/* Génère le code relatif à une instruction elif */
Ql *gencode_elif(
	Ctrl_ql *test_block, // Contient les quads du test
	int first_true,		 // Index du premier quad vrai
	Ql *list_false,		 // Goto vers le premier quad si faux
	Ql *else_part)
{ // Quads de l'instruction else
	Ql *res = concat(else_part, list_false);
	complete(test_block->fal, last_quad_idx(list_false) + 1);
	complete(test_block->tru, first_true);
	return res;
}

/* Génère le code relatif à un test */
void gencode_test(
	Operator operator, Operand * op1, Operand *op2, Ctrl_ql *res)
{
	// On génère le quad, la destination (result) sera déterminée plus tard
	Quad *t = gencode(operator, * op1, *op2, empty());
	res->tru = create_list(t);

	// On génère le quad faux, la destination sera déterminée plus tard
	Quad *f = gencode(OP_GOTO, empty(), empty(), empty());
	res->fal = create_list(f);
}

/* Génère le code relatif à une opération OR */
void gencode_or(
	Ctrl_ql *test_expr, Ctrl_ql *test_expr2, int first_true, Ctrl_ql *res)
{
	complete(test_expr->fal, first_true);
	res->fal = test_expr2->fal;
	res->tru = concat(test_expr->tru, test_expr2->tru);
}

/* Génère le code relatif à une opération AND */
void gencode_and(
	Ctrl_ql *test_expr, Ctrl_ql *test_expr2, int first_true, Ctrl_ql *res)
{
	complete(test_expr->tru, first_true);
	res->fal = concat(test_expr->fal, test_expr2->fal);
	res->tru = test_expr2->tru;
}

/* Génère le code relatif à une opération NOT */
void gencode_not(Ctrl_ql *test_expr, Ctrl_ql *res)
{
	res->fal = test_expr->tru;
	res->tru = test_expr->fal;
}

/* Génère le code relatif à la déclaration d'un tableau */
void gencode_tab(char *dst, char *value)
{
	// On vérifie que la value est positif strictement
	if (to_int(value) < 1)
	{
		fprintf(stderr,
				"Erreur: L'initialisation du tableau se doit d'être faite avec une valeur supérieure à 0\n");
		exit(EXIT_FAILURE);
	}
	int newValue = to_int(value);
	add_tableau_st(dst, newValue);
}

/* Génère le code relatif au remplissage d'un tableau */
void gencode_tab_assign(char *tabName, Operand *op, Op_list *src)
{
	Operand tab = empty();
	to_operand_id(&tab, tabName, 0);
	// On vérifie que le symbole d'entrée se doit d'être de type TAB_T, cas n°5 règle sémantique
	if (tab.symbol->type != TAB)
	{
		fprintf(stderr,
				"Erreur: la valeur d'opérande-entier se doit d'être composée uniquement de chiffre sans + ni -\n");
		exit(EXIT_FAILURE);
	}
	// On vérifie que la liste ne contient qu'une seule opérande
	if (src->size > 1)
	{
		fprintf(stderr,
				"Erreur: l'opérande de droite de l'assignation contient plus d'une opérande\n");
		exit(EXIT_FAILURE);
	}
	Operand op2_str = empty();
	// Forcer l'int a devenir un string pour pouvoir le echo
	if (src->data[0].type == INTEGER_T)
	{
		char *str_int;
		str_int = calloc(128, sizeof(char));
		sprintf(str_int, "\"%d\"", src->data[0].value_int);
		to_operand_const(&op2_str, str_int);
	}
	// Sinon on le garde tel quel
	else
	{
		to_operand_const(&op2_str, src->data[0].symbol->data);
	}

	if (op->value_int > tab.symbol->size - 1)
	{
		fprintf(stderr,
				"Erreur : le tableau ne possède pas cette case, erreur de dimension\n");
		exit(EXIT_FAILURE);
	}
	// on génère le quad
	gencode(OP_ASSIGN_TAB, op2_str, *op, integer(tab.symbol->position));
}

/* Génère le code relatif à une instruction while */
Ql *gencode_while(
	Ctrl_ql *test_block, // Contient les quads du test
	int first_cond,		 // index du premier quad de la condition
	int first_true)
{ // Index du premier quad vrai
	gencode(OP_GOTO, empty(), empty(), integer(first_cond));
	complete(test_block->fal, nextquad());
	complete(test_block->tru, first_true);
	return test_block->fal;
}

/* Génère le code relatif à une instruction until */
Ql *gencode_until(
	Ctrl_ql *test_block, // Contient les quads du test
	int first_cond,		 // index du premier quad de la condition
	int first_true)
{ // Index du premier quad vrai
	gencode(OP_GOTO, empty(), empty(), integer(first_cond));
	complete(test_block->tru, nextquad());
	complete(test_block->fal, first_true);
	return test_block->tru;
}

/* génére le code MIPS pour un crée un tableau si on a une liste opérande
La valeur de retour est le nom du tableau sur lequel ont travail*/
char *gencode_tab_list_operand(Op_list *op_list)
{
	if (op_list->name_id != NULL)
	{
		return op_list->name_id; // On check si on a besoin de créer un tableau
	}

	int size = op_list->size;
	Symbol *temp_id = add_temp_st();

	add_tableau_st(temp_id->name, size);

	for (int i = 0; i < size; i++)
	{
		Operand op = integer(i);
		gencode_tab_assign(temp_id->name, &op, create_list_op(&op_list->data[i]));
	}

	return temp_id->name;
}

Ctrl_for *gencode_start_for(char *id_name, Op_list *op_list)
{
	Ctrl_for *list = malloc(sizeof(Ctrl_for));

	CHECK(list);

	// On crée le tableau
	char *tab = gencode_tab_list_operand(op_list);

	// On crée le compteur
	list->temp_name = calloc(100, sizeof(char));

	CHECK(list->temp_name);

	Symbol *compteur = add_temp_st();

	Operand compteur_op = id(compteur->name);

	strcpy(list->temp_name, compteur->name);

	gencode(OP_ASSIGN, integer(0), empty(), compteur_op);

	// On crée mot
	add_var_st(id_name, CONST_T);

	list->q_index = nextquad();
	Quad *test_for = gencode(OP_STSUP, id(list->temp_name), integer(op_list->size), empty()); // Dest à compléter

	Ql *need_complete = init_quad_list();
	add_quad(need_complete, test_for);

	list->q_test = need_complete;

	// On assigne tab[i] à mot ---> mot = tab[i]
	Operand *op_tab = malloc(sizeof(Operand));
	to_operand_tab(op_tab, tab, &compteur_op);

	gencode(OP_ASSIGN, *op_tab, empty(), id(id_name));

	// i += 1
	gencode(OP_ADD, id(list->temp_name), integer(1), id(list->temp_name));

	return list;
}

void gencode_for(Ctrl_for *list)
{
	gencode(OP_GOTO, empty(), empty(), integer(list->q_index));
	complete(list->q_test, nextquad());
}

/*

for mot in TAB = (A|B|C) do
echo mot
done



# START FOR

CREER TAB -> FAIT

CREER COMPTEUR I; -> FAIT
i = 0; -> FAIT

CREER MOT; -> FAIT

if i > 2 jump end 			--> flag_if
mot = TAB[i] -> FAIT
i = i + 1 -> FAIT

LISTE INSTRUCTION
echo mot


goto flag_if

end


*/