/* Module contenant tout le code relatif à la table des symboles
    On y retrouve aussi une liste et ses fonctions associées
    pour stocker les symboles

    Il y a trois table des symboles :
    - une pour les variables
    - une pour les constantes
    - une pour les temporaires
*/

#include "../includes/imports.h"

extern St *symbols_table;

/* Crée une hashtable de taille définie */
St *init_st()
{
    St *ht;
    CHECK(ht = malloc(sizeof(St)));
    ht->size = 0;
    ht->size_total = 0;
    ht->last_pos = 0;
    ht->last_const = 0;
    ht->last_temp = 0;
    ht->capacity = INIT_SYMBOLS_LIST_CAPACITY;
    CHECK(ht->data = calloc(INIT_SYMBOLS_LIST_CAPACITY, sizeof(St_element *)));
    return ht;
}

/* Ajoute une variable à la table des symboles */
void add_var_st(char *name, Type_operand type)
{ // FIXME modifier cette fonction pour fixer une size de 4 pour arnaud
    Symbol *s;
    CHECK(s = malloc(sizeof(Symbol)));
    s->type = VAR;
    s->type_data = type;
    s->name = name;
    s->constant = NULL;
    s->data = NULL;
    s->position = symbols_table->last_pos;
    symbols_table->last_pos += 4; // Chaque variable prend 4 octets
    add_st(symbols_table, name, s);
}

/* Ajoute une variable tableau à la table des symboles */
void add_tableau_st(char *name, int size)
{ // TODO REDONDANCE
    Symbol *s;
    CHECK(s = malloc(sizeof(Symbol)));
    s->type = TAB;
    s->size = size;
    s->type_data = TAB_T;
    s->name = name;
    s->data = NULL;
    s->position = symbols_table->last_pos;
    symbols_table->last_pos += (4 * size); // Le tableau est fixe, sa taille est définie par size
    add_st(symbols_table, name, s);
}

/* Ajoute une constante à la table des symboles */
Symbol *add_const_st(char *data)
{
    Symbol *s;
    CHECK(s = malloc(sizeof(Symbol)));
    s->type = CONST;
    s->position = -1;
    CHECK(s->data = calloc(strlen(data) + 1, sizeof(char)));
    sprintf(s->data, "%s", data);

    char *name;
    CHECK(name = calloc(10, sizeof(char)));

    snprintf(name, 10, "const_%d", symbols_table->last_const);
    symbols_table->last_const++;
    s->name = name;
    add_st(symbols_table, name, s);
    return s;
}

/* Ajoute une temporaire à la table des symboles */
Symbol *add_temp_st()
{
    Symbol *s;
    CHECK(s = malloc(sizeof(Symbol)));
    s->type = TEMP;
    s->type_data = INTEGER_T;
    s->position = symbols_table->last_pos;
    symbols_table->last_pos += 4; // Chaque variable prend 4 octets

    char *name;
    CHECK(name = calloc(10, sizeof(char)));
    snprintf(name, 10, "temp_%d", symbols_table->last_temp);
    symbols_table->last_temp++;
    s->name = name;
    add_st(symbols_table, name, s);

    return s;
}

/* Ajoute une donnée dans la hashtable*/
void add_st(St *ht, char *key, Symbol *data)
{
    // On regarde si on ne devrait pas agrandir la table
    if (ht->size >= ht->capacity / 2)
    {
        extend_st(ht);
    }

    int index = hash(key) % ht->capacity; // On calcue l'index

    St_element *element = ht->data[index]; // On récupère l'élément à cet index

    // Si l'élément est vide, on l'ajoute
    if (element == NULL)
    {
        ht->data[index] = add_element_st(key, data);
        ht->size++;
        ht->size_total++;
    }
    else
    { // Sinon, on parcourt la liste jusqu'à trouver un élément vide
        while (element != NULL)
        {
            // Si l'élément est connu on le remplace
            if (strcmp(element->key, key) == 0)
            {
                element->data = data;
                return;
            }
            if (element->next == NULL)
            {
                element->next = add_element_st(key, data);
                break;
            }
            element = element->next;
        }
        ht->size++;
    }
}

/* Crée et un élément et renvoie son adresse */
St_element *add_element_st(char *key, Symbol *data)
{
    St_element *element;
    CHECK(element = malloc(sizeof(St_element)));
    element->key = key;
    element->data = data;
    element->next = NULL;
    return element;
}

/* Récupère une donnée dans la hashtable */
Symbol *get_st(St *ht, char *key)
{
    int index = hash(key) % ht->capacity; // On calcue l'index

    St_element *element = ht->data[index]; // On récupère l'élément à cet index

    while (element != NULL)
    {
        if (strcmp(element->key, key) == 0)
            return element->data;
        element = element->next;
    }
    return NULL;
}

/* Affiche la hashtable */
void print_st(St *ht)
{
    printf("\n──────────────── Table des symboles ────────────────\n");
    printf("Nombre d'éléments : %d\n", ht->size);
    printf("Taille de la première couche de la table : %d\n\n", ht->capacity);

    int nb_occupe = 0;

    for (int i = 0; i < ht->capacity; i++)
    {
        St_element *element = ht->data[i];

        if (element != NULL)
        {
            printf("Case n°%d/%d\n", i, ht->capacity);
            do
            {
                printf("| '%s'", element->key);
                switch (element->data->type)
                {
                case VAR_S:
                    printf(" (var-");
                    switch (element->data->type_data)
                    {
                    case INTEGER_T:
                        printf("int)");
                        break;
                    case CONST_T:
                        printf("const)");
                        break;
                    case TEMP_T:
                        printf("temp)");
                        break;
                    default:
                        break;
                    }
                    break;
                case CONST_S:
                    printf(" (const)\n| %s", element->data->data);
                    break;
                case TEMP_S:
                    printf(" (temp)");
                    break;
                default:
                    break;
                }
                if (element->data->type != CONST)
                    printf(" (pos:%d)\n", element->data->position);
            } while ((element = element->next) != NULL);
            nb_occupe++;
            printf("\n");
        }
    }
    printf("Nombre de cases occupées : %d/%d\n", nb_occupe, ht->capacity);
}

/* Supprime la hashtable */
void destroy_st(St *ht)
{
    for (int i = 0; i < ht->capacity; i++)
    {
        St_element *element = ht->data[i];
        while (element != NULL)
        {
            St_element *next = element->next;
            free(element);
            element = next;
        }
    }
    free(ht->data);
    free(ht);
}

/* Étend la table */
void extend_st(St *ht)
{
    int new_capacity = ht->capacity * 2; // On double la taille de la table

    // On crée une nouvelle table
    St_element **new_data;
    CHECK(new_data = calloc(new_capacity, sizeof(St_element *)));

    // On parcourt l'ancienne table
    for (int i = 0; i < ht->capacity; i++)
    {
        St_element *element = ht->data[i];
        while (element != NULL)
        {
            // On recalcule l'index
            int index = hash(element->key) % new_capacity;

            // On ajoute l'élément à la nouvelle table
            // On sauvegarde le prochain élément
            St_element *next = element->next;
            element->next = new_data[index];
            new_data[index] = element;
            element = next;
        }
    }

    // On libère l'ancienne table
    free(ht->data);
    ht->data = new_data;
    ht->capacity = new_capacity;
}

/* Calcule le hash d'une chaine de caractère */
int hash(char *key)
{
    int hash = 0;
    int i = 0;
    char c = key[0];
    while (c != '\0')
    {
        i++;
        // Des carrés pour augmenter la dispersion en cas de chaines
        // similaire (key1 et key2 par exemple)
        hash += hash + (i * i) + (c * c);
        c = key[i];
    }
    return hash;
}
