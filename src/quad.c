// Module contenant tout le code relatif aux quadruplets
// On y retrouve aussi une liste et ses fonctions associées
// pour stocker les quadruplets

#include "includes/quad.h"

Quad_list* quad_list;

/* Crée une quad_list de taille définie */
Quad_list* init_quad_list() {
    Quad_list* quad_list;
    CHECK(quad_list = malloc(sizeof(Quad_list)));
    quad_list->size = 0;
    quad_list->capacity = QUAD_LIST_CAPACITY;

    CHECK(quad_list->data = calloc(quad_list->capacity, sizeof(Quad)));
    return quad_list;
}

/* Crée un quad dans la liste */
void add_quad(
    Operator op, Operand operand1, Operand operand2, Operand result) {
    Quad quad = {op, operand1, operand2, result};
    if (quad_list->size == quad_list->capacity) {
        quad_list->capacity *= 2;
        CHECK(
            quad_list->data = realloc(
                quad_list->data, quad_list->capacity * sizeof(Quad)));
    }
    quad_list->data[quad_list->size++] = quad;
}

/* Supprime la quad_list */
void destroy_quad_list(Quad_list* quad_list) {
    free(quad_list->data);
    free(quad_list);
}
