/* Module contenant les fonctions relative à la table des quadruplets
*/

#ifndef SRC_INCLUDES_INTERMEDIATE_QUADS_H_
#define SRC_INCLUDES_INTERMEDIATE_QUADS_H_

#define INIT_QUAD_LIST_CAPACITY 100

#define create_list init_quad_list  // Renommage pour correspondre au cours
#define gencode init_quad           // Renommage pour correspondre au cours

// Définit un quad
typedef struct Quad {
    Operator op;
    Operand operand1;
    Operand operand2;
    Operand result;
    int idx;  // Index du quad
} Quad;

// Liste de quads
typedef struct Ql {
    Quad *data;
    int size;
    int capacity;
} Ql;

Ql* quad_list;


/* Crée une quad_list
* Alias : create_list
*/
Ql *init_quad_list();

/* Crée un quad et l'ajoute dans la liste principale
* Alias : gencode
*/
Quad *init_quad(
    Operator op, Operand operand1, Operand operand2, Operand result);

/* Ajout un quad dans une liste */
void add_quad(Ql *quad_list, Quad *quad);

/* On affiche la table des quad */
void print_quad_list(Ql *quad_list);

/* Affiche un quad */
void print_quad(Quad quad);

/* Renvoie l'index de prochain quad */
int nextquad();

/* Supprime la quad_list */
void destroy_quad_list(Ql *quad_list);


#endif  // SRC_INCLUDES_INTERMEDIATE_QUADS_H_
