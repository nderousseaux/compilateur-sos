/* Module contenant les fonctions pour simplifier 
* l'écriture du code assembleur
*/

#include "../includes/imports.h"

extern FILE *f;  // Fichier de sortie

int nb_cmp_loop = 0;  // Nombre de boucles de comparaison

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
    case TAB_T:
        put_reg_var_tab(op, reg);
        break;
    case EMPTY_T:
        put_int_reg(0, reg);
        break;
    default:
        break;
    }
}

/* Met la valeur d'un tableau dans une opérande */
void put_reg_var_tab(Operand * op, char * reg) {
    // On met l'addresse de tab dans t0
    put_int_reg(op->symbol->position, "t0");
    // On met la valeur de de i dans t1
    fprintf(f, "\t\tlw\t$t1,\t%d($fp)\n", op->value_int);
    // On multiplie t1 par 4
    put_int_reg(4, "t3");
    mul("t1", "t1", "t3");
    // On ajoute t0 et t1
    add("t0", "t0", "t1");

    // On ajoute fp à t0
    add("t0", "t0", "fp");

    // On charge la valeur de t0 pointée par t0 dans le reg
    fprintf(f, "\t\tlw\t$%s,\t0($t0)\n", reg);
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

/* Met un octet dans un registre */
void put_oct_reg(char *dst, char *src) {
    fprintf(f, "\t\tlb\t$%s,\t($%s)\n", dst, src);
}

/* Fait une addition */
void add(char *res, char *reg1, char *reg2) {
    fprintf(f, "\t\tadd\t$%s,\t$%s,\t$%s\n", res, reg1, reg2);
}

/* Ajoute à une registre une valeur entière */
void addi(char *res, char *reg, int value) {
    fprintf(f, "\t\taddi\t$%s,\t$%s,\t%d\n", res, reg, value);
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

/* Jump si égal, str */
void cmp_str(int quad_true, int quad_false, char * reg1, char *reg2) {
    fprintf(f, "\t\t# Comparaison de chaine\n");
    fprintf(f, "\t\tcmploop_%d:\n", nb_cmp_loop);
    put_oct_reg("t2", reg1);
    put_oct_reg("t3", reg2);
    jne(quad_false, "t2", "t3");
    jeq(quad_true, "t2", "zero");
    addi(reg1, reg1, 1);
    addi(reg2, reg2, 1);
    fprintf(f, "\t\tj\tcmploop_%d\n", nb_cmp_loop);
    nb_cmp_loop++;
}

/* Jump si vide, str */
void cmp_empty_str(int dest, char * reg1) {
    fprintf(f, "\t\tbeqz\t$%s, quad_%d", reg1, dest);
}

/* Jump si non vide, str */
void cmp_not_empty_str(int dest, char * reg1) {
    fprintf(f, "\t\tbnez\t$%s, quad_%d", reg1, dest);
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
    else if (op->type == TEMP_T && op->symbol->type_data == CONST_T)
        fprintf(f, "\t\tli\t$v0,\t4\n");
    else
        fprintf(f, "\t\tli\t$v0,\t1\n");
    fprintf(f, "\t\tsyscall\n");
}
