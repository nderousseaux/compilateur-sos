/* S'occupe des données dans code mips 
	- Génére les constantes au début du code
*/

#include "../includes/imports.h"

FILE *f;  // Fichier de sortie

/* Génére les constantes au début du code */
void gen_data() {
	fprintf(f, ".data\n");

	// On enregistre toute les constantes de la table des symboles
    for (int i = 0; i < symbols_table->capacity; i++) {
        St_element *element = symbols_table->data[i];
        while (element != NULL) {
            if (element->data->type == CONST)
                fprintf(f, "\t%s: .asciiz\t%s\"\n",
					element->key, element->data->data);

            element = element->next;
        }
    }
}
