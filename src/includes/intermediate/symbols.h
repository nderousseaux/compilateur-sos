/* Module contenant tout le code relatif à la table des symboles
	On y retrouve aussi une liste et ses fonctions associées
	pour stocker les symboles

    Il y a trois table des symboles :
    - une pour les variables
    - une pour les constantes
    - une pour les temporaires
*/

#ifndef SRC_INCLUDES_INTERMEDIATE_SYMBOLS_H_
#define SRC_INCLUDES_INTERMEDIATE_SYMBOLS_H_

#define INIT_SYMBOLS_LIST_CAPACITY 100

typedef struct Symbol {
    enum {CONST, VAR, TEMP} type;  // Type du symbole
    char *name;  // Nom du symbole
    char *data;  // Donnée associée au symbole
	int position;  // Position dans la pile (-1 si constante)
} Symbol;

typedef struct St_element {
    char* key;  // Nom dans la table
    Symbol* data;  // Data du symbole
    struct St_element* next;  // Element suivant
} St_element;

typedef struct St {
    int size;  // Taille du premier niveau de la table
    int size_total;  // Taille totale de la table (nombre d'éléments)
    int last_pos;  // Dernière position utilisée dans la pile
    int last_const;  // Dernière constante ajoutée
    int last_temp;  // Dernière temporaire ajoutée
    int capacity;  // Capacité maximale de la table
    St_element** data;
} St;

St* symbols_table;

/* Crée une hashtable de taille définie */
St* init_st();

/* Ajoute une variable à la table des symboles */
void add_var_st(char * name);

/* Ajoute une constante à la table des symboles */
Symbol * add_const_st(char * data);

/* Ajoute une temporaire à la table des symboles */
void add_temp_st();

/* Ajoute une donnée dans la hashtable*/
void add_st(St* ht, char* key, Symbol* data);

/* Crée et un élément et renvoie son adresse */
St_element *add_element_st(char* key, Symbol* data);

/* Récupère une donnée dans la hashtable */
Symbol* get_st(St* ht, char* key);

/* Affiche la hashtable */
void print_st(St* ht);

/* Supprime la hashtable */
void destroy_st(St* ht);

/* Étend la table */
void extend_st(St* ht);

/* Calcule le hash d'une chaine de caractère */
int hash(char* key);

#endif  // SRC_INCLUDES_INTERMEDIATE_SYMBOLS_H_
