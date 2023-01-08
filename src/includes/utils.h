// Module contenant du code utile en tout genre

#ifndef SRC_INCLUDES_UTILS_H_
#define SRC_INCLUDES_UTILS_H_

// Affiche un message d'erreur et quitte le programme
#define CHECK(x) \
  do { \
    if (!(x)) { \
      fprintf(stderr, "%s:%d: ", __func__, __LINE__); \
      if (errno == 0)  \
        errno = ECANCELED; \
      perror(#x); \
      exit(EXIT_FAILURE); \
    } \
  } while (0)


/* Transforme une chaine de caractères en integer */
int to_int(char *str);

/* Vérifie si une chaine de caractère peut être un string */
char is_int(char *str);

/* Copie une chaine de caractères */
char * copy_string(char *s);

/* Vérifie que id correspond bien à la regex */
void check_id(char * dst);

/* Retire les guillemets à une chaine (si ils existents)*/
char * trim(char * str);

#endif  // SRC_INCLUDES_UTILS_H_
