/* Module contenant les fonctions relatives
* à la gestion des quads pour les structures de contrôle
*/

#ifndef SRC_INCLUDES_INTERMEDIATE_CTRL_QL_H_
#define SRC_INCLUDES_INTERMEDIATE_CTRL_QL_H_

#include "./quads.h"

// Structure d'une liste de quads de contrôle
typedef struct Ctrl_ql {
	Ql *tru;  // Liste des quads incomplets à exécuter si Vrai
	Ql *fal;  // Liste des quads incomplets à exécuter si Faux
	Ql *next;  // Liste des quads incomplets à exécuter après
} Ctrl_ql;

#endif  // SRC_INCLUDES_INTERMEDIATE_CTRL_QL_H_
