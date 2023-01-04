/* Module contenant les fonctions relative à la table des quadruplets
*/

#include "../includes/imports.h"

Ql* quad_list;

/* Crée une quad_list */
Ql *init_quad_list() {
    Ql *ql;
    CHECK(ql = malloc(sizeof(Ql)));
    ql->size = 0;
    ql->capacity = INIT_QUAD_LIST_CAPACITY;

    CHECK(ql->data = calloc(ql->capacity, sizeof(Quad)));
    return ql;
}

/* Crée une quad list et l'initialise avec un premier quad */
Ql *create_list(Quad *quad) {
    Ql *ql = init_quad_list();
    add_quad(ql, quad);
    return ql;
}

/* Crée un quad et l'ajoute dans la liste principale
* Alias : gencode
*/
Quad *init_quad(
    Operator op, Operand operand1, Operand operand2, Operand result) {
    Quad quad = {op, operand1, operand2, result, nextquad(quad_list)};
    add_quad(quad_list, &quad);
    return &quad_list->data[quad_list->size - 1];
}

/* Ajout un quad dans une liste */
void add_quad(Ql *ql, Quad *quad) {
    if (ql->size == ql->capacity) {
        ql->capacity *= 2;
        CHECK(
            ql->data = realloc(
                ql->data, ql->capacity * sizeof(Quad)));
    }
    ql->data[ql->size++] = *quad;
}

/* On affiche la table des quad */
void print_quad_list(Ql *ql) {
    printf("\n──────────────── Liste des quadruplets ────────────────\n");
    printf("Nombre d'éléments : %d\n", ql->size);
    printf("Taille de la table : %d\n\n", ql->capacity);

    for (int i = 0; i < ql->size; i++) {
        printf("Case n°%d:\n", i);
        print_quad(ql->data[i]);
    }
}

/* Affiche un quad */
void print_quad(Quad quad) {
    printf("| idx: %d\n", quad.idx);
    printf("| OPERATEUR\t");
    print_operator(quad.op);
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

/* Renvoie l'index du prochain quad */
int nextquad() {
    return quad_list->size;
}

/* Concatène deux listes de quads */
Ql * concat(Ql *ql1, Ql *ql2) {
    Ql * ql = ql1;
    for (int i = 0; i < ql2->size; i++) {
        add_quad(ql, &ql2->data[i]);
    }
    destroy_quad_list(ql2);

    return ql;
}

/* Supprime la quad_list */
void destroy_quad_list(Ql *ql) {
    free(ql->data);
    free(ql);
}
