/* Module contenant les fonctions pour simplifier 
* l'écriture du code assembleur
*/

#ifndef SRC_INCLUDES_MIPS_CODE_UTILS_H_
#define SRC_INCLUDES_MIPS_CODE_UTILS_H_

#include "../intermediate/symbols.h"

FILE *f;  // Fichier de sortie

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

/* Fait une addition */
void add(char *res, char *reg1, char *reg2);

/* Fait une soustraction */
void sub(char *res, char *reg1, char *reg2);

/* Fait une multiplication */
void mul(char *res, char *reg1, char *reg2);

/* Fait une division */
void divi(char *res, char *reg1, char *reg2);

/* Fait un modulo */
void mod(char *res, char *reg1, char *reg2);

/* Appelle la primitive EXIT 
* La valeur de sortie doit être définie dans $a0
*/
void syscall_exit();

/* Appelle la primitive ECHO
* La valeur à afficher doit être définie dans $a0
*/
void syscall_echo(Operand *op);


#endif  // SRC_INCLUDES_MIPS_CODE_UTILS_H_
