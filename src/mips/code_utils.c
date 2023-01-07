/* Module contenant les fonctions pour simplifier 
* l'écriture du code assembleur
*/

#include "../includes/imports.h"

extern FILE *f;  // Fichier de sortie

/* Met une opérande dans un registre */
void put_op_reg(Operand * op, char * reg) {
    switch (op->type) {
    case INTEGER_T:
        put_int_reg(op->value_int, reg);
        break;
    case CONST_T:
        put_const_reg(op->symbol->name, reg);
        break;
    case ID_T:
        put_var_reg(op->symbol->position, reg);
        break;
    case TEMP_T:
        put_var_reg(op->symbol->position, reg);
        break;
    case EMPTY_T:
        put_int_reg(0, reg);
        break;
    default:
        break;
    }
}

/* Met un entier dans un registre  */
void put_int_reg(int value, char *reg) {
    fprintf(f, "\t\tli\t$%s,\t%d\n", reg, value);
}

/* Met une variable de la pile dans un registre */
void put_var_reg(int pos, char *reg) {
    fprintf(f, "\t\tlw\t$%s,\t%d($fp)\n", reg, pos);
}

/* Met l'adresse d'une constante dans un registre */
void put_const_reg(char *name, char *reg) {
    fprintf(f, "\t\tla\t$%s,\t%s\n", reg, name);
}

/* Met un registre dans la pile */
void put_reg_var(char *reg, int pos) {
    fprintf(f, "\t\tsw\t$%s,\t%d($fp)\n", reg, pos);
}

/* Fait une addition */
void add(char *res, char *reg1, char *reg2) {
    fprintf(f, "\t\tadd\t$%s,\t$%s,\t$%s\n", res, reg1, reg2);
}

/* Fait une soustraction */
void sub(char *res, char *reg1, char *reg2) {
    fprintf(f, "\t\tsub\t$%s,\t$%s,\t$%s\n", res, reg1, reg2);
}

/* Fait une multiplication */
void mul(char *res, char *reg1, char *reg2) {
    fprintf(f, "\t\tmul\t$%s,\t$%s,\t$%s\n", res, reg1, reg2);
}

/* Fait une division */
void divi(char *res, char *reg1, char *reg2) {
    fprintf(f, "\t\tdiv\t$%s,\t$%s\n", reg1, reg2);
    fprintf(f, "\t\tmflo\t$%s\n", res);
}

/* Fait un modulo */
void mod(char *res, char *reg1, char *reg2) {
    fprintf(f, "\t\tdiv\t$%s,\t$%s\n", reg1, reg2);
    fprintf(f, "\t\tmfhi\t$%s\n", res);
}

/* Jump vers un quad */
void jump(int quad) {
    fprintf(f, "\t\tj\tquad_%d\n", quad);
}

/* Jump si égal */
void jeq(int quad, char *reg1, char *reg2) {
    fprintf(f, "\t\tbeq\t$%s,\t$%s,\tquad_%d\n", reg1, reg2, quad);
}

/* Jump si différent */
void jne(int quad, char *reg1, char *reg2) {
    fprintf(f, "\t\tbne\t$%s,\t$%s,\tquad_%d\n", reg1, reg2, quad);
}

/* Jump si supérieur */
void jgt(int quad, char *reg1, char *reg2) {
    fprintf(f, "\t\tbgt\t$%s,\t$%s,\tquad_%d\n", reg1, reg2, quad);
}

/* Jump si supérieur ou égal */
void jge(int quad, char *reg1, char *reg2) {
    fprintf(f, "\t\tbge\t$%s,\t$%s,\tquad_%d\n", reg1, reg2, quad);
}

/* Jump si inférieur */
void jlt(int quad, char *reg1, char *reg2) {
    fprintf(f, "\t\tblt\t$%s,\t$%s,\tquad_%d\n", reg1, reg2, quad);
}

/* Jump si inférieur ou égal */
void jle(int quad, char *reg1, char *reg2) {
    fprintf(f, "\t\tble\t$%s,\t$%s,\tquad_%d\n", reg1, reg2, quad);
}

/* Appelle la primitive EXIT 
* La valeur de sortie doit être définie dans $a0
*/
void syscall_exit() {
    fprintf(f, "\t\tli\t$v0,\t17\n");
    fprintf(f, "\t\tsyscall\n");
}

/* Appelle la primitive ECHO
* La valeur à afficher doit être définie dans $a0
*/
void syscall_echo(Operand *op) {
    if (op->type == CONST_T)
        fprintf(f, "\t\tli\t$v0,\t4\n");
    else if (op->type == ID_T && op->symbol->type_data == CONST_T)
        fprintf(f, "\t\tli\t$v0,\t4\n");
    else
        fprintf(f, "\t\tli\t$v0,\t1\n");
    fprintf(f, "\t\tsyscall\n");
}
