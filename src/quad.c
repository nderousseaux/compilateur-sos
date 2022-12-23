// Module contenant tout le code relatif aux quadruplets
// On y retrouve aussi une liste et ses fonctions associées
// pour stocker les quadruplets

#include "includes/quad.h"

Quad_list *quad_list;

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
    Quad quad = {op, operand1, operand2, result};
    if (quad_list->size == quad_list->capacity)
    {
        quad_list->capacity *= 2;
        CHECK(
            quad_list->data = realloc(
                quad_list->data, quad_list->capacity * sizeof(Quad)));
    }
    quad_list->data[quad_list->size++] = quad;
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
    printf("| OPERATEUR\t");
    switch (quad.op)
    {
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
