// Module permettant la génération du code intermédiaire
// Il contient les fonctions permettant de créer les quadruplets
// depuis la grammaire. Cela aurait pu être directement dans le yacc,
// mais cela aurait rendu le code difficilement lisible.

#include "includes/imports.h"

/* Renvoie un entier à partir d'une chaine de caractères */
int to_int(char *str)
{
    long res;
    int i = 0;
    char neg = 0;
    if (str[0] == '+')
        i = 1;
    else if (str[0] == '-')
    {
        i = 1;
        neg = 1;
    }

    // On teste les autres caractères
    for (; i < strlen(str); i++)
    {
        if (str[i] < '0' || str[i] > '9')
        {
            fprintf(stderr, "Erreur: %s n'est pas un entier\n", str);
        }
    }

    if (neg)
        res = -atoi(str + 1);
    else
        res = atoi(str);

    // On vérifie que res soit bien dans l'intervalle [-2^31, 2^31-1]
    if (res < -2147483648 || res > 2147483647)
    {
        fprintf(stderr, "Erreur: %s n'est pas un entier sur 32 bits\n", str);
    }

    return res;
}

/* Ajoute une operande à la liste chainée */
Operand_y *add_operand(Operand_y *list, Operand_y *op)
{
    if (list == NULL)
    {
        return op;
    }
    else
    {
        Operand_y *tmp = list;
        while (tmp->next != NULL)
        {
            tmp = (Operand_y *)tmp->next;
        }
        tmp->next = (struct Operand_y *)op;
        return list;
    }
}

/* Fait une opération sur toutes les opérandes de la liste */
void op_all_operand(Operand_y *list, Operator op)
{
    Operand_y *tmp = list;
    while (tmp != NULL)
    {
        switch (op)
        {
        case OP_ECHO:
            quad_echo(tmp->str, tmp->type);
            break;

        default:
            break;
        }

        tmp = (Operand_y *)tmp->next;
    }
}

/* Crée un quadruplet exit */
void quad_exit(int status)
{
    add_quad(OP_EXIT, integer(status), empty(), empty());
}

/* Crée une quadruplet echo */
void quad_echo(char *str, enum operand_type type)
{
    switch (type)
    {
    case O_VAR:
        add_quad(OP_ECHO, var(str), empty(), empty());
        break;
    case O_ID:
        add_quad(OP_ECHO, id(str), empty(), empty());
        break;
    default:
        break;
    }
}

/* Crée un quad d'assignation */
void quad_assign(char *src, char *dest, enum operand_type type)
{

    /* On crée l'identifiant dans la table des symboles */
    create_symbol(dest);

    switch (type)
    {
    case O_VAR:
        add_quad(OP_ASSIGN, var(src), empty(), id(dest));
        break;
    case O_INT:
        add_quad(OP_ASSIGN, integer(to_int(src)), empty(), id(dest));
        break;
    case O_ID:
        add_quad(OP_ASSIGN, id(src), empty(), id(dest));
        break;
    default:
        break;
    }
}

void quad_operation(Operand_y op1, Operand_y op2, enum operator_type type, Operand_y res) {

    /* On crée l'identifiant dans la table des symboles */


    switch (type)
    {
    case O_PLUS:
        add_quad(OP_PLUS, integer(to_int(op1.str)), integer(to_int(op2.str)), integer(to_int(res.str)));
        break;
    case O_MOINS:
        add_quad(OP_MOINS, integer(to_int(op1.str)), integer(to_int(op2.str)),  integer(to_int(res.str)));
        break;
    case O_FOIS:
        add_quad(OP_FOIS, integer(to_int(op1.str)), integer(to_int(op2.str)), integer(to_int(res.str)));
        break;
    case O_DIVISION:
        add_quad(OP_FOIS, integer(to_int(op1.str)), integer(to_int(op2.str)), integer(to_int(res.str)));
        break;
    case O_MODULO:
        add_quad(OP_MODULO, integer(to_int(op1.str)), integer(to_int(op2.str)), integer(to_int(res.str)));
        break;
    default:
        break;
    }
}

/* Crée une quadruplet goto */
void quad_goto(int idx)
{
    if (idx == -1)
        add_quad(OP_GOTO, empty(), empty(), empty());
    else
        add_quad(OP_GOTO, empty(), empty(), integer(idx));
}

/* création d'un quad pour l'égalité*/
void quad_equal(Operand_y op1, Operand_y op2, int go)
{
    if (op1.type == O_ID && op2.type == O_ID)
    {
        add_quad(OP_EQUAL, id(op1.str), id(op2.str), integer(go));
    }
    else if (op1.type == O_INT && op2.type == O_INT)
    {
        add_quad(OP_EQUAL, integer(to_int(op1.str)), integer(to_int(op2.str)), integer(go));
    }
    else if (op1.type == O_INT && op2.type == O_ID)
    {
        add_quad(OP_EQUAL, integer(to_int(op1.str)), id(op2.str), integer(go));
    }
    else if (op1.type == O_ID && op2.type == O_INT)
    {
        add_quad(OP_EQUAL, id(op1.str), integer(to_int(op2.str)), integer(go));
    }
    else
    {
        fprintf(stderr, "Erreur quad_equal, operand assignation\n");
    }
}

void quad_nequal(Operand_y op1, Operand_y op2, int go)
{
    if (op1.type == O_ID && op2.type == O_ID)
    {
        add_quad(OP_NEQUAL, id(op1.str), id(op2.str), integer(go));
    }
    else if (op1.type == O_INT && op2.type == O_INT)
    {
        add_quad(OP_NEQUAL, integer(to_int(op1.str)), integer(to_int(op2.str)), integer(go));
    }
    else if (op1.type == O_INT && op2.type == O_ID)
    {
        add_quad(OP_NEQUAL, integer(to_int(op1.str)), id(op2.str), integer(go));
    }
    else if (op1.type == O_ID && op2.type == O_INT)
    {
        add_quad(OP_NEQUAL, id(op1.str), integer(to_int(op2.str)), integer(go));
    }
    else
    {
        fprintf(stderr, "Erreur quad_nequal, operand assignation\n");
    }
}

void quad_stsup(Operand_y op1, Operand_y op2, int go)
{
    if (op1.type == O_ID && op2.type == O_ID)
    {
        add_quad(OP_STSUP, id(op1.str), id(op2.str), integer(go));
    }
    else if (op1.type == O_INT && op2.type == O_INT)
    {
        add_quad(OP_STSUP, integer(to_int(op1.str)), integer(to_int(op2.str)), integer(go));
    }
    else if (op1.type == O_INT && op2.type == O_ID)
    {
        add_quad(OP_STSUP, integer(to_int(op1.str)), id(op2.str), integer(go));
    }
    else if (op1.type == O_ID && op2.type == O_INT)
    {
        add_quad(OP_STSUP, id(op1.str), integer(to_int(op2.str)), integer(go));
    }
    else
    {
        fprintf(stderr, "Erreur quad_stsup, operand assignation\n");
    }
}

void quad_supeq(Operand_y op1, Operand_y op2, int go)
{
    if (op1.type == O_ID && op2.type == O_ID)
    {
        add_quad(OP_SUPEQ, id(op1.str), id(op2.str), integer(go));
    }
    else if (op1.type == O_INT && op2.type == O_INT)
    {
        add_quad(OP_SUPEQ, integer(to_int(op1.str)), integer(to_int(op2.str)), integer(go));
    }
    else if (op1.type == O_INT && op2.type == O_ID)
    {
        add_quad(OP_SUPEQ, integer(to_int(op1.str)), id(op2.str), integer(go));
    }
    else if (op1.type == O_ID && op2.type == O_INT)
    {
        add_quad(OP_SUPEQ, id(op1.str), integer(to_int(op2.str)), integer(go));
    }
    else
    {
        fprintf(stderr, "Erreur quad_supeq, operand assignation\n");
    }
}

void quad_stinf(Operand_y op1, Operand_y op2, int go)
{
    if (op1.type == O_ID && op2.type == O_ID)
    {
        add_quad(OP_STINF, id(op1.str), id(op2.str), integer(go));
    }
    else if (op1.type == O_INT && op2.type == O_INT)
    {
        add_quad(OP_STINF, integer(to_int(op1.str)), integer(to_int(op2.str)), integer(go));
    }
    else if (op1.type == O_INT && op2.type == O_ID)
    {
        add_quad(OP_STINF, integer(to_int(op1.str)), id(op2.str), integer(go));
    }
    else if (op1.type == O_ID && op2.type == O_INT)
    {
        add_quad(OP_STINF, id(op1.str), integer(to_int(op2.str)), integer(go));
    }
    else
    {
        fprintf(stderr, "Erreur quad_stinf, operand assignation\n");
    }
}

void quad_infeq(Operand_y op1, Operand_y op2, int go)
{
    if (op1.type == O_ID && op2.type == O_ID)
    {
        add_quad(OP_INFEQ, id(op1.str), id(op2.str), integer(go));
    }
    else if (op1.type == O_INT && op2.type == O_INT)
    {
        add_quad(OP_INFEQ, integer(to_int(op1.str)), integer(to_int(op2.str)), integer(go));
    }
    else if (op1.type == O_INT && op2.type == O_ID)
    {
        add_quad(OP_INFEQ, integer(to_int(op1.str)), id(op2.str), integer(go));
    }
    else if (op1.type == O_ID && op2.type == O_INT)
    {
        add_quad(OP_INFEQ, id(op1.str), integer(to_int(op2.str)), integer(go));
    }
    else
    {
        fprintf(stderr, "Erreur quad_infeq, operand assignation\n");
    }
}

/* Crée un quadruplet if */
void quad_if(Operand_y op1, Operand_y op2, int go)
{
    if (op1.type == O_ID && op2.type == O_ID)
    {
        add_quad(OP_IF, id(op1.str), id(op2.str), integer(go));
    }
    else if (op1.type == O_INT && op2.type == O_INT)
    {
        add_quad(OP_IF, integer(to_int(op1.str)), integer(to_int(op2.str)), integer(go));
    }
    else if (op1.type == O_INT && op2.type == O_ID)
    {
        add_quad(OP_IF, integer(to_int(op1.str)), id(op2.str), integer(go));
    }
    else if (op1.type == O_ID && op2.type == O_INT)
    {
        add_quad(OP_IF, id(op1.str), integer(to_int(op2.str)), integer(go));
    }
    else
    {
        fprintf(stderr, "Erreur quad_nequal, operand assignation\n");
    }
}

/* Crée une quadruplet goto */
void quad_goto(int idx)
{
    if (idx == -1)
        add_quad(OP_GOTO, empty(), empty(), empty());
    else
        add_quad(OP_GOTO, empty(), empty(), integer(idx));
}

/* création d'un quad pour l'égalité*/
void quad_equal(Operand_y op1, Operand_y op2, int go)
{
    if (op1.type == O_ID && op2.type == O_ID)
    {
        add_quad(OP_EQUAL, id(op1.str), id(op2.str), integer(go));
    }
    else if (op1.type == O_INT && op2.type == O_INT)
    {
        add_quad(OP_EQUAL, integer(to_int(op1.str)), integer(to_int(op2.str)), integer(go));
    }
    else if (op1.type == O_INT && op2.type == O_ID)
    {
        add_quad(OP_EQUAL, integer(to_int(op1.str)), id(op2.str), integer(go));
    }
    else if (op1.type == O_ID && op2.type == O_INT)
    {
        add_quad(OP_EQUAL, id(op1.str), integer(to_int(op2.str)), integer(go));
    }
    else
    {
        fprintf(stderr, "Erreur quad_equal, operand assignation\n");
    }
}

void quad_nequal(Operand_y op1, Operand_y op2, int go)
{
    if (op1.type == O_ID && op2.type == O_ID)
    {
        add_quad(OP_NEQUAL, id(op1.str), id(op2.str), integer(go));
    }
    else if (op1.type == O_INT && op2.type == O_INT)
    {
        add_quad(OP_NEQUAL, integer(to_int(op1.str)), integer(to_int(op2.str)), integer(go));
    }
    else if (op1.type == O_INT && op2.type == O_ID)
    {
        add_quad(OP_NEQUAL, integer(to_int(op1.str)), id(op2.str), integer(go));
    }
    else if (op1.type == O_ID && op2.type == O_INT)
    {
        add_quad(OP_NEQUAL, id(op1.str), integer(to_int(op2.str)), integer(go));
    }
    else
    {
        fprintf(stderr, "Erreur quad_nequal, operand assignation\n");
    }
}

void quad_stsup(Operand_y op1, Operand_y op2, int go)
{
    if (op1.type == O_ID && op2.type == O_ID)
    {
        add_quad(OP_STSUP, id(op1.str), id(op2.str), integer(go));
    }
    else if (op1.type == O_INT && op2.type == O_INT)
    {
        add_quad(OP_STSUP, integer(to_int(op1.str)), integer(to_int(op2.str)), integer(go));
    }
    else if (op1.type == O_INT && op2.type == O_ID)
    {
        add_quad(OP_STSUP, integer(to_int(op1.str)), id(op2.str), integer(go));
    }
    else if (op1.type == O_ID && op2.type == O_INT)
    {
        add_quad(OP_STSUP, id(op1.str), integer(to_int(op2.str)), integer(go));
    }
    else
    {
        fprintf(stderr, "Erreur quad_stsup, operand assignation\n");
    }
}

void quad_supeq(Operand_y op1, Operand_y op2, int go)
{
    if (op1.type == O_ID && op2.type == O_ID)
    {
        add_quad(OP_SUPEQ, id(op1.str), id(op2.str), integer(go));
    }
    else if (op1.type == O_INT && op2.type == O_INT)
    {
        add_quad(OP_SUPEQ, integer(to_int(op1.str)), integer(to_int(op2.str)), integer(go));
    }
    else if (op1.type == O_INT && op2.type == O_ID)
    {
        add_quad(OP_SUPEQ, integer(to_int(op1.str)), id(op2.str), integer(go));
    }
    else if (op1.type == O_ID && op2.type == O_INT)
    {
        add_quad(OP_SUPEQ, id(op1.str), integer(to_int(op2.str)), integer(go));
    }
    else
    {
        fprintf(stderr, "Erreur quad_supeq, operand assignation\n");
    }
}

void quad_stinf(Operand_y op1, Operand_y op2, int go)
{
    if (op1.type == O_ID && op2.type == O_ID)
    {
        add_quad(OP_STINF, id(op1.str), id(op2.str), integer(go));
    }
    else if (op1.type == O_INT && op2.type == O_INT)
    {
        add_quad(OP_STINF, integer(to_int(op1.str)), integer(to_int(op2.str)), integer(go));
    }
    else if (op1.type == O_INT && op2.type == O_ID)
    {
        add_quad(OP_STINF, integer(to_int(op1.str)), id(op2.str), integer(go));
    }
    else if (op1.type == O_ID && op2.type == O_INT)
    {
        add_quad(OP_STINF, id(op1.str), integer(to_int(op2.str)), integer(go));
    }
    else
    {
        fprintf(stderr, "Erreur quad_stinf, operand assignation\n");
    }
}

void quad_infeq(Operand_y op1, Operand_y op2, int go)
{
    if (op1.type == O_ID && op2.type == O_ID)
    {
        add_quad(OP_INFEQ, id(op1.str), id(op2.str), integer(go));
    }
    else if (op1.type == O_INT && op2.type == O_INT)
    {
        add_quad(OP_INFEQ, integer(to_int(op1.str)), integer(to_int(op2.str)), integer(go));
    }
    else if (op1.type == O_INT && op2.type == O_ID)
    {
        add_quad(OP_INFEQ, integer(to_int(op1.str)), id(op2.str), integer(go));
    }
    else if (op1.type == O_ID && op2.type == O_INT)
    {
        add_quad(OP_INFEQ, id(op1.str), integer(to_int(op2.str)), integer(go));
    }
    else
    {
        fprintf(stderr, "Erreur quad_infeq, operand assignation\n");
    }
}

/* Crée un quadruplet if */
void quad_if(Operand_y op1, Operand_y op2, int go)
{
    if (op1.type == O_ID && op2.type == O_ID)
    {
        add_quad(OP_IF, id(op1.str), id(op2.str), integer(go));
    }
    else if (op1.type == O_INT && op2.type == O_INT)
    {
        add_quad(OP_IF, integer(to_int(op1.str)), integer(to_int(op2.str)), integer(go));
    }
    else if (op1.type == O_INT && op2.type == O_ID)
    {
        add_quad(OP_IF, integer(to_int(op1.str)), id(op2.str), integer(go));
    }
    else if (op1.type == O_ID && op2.type == O_INT)
    {
        add_quad(OP_IF, id(op1.str), integer(to_int(op2.str)), integer(go));
    }
    else
    {
        fprintf(stderr, "Erreur quad_nequal, operand assignation\n");
    }
}