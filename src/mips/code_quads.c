/* Génère le code mips de chaque quad
*/

#include "../includes/imports.h"

FILE *f;  // Fichier de sortie

/* Traitement du quad OP_EXIT */
void gen_end(Quad quad) {
    fprintf(f, "\t\t# On génère le code de terminaison\n");
    put_int_reg(quad.result.value_int, "a0");
    syscall_exit();
}

/* Traitement du quad OP_ASSIGN */
void gen_assign(Quad quad) {
    fprintf(
        f,
        "\t\t# On met %s dans %s\n",
        printable_operand(quad.operand1),
        printable_operand(quad.result));

    // On teste le type du premier opérande
    switch (quad.operand1.type) {
    case INTEGER_T:
        put_int_reg(quad.operand1.value_int, "t2");
        break;
    case CONST_T:
        put_const_reg(quad.operand1.symbol->name, "t2");
        break;
    default:
        break;
    }
    put_reg_var("t2", quad.result.symbol->position);
}

/* Traitement du quad OP_ECHO */
void gen_echo(Quad quad) {
    fprintf(f, "\t\t# On affiche %s\n", printable_operand(quad.result));
    switch (quad.result.type) {
    case INTEGER_T:
        put_int_reg(quad.result.value_int, "a0");
        syscall_echo(quad.result.type);
        break;
    case ID_T:
        put_var_reg(quad.result.symbol->position, "a0");
        syscall_echo(quad.result.symbol->type_data);
        break;
    case CONST_T:
        put_const_reg(quad.result.symbol->name, "a0");
        syscall_echo(quad.result.type);
        break;
    default:
        break;
    }
    // TODO(nderousseaux): vérifier la ligne du haut après concaténation
    // On peut peut être encore factoriser les s
}
