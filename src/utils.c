// Module contenant du code utile en tout genre

#include "../includes/imports.h"

/* Transforme une chaine de caractères en integer */
int to_int(char *str) {
	int res;
    int i = 0;
    char neg = 0;
    if (str[0] == '+') {
        i = 1;
	} else if (str[0] == '-') {
        i = 1;
        neg = 1;
    }

    // On teste les autres caractères
    for (; i < strlen(str); i++) {
        if (str[i] < '0' || str[i] > '9') {
            fprintf(stderr, "Erreur: %s n'est pas un entier\n", str);
        }
    }

    if (neg)
        res = -atoi(str + 1);
    else
        res = atoi(str);

    // On vérifie que res soit bien dans l'intervalle [-2^31, 2^31-1]
    if (res < -2147483648 || res > 2147483647)
        fprintf(stderr, "Erreur: %s n'est pas un entier sur 32 bits\n", str);

    return res;
}
