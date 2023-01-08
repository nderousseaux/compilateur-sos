/* Module contenant les fonctions pour simplifier 
* l'écriture du code assembleur
*/

#ifndef SRC_INCLUDES_MIPS_CODE_UTILS_H_
#define SRC_INCLUDES_MIPS_CODE_UTILS_H_

#include "../intermediate/symbols.h"

/* Met la valeur d'un tableau dans une opérande */
void put_reg_var_tab(Operand * op, char * reg);

/* Met une opérande dans un registre */
void put_op_reg(Operand * op, char * reg);

/* Met un entier dans un registre  */
void put_int_reg(int value, char *reg);

/* Met une variable de la pile dans un registre */
void put_var_reg(int pos, char *reg);

/* Met l'adresse d'une constante dans un registre */
void put_const_reg(char *name, char *reg);

/* Met un registre dans la pile */
void put_reg_var(char *reg, int pos);

/* Met un octet dans un registre */
void put_oct_reg(char *dst, char *src);

/* Fait une addition */
void add(char *res, char *reg1, char *reg2);

/* Ajoute à une registre une valeur entière */
void addi(char *res, char *reg, int value);

/* Fait une soustraction */
void sub(char *res, char *reg1, char *reg2);

/* Fait une multiplication */
void mul(char *res, char *reg1, char *reg2);

/* Fait une division */
void divi(char *res, char *reg1, char *reg2);

/* Fait un modulo */
void mod(char *res, char *reg1, char *reg2);

/* Jump vers un quad */
void jump(int quad);

/* Jump si égal */
void jeq(int quad, char *reg1, char *reg2);

/* Jump si égal, str */
void cmp_str(int quad_true, int quad_false, char * reg1, char *reg2);

/* Jump si vide, str */
void cmp_empty_str(int dest, char * reg1);

/* Jump si non vide, str */
void cmp_not_empty_str(int dest, char * reg1);

/* Jump si différent */
void jne(int quad, char *reg1, char *reg2);

/* Jump si supérieur */
void jgt(int quad, char *reg1, char *reg2);

/* Jump si supérieur ou égal */
void jge(int quad, char *reg1, char *reg2);

/* Jump si inférieur */
void jlt(int quad, char *reg1, char *reg2);

/* Jump si inférieur ou égal */
void jle(int quad, char *reg1, char *reg2);

/* Appelle la primitive EXIT 
* La valeur de sortie doit être définie dans $a0
*/
void syscall_exit();

/* Appelle la primitive ECHO
* La valeur à afficher doit être définie dans $a0
*/
void syscall_echo(Operand *op);


#endif  // SRC_INCLUDES_MIPS_CODE_UTILS_H_
