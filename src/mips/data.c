/* S'occupe des données dans code mips 
	- Génére les constantes au début du code
*/

#include "../includes/imports.h"

FILE *f;  // Fichier de sortie

/* Génére les constantes au début du code */
void gen_data() {
	fprintf(f, ".data\n");
}