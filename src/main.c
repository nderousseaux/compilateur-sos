#include "includes/imports.h"

/* Affiche le message d'aide avec l'option -h */
void print_usage(char *program_name)
{
    printf("Usage:\t%s [-o output-file] [-t] <input file>\n", program_name);
    printf("\t%s [-c]\n\n", program_name);
    printf("\t-o\t\tSpecify output file (default: %s)\n", OUTPUT_FILE);
    printf("\t-t\t\tPrint symbols table and quad list during compilation \n");
    printf("\t-c\t\tPrint copyright\n");
    printf("\t-v\t\tPrint this to show the project's members\n");
    printf("\t-h\t\tPrint this help\n");
}

/* Affiche le logo avec l'option -c */
void print_copyright()
{
    char *file = {
#include "includes/copyright"
    };
    printf("%s", file);
}

void print_member()
{
    printf("Membres du projet:\n");
    printf("Arnaud FILIPPI\n");
    printf("Charles JARDOT\n");
    printf("Thomas Poirier\n");
    printf("Nathanaël DEROUSSEAUX\n\n\n");
}

/* Parse les arguments, renvoie 0 si pas debug, 1 si mode debug */
char parse_args(char **finput, char **foutput, int argc, char **argv)
{
    char debug = 0;

    // On récupère les options
    int opt;

    struct option long_options[] = {
        {"version", required_argument, NULL, 'v'},
        {"tos", required_argument, NULL, 't'},
        {0, 0, 0, 0}};

    int option_index = 0;

    char *optstring = "tvhco:";

    while ((opt = getopt_long(argc, argv, optstring, long_options, &option_index)) != -1)
    {
        switch (opt)
        {
        case 'o': // Fichier de sortie
            *foutput = optarg;
            break;
        case 'h': // Aide
            print_usage(argv[0]);
            exit(EXIT_SUCCESS);
            break;
        case 'c': // Logo
            print_copyright();
            exit(EXIT_SUCCESS);
            break;
        case 't': // Table des symboles
            debug = 1;
            break;
        case 'v': // Membres du groupe
            print_member();
            break;
        default:
            print_usage(argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // On récupère le fichier d'entrée (un seul)
    if (optind == argc - 1)
    {
        *finput = argv[optind];
    }
    else
    {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    return debug;
}

/* Ouvre un fichier */
FILE *open_file(char *file_name, char *mode)
{
    FILE *file = fopen(file_name, mode);
    if (file == NULL)
    {
        fprintf(stderr, "Error: cannot open file %s\n", file_name);
        exit(EXIT_FAILURE);
    }
    return file;
}

int main(int argc, char *argv[])
{
    char *finput = NULL;
    char *foutput = OUTPUT_FILE;
    FILE *input, *output;

    // On récupère les arguments
    char debug = parse_args(&finput, &foutput, argc, argv);

    // On ouvre les deux fichiers
    input = open_file(finput, "r");
    output = open_file(foutput, "w");

    // On execute le programme
    int ret = SoS(input, output, debug);

    // On ferme les fichiers
    fclose(input);
    fclose(output);

    return ret;
}
