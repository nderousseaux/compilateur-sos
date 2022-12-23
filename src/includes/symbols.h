// Module contenant tout le code relatif à la table des symboles
// On y retrouve aussi une liste et ses fonctions associées
// pour stocker les symboles

#ifndef SRC_INCLUDES_SYMBOLS_H_
#define SRC_INCLUDES_SYMBOLS_H_

#include "./operand.h"
#include "./utils.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SYMBOLS_LIST_CAPACITY 100

typedef struct Symbol{
    char* data;
    int position; // Position dans la pile, -1 si une constante
} Symbol;

typedef struct St_element{
    char* key;
    Symbol* data;
    struct St_element* next;
} St_element;

typedef struct St{
    int size;   //Taille du premier niveau de la table
    int size_total; // Taille totale de la table
    int last_pos; // Dernière position utilisée dans la pile
    int last_const; // Dernière constante utilisée
    int capacity;
    St_element** data;
} St;

extern St* symbols_table;

/* Crée une hashtable de taille définie */
St* init_st();

/* Crée un symbole (vide) */
void create_symbol(char* id);

/* Crée un symbole (constante) */
char * create_const(char * data);

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

/* Renvoie le nom de la prochaine constante (et l'incrémente) */
char * next_const();

#endif  // SRC_INCLUDES_SYMBOLS_H_
