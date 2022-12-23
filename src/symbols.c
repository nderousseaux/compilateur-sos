// Module contenant tout le code relatif à la table des symboles
// On y retrouve aussi une liste et ses fonctions associées
// pour stocker les symboles

#include "includes/symbols.h"

St* symbols_table; 

/* Crée une hashtable de taille définie */
St* init_st(){
    St* ht;
    CHECK(ht = malloc(sizeof(St)));
    ht->size = 0;
    ht->size_total = 0;
    ht->last_pos = 0;
    ht->last_const = 0;
    ht->capacity = SYMBOLS_LIST_CAPACITY;

    CHECK(ht->data = calloc(SYMBOLS_LIST_CAPACITY, sizeof(St_element*)));
    return ht;
}

/* Crée un symbole (vide) */
void create_symbol(char* id)
{
    //Si il existe déjà
    if(get_st(symbols_table, id) != NULL)
        return;

    Symbol* s;
    CHECK(s = malloc(sizeof(Symbol)));
    s->data = NULL;
    s->position = symbols_table->last_pos;
    symbols_table->last_pos++;
    add_st(symbols_table, id, s);
}

/* Crée un symbole (constante) */
char * create_const(char * data)
{
    Symbol* s;
    CHECK(s = malloc(sizeof(Symbol)));
    char * key = next_const();    
    s->data = data;
    s->position = -1;
    add_st(symbols_table, key, s);

    return key;
}

/* Ajoute une donnée dans la hashtable*/
void add_st(St* ht, char* key, Symbol* data){
    //On regarde si on ne devrait pas agrandir la table
     if (ht->size >= ht->capacity / 2) {
        extend_st(ht);
    }

    int index = hash(key) % ht->capacity; //On calcue l'index

    St_element* element = ht->data[index]; //On récupère l'élément à cet index

    //Si l'élément est vide, on l'ajoute
    if(element == NULL){   
        ht->data[index] = add_element_st(key, data);
        ht->size++;
        ht->size_total++;
    }
    //Sinon, on parcourt la liste jusqu'à trouver un élément vide
    else{
        while(element != NULL){
            //Si l'élément est connu on le remplace
            if(strcmp(element->key, key) == 0){
                element->data = data;
                return;
            }
            if(element->next == NULL){
                element->next = add_element_st(key, data);
                break;
            }
            element = element->next;
        }
        
        ht->size++;
    }
}

/* Crée et un élément et renvoie son adresse */
St_element *add_element_st(char* key, Symbol* data){
    St_element* element;
    CHECK(element = malloc(sizeof(St_element)));
    element->key = key;
    element->data = data;
    element->next = NULL;
    return element;
}

/* Récupère une donnée dans la hashtable */
Symbol* get_st(St* ht, char* key){
    int index = hash(key) % ht->capacity; //On calcue l'index

    St_element* element = ht->data[index]; //On récupère l'élément à cet index

    while (element != NULL){
        if(strcmp(element->key, key) == 0){
            return element->data;
        }
        element = element->next;
    }
    return NULL;
}

/* Affiche la hashtable */
void print_st(St* ht){
    printf("\n──────────────── Table des symboles ────────────────\n");
    printf("Nombre d'éléments : %d\n", ht->size);
    printf("Taille de la première couche de la table : %d\n\n", ht->capacity);

    int nb_occupe = 0;

    for(int i=0; i<ht->capacity; i++){
        St_element* element = ht->data[i];

        if(element != NULL){
            printf("Case n°%d/%d\n", i, ht->capacity);
            do{
                printf("| '%s' : %s", element->key, element->data->data);
                if(element->data->position == -1)
                    printf(" (constante)\n");
                else
                    printf(" (pos:%d)\n", element->data->position);
            }
            while((element = element->next) != NULL);
            nb_occupe++;
            printf("\n");
        }
    }
    printf("Nombre de cases occupées : %d/%d\n", nb_occupe, ht->capacity);
}

/* Supprime la hashtable */
void destroy_st(St* ht){
    for(int i = 0; i < ht->capacity; i++){
        St_element* element = ht->data[i];
        while(element != NULL){
            St_element* next = element->next;
            free(element);
            element = next;
        }
    }
    free(ht->data);
    free(ht);
}

/* Étend la table */
void extend_st(St* ht) {
    int new_capacity = ht->capacity * 2; //On double la taille de la table

    //On crée une nouvelle table
    St_element** new_data;
    CHECK(new_data = calloc(new_capacity, sizeof(St_element*)));

    //On parcourt l'ancienne table
    for(int i = 0; i < ht->capacity; i++){
        St_element* element = ht->data[i];
        while(element != NULL){
            //On recalcule l'index
            int index = hash(element->key) % new_capacity;

            //On ajoute l'élément à la nouvelle table
            St_element* next = element->next; //On sauvegarde le prochain élément
            element->next = new_data[index];
            new_data[index] = element;
            element = next;
        }
    }

    //On libère l'ancienne table
    free(ht->data);
    ht->data = new_data;
    ht->capacity = new_capacity;
}

/* Calcule le hash d'une chaine de caractère */
int hash(char* key){
    int hash = 0;
    int i = 0;
    char c = key[0];
    while (c != '\0'){
        i++;

        hash += hash + (i * i) + (c * c); //Des carrés pour augmenter la dispersion en cas de chaines similaire (key1 et key2 par exemple)
        c = key[i];
    }
    return hash;
}

/* Renvoie le nom de la prochaine constante (et l'incrémente) */
char * next_const(){
    char * key;
    CHECK(key = calloc(10, sizeof(char)));

    sprintf(key, "string_%d", symbols_table->last_const);

    symbols_table->last_const++;
    return key;
}