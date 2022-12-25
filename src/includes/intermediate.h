// Module permettant la génération du code intermédiaire
// Il contient les fonctions permettant de créer les quadruplets
// depuis la grammaire. Cela aurait pu être directement dans le yacc,
// mais cela aurait rendu le code difficilement lisible.

#ifndef SRC_INCLUDES_INTERMEDIATE_H_
#define SRC_INCLUDES_INTERMEDIATE_H_

#include "quad.h"

enum operateur_type
{
    O_EQUAL,  //==
    O_NEQUAL, //!=
    O_STSUP,  //>
    O_SUPEQ,  //>=
    O_STINF,  //<
    O_INFEQ,  // <=
    O_NOEMPTY,
    O_EMPTY
};

enum operator_type{
    O_PLUS,
    O_MOINS, 
    O_FOIS,
    O_DIVISION,
    O_MODULO,
};

enum operand_type
{
    O_VAR,
    O_INT,
    O_ID
};

typedef struct
{ // Structure opérande
    enum operand_type type;
    char *str;
    int val;
    struct Operand_y *next;
} Operand_y;

typedef struct
{
    enum operator_type type;
} Operator_y;

typedef struct
{
    enum operator_type type;
} Operator_y;

typedef struct
{
    enum operateur_type type;
} Operateur_y;

/* Crée un quadruplet exit */
void quad_exit(int status);

/* Crée un quad d'assignation */
void quad_assign(char *src, char *dest, enum operand_type type);

/* Crée une quadruplet echo */
void quad_echo(char *str, enum operand_type type);

void quad_operation(Operand_y op1, Operand_y op2, enum operator_type type, Operand_y res);



/* Crée une quadruplet goto */
void quad_goto(int idx);

/* Crée un quadruplet equal */
void quad_equal(Operand_y op1, Operand_y op2, int go);

void quad_nequal(Operand_y op1, Operand_y op2, int go);

void quad_stsup(Operand_y op1, Operand_y op2, int go);

void quad_supeq(Operand_y op1, Operand_y op2, int go);

void quad_stinf(Operand_y op1, Operand_y op2, int go);

void quad_infeq(Operand_y op1, Operand_y op2, int go);

/* Ajoute une operande à la liste chainée */
Operand_y *add_operand(Operand_y *list, Operand_y *op);

/* Fait une opération sur toutes les opérandes de la liste */
void op_all_operand(Operand_y *list, Operator op);

/* Renvoie un entier à partir d'une chaine de caractères */
int to_int(char *str);

void quad_equal(Operand_y op1, Operand_y op2, int go);
#endif // SRC_INCLUDES_INTERMEDIATE_H_
