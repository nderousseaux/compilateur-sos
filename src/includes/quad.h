// Module contenant tout le code relatif aux quadruplets
// On y retrouve aussi une liste et ses fonctions associées
// pour stocker les quadruplets

#ifndef SRC_INCLUDES_QUAD_H_
#define SRC_INCLUDES_QUAD_H_

#define QUAD_LIST_CAPACITY 100

#include "operand.h"

// Définit un quad
typedef struct Quad
{
    Operator op;
    Operand operand1;
    Operand operand2;
    Operand result;
    int idx; // Index du quad
} Quad;

// Liste de quads
typedef struct Quad_list
{
    Quad *data;
    int size;
    int capacity;
} Quad_list;

// Liste de positions de quads
typedef struct idx_quad
{
    int idx;
    struct idx_quad *next_idx;
} idx_quad;

typedef struct Bool
{
    idx_quad *tru;  // Liste des quads incomplets à exécuter si Vrai
    idx_quad *fals; // Liste des quads incomplets à exécuter si faux
    idx_quad *next; // Liste des positions de tous les quads incomplets pour la suite du code
    int idx;        // Peut indiquer un quad transporté
} Bool;

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

void add_idx_quad(idx_quad *dest, int idx);

void destroy_idx_quad(idx_quad *dest);

idx_quad *creelist(int next);

idx_quad *concat(idx_quad *Q1, idx_quad *Q2);

void complete(idx_quad *list, int address);

/* Affiche une liste quad*/
void print_idx_quad(idx_quad *list);

#endif // SRC_INCLUDES_QUAD_H_
