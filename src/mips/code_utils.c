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
    fprintf(f, "\tadd\t$%s,\t$%s,\t$%s\n", res, reg1, reg2);
}

/* Fait une soustraction */
void sub(char *res, char *reg1, char *reg2) {
    fprintf(f, "\tsub\t$%s,\t$%s,\t$%s\n", res, reg1, reg2);
}

/* Fait une multiplication */
void mul(char *res, char *reg1, char *reg2) {
    fprintf(f, "\tmul\t$%s,\t$%s,\t$%s\n", res, reg1, reg2);
}

/* Fait une division */
void divi(char *res, char *reg1, char *reg2) {
    fprintf(f, "\tdiv\t$%s,\t$%s\n", reg1, reg2);
    fprintf(f, "\tmflo\t$%s\n", res);
}

/* Fait un modulo */
void mod(char *res, char *reg1, char *reg2) {
    fprintf(f, "\tdiv\t$%s,\t$%s\n", reg1, reg2);
    fprintf(f, "\tmfhi\t$%s\n", res);
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
