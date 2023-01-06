/* Génère le code mips de chaque quad
*/

#include "../includes/imports.h"

extern FILE *f;  // Fichier de sortie

/* Traitement du quad OP_EXIT */
void gen_end(Quad * quad) {
    fprintf(f, "\t\t# On génère le code de terminaison\n");
    put_int_reg(quad->result.value_int, "a0");
    syscall_exit();
}

/* Traitement du quad OP_ASSIGN */
void gen_assign(Quad * quad) {
    fprintf(
        f,
        "\t\t# On met %s dans %s\n",
        printable_operand(quad->operand1),
        printable_operand(quad->result));

    // On met l'opérande dans t2
    put_op_reg(&quad->operand1, "t2");

    // On met t2 dans la variable
    put_reg_var("t2", quad->result.symbol->position);
}

/* Traitement du quad OP_ECHO */
void gen_echo(Quad * quad) {
    fprintf(f, "\t\t# On affiche %s\n", printable_operand(quad->result));

    // On met la valeur à afficher dans a0
    put_op_reg(&quad->result, "a0");
    syscall_echo(&quad->result);
}

/* Traitement du quad OP_ADD */
void gen_add(Quad * quad) {
    fprintf(
        f,
        "\t\t# On met %s + %s dans %s\n",
        printable_operand(quad->operand1),
        printable_operand(quad->operand2),
        printable_operand(quad->result));

    // On met l'opérande 1 dans t0
    put_op_reg(&quad->operand1, "t0");

    // On met l'opérande 2 dans t
    put_op_reg(&quad->operand2, "t1");

    // On fait l'addition (t0 = t0 + t1)
    add("t0", "t0", "t1");

    // On charge t0 dans la variable
    put_reg_var("t0", quad->result.symbol->position);
}

/* Traitement du quad OP_SUB */
void gen_sub(Quad * quad) {
    fprintf(
        f,
        "\t\t# On met %s - %s dans %s\n",
        printable_operand(quad->operand1),
        printable_operand(quad->operand2),
        printable_operand(quad->result));


    // On met l'opérande 1 dans t0
    put_op_reg(&quad->operand1, "t0");

    // On met l'opérande 2 dans t
    put_op_reg(&quad->operand2, "t1");

    // On fait la soustraction (t0 = t0 - t1)
    sub("t0", "t0", "t1");

    // On charge t0 dans la variable
    put_reg_var("t0", quad->result.symbol->position);
}

/* Traitement du quad OP_MUL */
void gen_mul(Quad * quad) {
    fprintf(
        f,
        "\t\t# On met %s * %s dans %s\n",
        printable_operand(quad->operand1),
        printable_operand(quad->operand2),
        printable_operand(quad->result));


    // On met l'opérande 1 dans t0
    put_op_reg(&quad->operand1, "t0");

    // On met l'opérande 2 dans t
    put_op_reg(&quad->operand2, "t1");

    // On fait la soustraction (t0 = t0 * t1)
    mul("t0", "t0", "t1");

    // On charge t0 dans la variable
    put_reg_var("t0", quad->result.symbol->position);
}

/* Traitement du quad OP_DIV */
void gen_div(Quad * quad) {
    fprintf(
        f,
        "\t\t# On met %s / %s dans %s\n",
        printable_operand(quad->operand1),
        printable_operand(quad->operand2),
        printable_operand(quad->result));


    // On met l'opérande 1 dans t0
    put_op_reg(&quad->operand1, "t0");

    // On met l'opérande 2 dans t
    put_op_reg(&quad->operand2, "t1");

    // On fait la soustraction (t0 = t0 / t1)
    divi("t0", "t0", "t1");

    // On charge t0 dans la variable
    put_reg_var("t0", quad->result.symbol->position);
}

/* Traitement du quad OP_MOD */
void gen_mod(Quad * quad) {
    fprintf(
        f,
        "\t\t# On met %s // %s dans %s\n",
        printable_operand(quad->operand1),
        printable_operand(quad->operand2),
        printable_operand(quad->result));


    // On met l'opérande 1 dans t0
    put_op_reg(&quad->operand1, "t0");

    // On met l'opérande 2 dans t
    put_op_reg(&quad->operand2, "t1");

    // On fait la soustraction (t0 = t0 % t1)
    mod("t0", "t0", "t1");

    // On charge t0 dans la variable
    put_reg_var("t0", quad->result.symbol->position);
}

/* Traitement du quad OP_NOTEMPTY */
void gen_noempty(Quad * quad) {
    (void) quad;
}

/* Traitement du quad OP_EMPTY */
void gen_empty(Quad * quad) {
    (void) quad;
}

/* Traitement du quad OP_EQUAL */
void gen_equal(Quad * quad) {
    // TODO(nderousseaux) equal vaut aussi pour str ?
    fprintf(
        f,
        "\t\t# On saute au quad n°%d si %s == %s\n",
        quad->result.value_int,
        printable_operand(quad->operand1),
        printable_operand(quad->operand2));

    // On met l'opérande 1 dans t0
    put_op_reg(&quad->operand1, "t0");

    // On met l'opérande 2 dans t1
    put_op_reg(&quad->operand2, "t1");

    // Si t0 == t1, on saute au résultat
    jeq(quad->result.value_int , "t0", "t1");
}

/* Traitement du quad OP_NEQUAL */
void gen_nequal(Quad * quad) {
    // TODO(nderousseaux) nequal vaut aussi pour str ?
    fprintf(
        f,
        "\t\t# On saute au quad n°%d si %s != %s\n",
        quad->result.value_int,
        printable_operand(quad->operand1),
        printable_operand(quad->operand2));

    // On met l'opérande 1 dans t0
    put_op_reg(&quad->operand1, "t0");

    // On met l'opérande 2 dans t1
    put_op_reg(&quad->operand2, "t1");

    // Si t0 != t1, on saute au résultat
    jne(quad->result.value_int , "t0", "t1");
}

/* Traitement du quad OP_STSUP */
void gen_stsup(Quad * quad) {
    fprintf(
        f,
        "\t\t# On saute au quad n°%d si %s > %s\n",
        quad->result.value_int,
        printable_operand(quad->operand1),
        printable_operand(quad->operand2));

    // On met l'opérande 1 dans t0
    put_op_reg(&quad->operand1, "t0");

    // On met l'opérande 2 dans t1
    put_op_reg(&quad->operand2, "t1");

    // Si t0 > t1, on saute au résultat
    jgt(quad->result.value_int , "t0", "t1");
}

/* Traitement du quad OP_SUPEQ */
void gen_supeq(Quad * quad) {
    fprintf(
        f,
        "\t\t# On saute au quad n°%d si %s >= %s\n",
        quad->result.value_int,
        printable_operand(quad->operand1),
        printable_operand(quad->operand2));

    // On met l'opérande 1 dans t0
    put_op_reg(&quad->operand1, "t0");

    // On met l'opérande 2 dans t1
    put_op_reg(&quad->operand2, "t1");

    // Si t0 >= t1, on saute au résultat
    jge(quad->result.value_int , "t0", "t1");
}

/* Traitement du quad OP_STINF */
void gen_stinf(Quad * quad) {
    fprintf(
        f,
        "\t\t# On saute au quad n°%d si %s < %s\n",
        quad->result.value_int,
        printable_operand(quad->operand1),
        printable_operand(quad->operand2));

    // On met l'opérande 1 dans t0
    put_op_reg(&quad->operand1, "t0");

    // On met l'opérande 2 dans t1
    put_op_reg(&quad->operand2, "t1");

    // Si t0 < t1, on saute au résultat
    jlt(quad->result.value_int , "t0", "t1");
}

/* Traitement du quad OP_INFEQ */
void gen_infeq(Quad * quad) {
    fprintf(
        f,
        "\t\t# On saute au quad n°%d si %s <= %s\n",
        quad->result.value_int,
        printable_operand(quad->operand1),
        printable_operand(quad->operand2));

    // On met l'opérande 1 dans t0
    put_op_reg(&quad->operand1, "t0");

    // On met l'opérande 2 dans t1
    put_op_reg(&quad->operand2, "t1");

    // Si t0 <= t1, on saute au résultat
    jle(quad->result.value_int , "t0", "t1");
}

/* Traitement du quad OP_GOTO */
void gen_goto(Quad * quad) {
    fprintf(f, "\t\t# On saute au quad %d\n", quad->result.value_int);
    jump(quad->result.value_int);
}
