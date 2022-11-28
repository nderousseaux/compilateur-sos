#include "includes/SoS.h"

#define OUTPUT_FILE "out.asm"

void print_usage(char * program_name) {
    printf("Usage: %s [-o output-file] <input file>\n", program_name);
    exit(EXIT_FAILURE);
}

void parse_args(char ** finput, char ** foutput, int argc, char ** argv) {
    // On récupère les options
    int opt;
    while ((opt = getopt(argc, argv, "ho:")) != -1) {
        switch (opt) {
            case 'o':
                *foutput = optarg;
                break;
            case 'h':
                print_usage(argv[0]);
                break;
            default:
                print_usage(argv[0]);
        }
    }

    // On récupère le fichier d'entrée (un seul)
    if (optind == argc-1) {
        *finput = argv[optind];
    } else {
        print_usage(argv[0]);
    }
}

FILE * open_file(char * file_name, char * mode) {
    FILE * file = fopen(file_name, mode);
    if (file == NULL) {
        fprintf(stderr, "Error: cannot open file %s\n", file_name);
        exit(EXIT_FAILURE);
    }
    return file;
}

int main(int argc, char *argv[]) {
    char * finput = NULL;
    char * foutput = OUTPUT_FILE;
    FILE * input, * output;

    // On récupère les arguments
    parse_args(&finput, &foutput, argc, argv);

    // On ouvre les deux fichiers
    input = open_file(finput, "r");
    output = open_file(foutput, "w");

    // On execute le programme
    int ret = SoS(input, output);

    // On ferme les fichiers
    fclose(input);
    fclose(output);

    return ret;
}
