// Module contenant tout le code relatif aux quadruplets
// On y retrouve aussi une liste et ses fonctions associées
// pour stocker les quadruplets

#ifndef SRC_INCLUDES_QUAD_H_
#define SRC_INCLUDES_QUAD_H_

#include <stdlib.h>

#include "./operand.h"
#include "./utils.h"

#define QUAD_LIST_CAPACITY 100

// Définit un quad
typedef struct Quad
{
    Operator op;
    Operand operand1;
    Operand operand2;
    Operand result;
} Quad;

// Liste de quads
typedef struct Quad_list
{
    Quad *data;
    int size;
    int capacity;
} Quad_list;

extern Quad_list *quad_list; // Liste des quadruplets

/* Crée une quad_list */
struct Quad_list *init_quad_list();

/* Crée un quad dans la liste */
void add_quad(
    Operator op, Operand operand1, Operand operand2, Operand result);

/* On affiche la table des quad */
void print_quad_list(Quad_list *quad_list);

/* Affiche un quad */
void print_quad(Quad quad);

/* Affiche un operand */
void print_operand(Operand operand);

/* Supprime la quad_list */
void destroy_quad_list(Quad_list *quad_list);

// Ajout personnel, à discuter avec Natha

Quad_list *creelist(Quad Quad);

Quad_list *concat(Quad_list *Q1, Quad_list *Q2);

void complete(Quad_list list, int address);

#endif // SRC_INCLUDES_QUAD_H_
