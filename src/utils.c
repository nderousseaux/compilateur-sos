// Module contenant du code utile en tout genre

#include "../includes/imports.h"

/* Transforme une chaine de caractères en integer */
int to_int(char *str) {
	int res;
    int i = 0;
    char dec = 0;

    if (str[i] == '"') {
        i++;
        dec++;
    }

    if (str[i] == '+' || str[i] == '-')
        i++;

    // On teste les autres caractères
    for (; i < strlen(str); i++) {
        if (i == strlen(str) - 1 && str[i] == '"')
            break;
        if (str[i] < '0' || str[i] > '9') {
            fprintf(stderr, "Erreur: %s n'est pas un entier\n", str);
        }
    }

    res = atoi(str + dec);

    // On vérifie que res soit bien dans l'intervalle [-2^31, 2^31-1]
    if (res < -2147483648 || res > 2147483647)
        fprintf(stderr, "Erreur: %s n'est pas un entier sur 32 bits\n", str);

    return res;
}

/* Copie une chaine de caractères */
char * copy_string(char *s) {
    char *t;
    CHECK(t = calloc(strlen(s) + 255, sizeof(char)));
    strcpy(t, s);
    return t;
}

/* Vérifie que id correspond bien à la regex */
void check_id(char * dst) {
    if (strlen(dst) > 255) {
        fprintf(stderr, "Erreur: l'identifiant %s est trop long\n", dst);
        exit(EXIT_FAILURE);
    }

    if (dst[0] < 'a' || dst[0] > 'z') {
        fprintf(
            stderr,
            "id %s, syntax error\n",
            dst);
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < strlen(dst); i++) {
        if (dst[i] == '_' || dst[i] == '-')
            continue;
        if (dst[i] < 'a' || dst[i] > 'z') {
            if (dst[i] < 'A' || dst[i] > 'Z') {
                if (dst[i] < '0' || dst[i] > '9') {
                    fprintf(
                        stderr,
                        "id %s, syntax error\n",
                        dst);
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}
