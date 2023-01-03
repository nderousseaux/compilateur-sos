/* Génère le code mips de chaque quad
*/

// TODO(nderousseaux): tout relire

#include "../includes/imports.h"

FILE *f;  // Fichier de sortie

/* Traitement du quad OP_EXIT */
void gen_end(Quad quad) {
	// TODO(nderousseaux): Si le type est une variable ?? Possible ?
	// (operateur entier dans la grammaire)
    fprintf(f, "\t\t# On génère le code de terminaison\n");
    fprintf(f, "\t\tli\t$a0,\t%d\n", quad.result.value_int);
    fprintf(f, "\t\tli\t$v0,\t17\n");
    fprintf(f, "\t\tsyscall\n");
}

/* Traitement du quad OP_ASSIGN */
void gen_assign(Quad quad) {
    // On teste le type du premier opérande
    switch (quad.operand1.type) {
    case INTEGER_T:
        // TODO(nderousseaux): Factoriser ?
        fprintf(f, "\t\t# On met %d dans %s\n",
            quad.operand1.value_int,
            quad.result.symbol->name);
        fprintf(f, "\t\tli\t$t2,\t%d\n", quad.operand1.value_int);
        fprintf(f, "\t\tsw\t$t2,\t%d($fp)\n", quad.result.symbol->position);
        break;

    // TODO(nderousseaux): Gérer les autres cas (const ou id)
    default:
        break;
    }
}

/* Traitement du quad OP_ECHO */
void gen_echo(Quad quad) {
    // On teste le type du premier opérande
    switch (quad.result.type) {
    case INTEGER_T:
        fprintf(f, "\t\t# On affiche %d\n", quad.result.value_int);
        fprintf(f, "\t\tli\t$a0,\t%d\n", quad.result.value_int);
        fprintf(f, "\t\tli\t$v0,\t1\n");
        fprintf(f, "\t\tsyscall\n");
        break;
    case ID_T:
        fprintf(f, "\t\t# On affiche %s\n", quad.result.symbol->name);
        fprintf(f, "\t\tlw\t$a0,\t%d($fp)\n", quad.result.symbol->position);
        fprintf(f, "\t\tli\t$v0,\t1\n");
        fprintf(f, "\t\tsyscall\n");
        break;
    default:
        break;
    }
}
