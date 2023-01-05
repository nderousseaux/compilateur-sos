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
