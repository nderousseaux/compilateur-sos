/* Module contenant les fonctions pour simplifier 
* l'écriture du code assembleur
*/

#include "../includes/imports.h"

FILE *f;  // Fichier de sortie

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
void syscall_echo(Type_operand type) {
    if (type == INTEGER_T)
        fprintf(f, "\t\tli\t$v0,\t1\n");
    else if (type == CONST_T)
        fprintf(f, "\t\tli\t$v0,\t4\n");
    fprintf(f, "\t\tsyscall\n");
}