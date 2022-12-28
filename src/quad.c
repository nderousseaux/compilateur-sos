// Module contenant tout le code relatif aux quadruplets
// On y retrouve aussi une liste et ses fonctions associées
// pour stocker les quadruplets

#include "includes/quad.h"

Quad_list *quad_list;

int nextquad = 0;

/* Crée une quad_list de taille définie */
Quad_list *init_quad_list()
{
    Quad_list *quad_list;
    CHECK(quad_list = malloc(sizeof(Quad_list)));
    quad_list->size = 0;
    quad_list->capacity = QUAD_LIST_CAPACITY;

    CHECK(quad_list->data = calloc(quad_list->capacity, sizeof(Quad)));
    return quad_list;
}

/* Crée un quad dans la liste */
void add_quad(
    Operator op, Operand operand1, Operand operand2, Operand result)
{
    Quad quad = {op, operand1, operand2, result, nextquad};
    if (quad_list->size == quad_list->capacity)
    {
        quad_list->capacity *= 2;
        CHECK(
            quad_list->data = realloc(
                quad_list->data, quad_list->capacity * sizeof(Quad)));
    }
    quad_list->data[quad_list->size++] = quad;
    nextquad++;
}

/* On affiche la table des quad */
void print_quad_list(Quad_list *quad_list)
{
    printf("\n──────────────── Liste des quadruplets ────────────────\n");
    printf("Nombre d'éléments : %d\n", quad_list->size);
    printf("Taille de la table : %d\n\n", quad_list->capacity);

    for (int i = 0; i < quad_list->size; i++)
    {
        printf("Case n°%d:\n", i);
        print_quad(quad_list->data[i]);
    }
}

/* Affiche un quad */
void print_quad(Quad quad)
{
    printf("| idx: %d\n", quad.idx);
    printf("| OPERATEUR\t");
    switch (quad.op)
    {
    case OP_GOTO:
        printf("GOTO ");
        break;
    case OP_EXIT:
        printf("EXIT ");
        break;
    case OP_ASSIGN:
        printf("ASSIGN ");
        break;
    case OP_ECHO:
        printf("ECHO ");
        break;
    case OP_EQUAL:
        printf("EQUAL ");
        break;
    case OP_NEQUAL:
        printf("NEQUAL ");
        break;
    case OP_STSUP:
        printf("STSUP ");
        break;
    case OP_SUPEQ:
        printf("SUPEQ ");
        break;
    case OP_STINF:
        printf("STINF ");
        break;
    case OP_INFEQ:
        printf("INFEQ ");
        break;
    case OP_IF:
        printf("IF ");
        break;
    default:
        printf("\nOpérateur inconnu (pensez à l'ajouter dans print_quad...)\n");
        break;
    }
    printf("\n| OPERANDE 1\t");
    print_operand(quad.operand1);
    printf("\n");
    printf("| OPERANDE 2\t");
    print_operand(quad.operand2);
    printf("\n");
    printf("| RESULTAT\t");
    print_operand(quad.result);
    printf("\n");
}

/* Affiche un operand */
void print_operand(Operand operand)
{
    switch (operand.type)
    {
    case ID:
        printf("ID\t%s", operand.value);
        break;
    case VAR:
        printf("VAR\t%s", operand.value);
        break;
    case INTEGER:
        printf("INTEGER\t%d", operand.integer_value);
        break;
    case EMPTY:
        printf("EMPTY");
        break;
    default:
        printf("\nType de l'opérande inconnu (pensez à l'ajouter dans print_operand...)\n");
        break;
    }
}
/* Supprime la quad_list */
void destroy_quad_list(Quad_list *quad_list)
{
    free(quad_list->data);
    free(quad_list);
}

/* Ajout personnel à discuter avec le grand Natha*/

void add_idx_quad(idx_quad *dest, int idx)
{
    idx_quad *next = dest->next_idx;

    while (next != NULL)
    {
        next = dest->next_idx;
    }

    CHECK(next = malloc(sizeof(idx_quad)));
    next->idx = idx;
    next->next_idx = NULL;
}

void destroy_idx_quad(idx_quad *dest)
{
    idx_quad *next = dest->next_idx;

    if (next != NULL)
    {
        destroy_idx_quad(next);
    }

    free(dest);
}

idx_quad *creelist(int value)
{

    idx_quad *list;
    CHECK(list = malloc(sizeof(idx_quad)));

    list->idx = value;
    return list;
}

idx_quad *concat(idx_quad *Q1, idx_quad *Q2)
{

    idx_quad *Q3 = creelist(-1);

    idx_quad *next = Q1;
    while (next != NULL)
    {
        add_idx_quad(Q3, next->idx);
        next = next->next_idx;
    }
    next = Q2;
    while (next != NULL)
    {
        add_idx_quad(Q3, next->idx);
        next = next->next_idx;
    }

    destroy_idx_quad(Q1);
    destroy_idx_quad(Q2);

    return Q3;
}

void complete(idx_quad *list, int address)
{
    int indice = list->idx;
    quad_list->data[indice].result.integer_value = address;

    idx_quad *next = list->next_idx;
    while (next != NULL)
    {
        int indice = list->idx;
        quad_list->data[indice].result.integer_value = address;
    }
}