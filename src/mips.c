// Module permettant de passer du code intermédiaire au code MIPS

#include "includes/mips.h"

/* Génère les données */
void gen_data() {
    fprintf(f, ".data\n");
}

/* Génère le code */
void gen_code() {
    fprintf(f, ".text\n");
    fprintf(f, "main:\n");

    // Pour chaque élément de la table des quadruplets
    Quad quad;
    for (int i = 0; i < quad_list->size; i++) {
        quad = quad_list->data[i];
        switch (quad.op) {
            case OP_EXIT:
                // On génère le code de terminaison
                gen_end(quad.operand1.value);
                break;
            default:
                break;
        }
    }
}

/* On génère le code de terminaison */
void gen_end(int exit_code) {
    fprintf(f, "li $a0, %d\n", exit_code);
    fprintf(f, "li $v0, 17\n");
    fprintf(f, "syscall\n");
}

/* Génére code assembleur final dans le .asm */
void gen_mips(FILE * output) {
    f = output;
    // On intègre les données
    gen_data();

    // On intègre le code
    gen_code();
}
