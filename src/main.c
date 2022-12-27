#include "includes/SoS.h"
#define OUTPUT_FILE "out.asm"

void print_usage(char *program_name)
{
    printf("Usage:\t%s [-o output-file] [-d] <input file>\n", program_name);
    printf("\t%s [-c]\n\n", program_name);
    printf("\t-o\t\tSpecify output file (default: %s)\n", OUTPUT_FILE);
    printf("\t-d\t\tPrint symbols table and quad list during compilation \n");
    printf("\t-c\t\tPrint copyright\n");
    printf("\t-h\t\tPrint this help\n");
}

void print_copyright()
{
    char *file = {
#include "includes/copyright"
    };
    printf("%s", file);
}

int parse_args(char **finput, char **foutput, int argc, char **argv)
{
    char debug = 0;
    // On récupère les options
    int opt;
    while ((opt = getopt(argc, argv, "dcho:")) != -1)
    {
        switch (opt)
        {
        case 'o':
            *foutput = optarg;
            break;
        case 'h':
            print_usage(argv[0]);
            exit(EXIT_SUCCESS);
            break;
        case 'c':
            print_copyright();
            exit(EXIT_SUCCESS);
            break;
        case 'd':
            debug = 1;
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
