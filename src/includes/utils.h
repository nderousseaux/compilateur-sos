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

#endif  // SRC_INCLUDES_UTILS_H_
