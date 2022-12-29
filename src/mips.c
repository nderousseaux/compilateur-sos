// Module permettant de passer du code intermédiaire au code MIPS

#include "includes/imports.h"

FILE *f; // Fichier de sortie

/* Récupère l'index de la variable dans la pile */
int get_address(char *name)
{
    // On récupère la variable dans la table des symboles
    Symbol *symbol = get_st(symbols_table, name);
    if (symbol == NULL)
    {
        return -1;
    }
    else
    {
        // On récupère l'index de la variable dans la pile
        return symbol->position * 4;
    }

    return -1;
}

/* Génère les données */
void gen_data()
{
    fprintf(f, ".data\n");

    // On enregistre toute les constantes de la table des symboles
    for (int i = 0; i < symbols_table->capacity; i++)
    {
        St_element *element = symbols_table->data[i];
        while (element != NULL)
        {
            if (element->data->position == -1)
                fprintf(f, "\t%s: .asciiz\t%s\n", element->key, element->data->data);
            }

            element = element->next;
        }
    }
}

/* Génère le code */
void gen_code()
{

    fprintf(f, ".text\n");
    fprintf(f, "main:\n");

    // On génère la pile
    gen_stack();

    char exit = 0;

    // Pour chaque élément de la table des quadruplets
    Quad quad;
    for (int i = 0; i < quad_list->size; i++)
    {
        quad = quad_list->data[i];
        fprintf(f, "\t%s:", gen_flag_quad(i));

        switch (quad.op)
        {
        case OP_ASSIGN:
            // On génère le code d'affectation
            gen_assign(quad);
            break;
        case OP_ECHO:
            // On génère le code d'affichage
            gen_echo(quad);
            break;
        case OP_EXIT:
            // On génère le code de terminaison
            gen_end(quad.operand1.integer_value);
            exit = 1;
            break;

            case OP_PLUS:
                gen_plus(quad);
                break;
        case OP_EQUAL:
            gen_equal(quad);
            break;
        case OP_NEQUAL:
            gen_nequal(quad);
            break;
        case OP_STSUP:
            gen_stsup(quad);
            break;
        case OP_SUPEQ:
            gen_supeq(quad);
            break;
        case OP_STINF:
            gen_stinf(quad);
            break;
        case OP_INFEQ:
            gen_infeq(quad);
            break;
        case OP_GOTO:
            gen_goto(quad);
            break;
        default:
            break;
        }
    }

    // Si on n'a pas trouvé d'instruction de sortie, on génère le code de terminaison
    if (!exit)
    {
        gen_end(0);
        fprintf(f, "\t# On termine le programme\n");
        fprintf(f, "quad_%d:", quad_list->size);
    }
}

char *gen_flag_quad(int idx_quad)
{
    char *flag;
    CHECK(flag = calloc(sizeof(char), 10));
    sprintf(flag, "quad_%d", idx_quad);
    return (flag);
}

/* Génère la pile */
void gen_stack()
{
    // On alloue la mémoire pour la pile
    fprintf(f, "\n\t# On alloue la mémoire pour la pile\n");
    fprintf(f, "\taddiu\t$sp,\t$sp,\t-%d\n", symbols_table->last_pos * 4 + 4);
    fprintf(f, "\tsw\t\t$fp,\t%d($sp)\n", symbols_table->last_pos * 4);
    fprintf(f, "\tmove\t$fp,\t$sp\n");
}

/* Génère le code d'affectation */
void gen_assign(Quad quad)
{
    //On récupère la variable dans la pile
    int pos_arg1 = get_address(quad.operand1.value);
    int pos_res = get_address(quad.result.value);

    // On teste le type du premier opérande
    switch (quad.operand1.type)
    {
    case INTEGER:
        fprintf(f, "\n\t# On met %d dans %s\n", quad.operand1.integer_value, quad.result.value);
        fprintf(f, "\tli\t$t2,\t%d\n", quad.operand1.integer_value);
        fprintf(f, "\tsw\t$t2,\t%d($fp)\n", pos_res);
        break;

    case VAR:
        fprintf(f, "\n\t# On met %s dans %s\n", quad.operand1.value, quad.result.value);
        fprintf(f, "\tla\t$t2,\t%s\n", quad.operand1.value);
        fprintf(f, "\tsw\t$t2,\t%d($fp)\n", pos_res);
        break;

        case ID:
            fprintf(f,"\tlw\t$t3,\t%d($fp)\n", pos_arg1);
            fprintf(f,"\tsw\t$t3,\t%d($fp)\n", pos_res);
            break;

        case TEMP:
            fprintf(f,"\tlw\t$t3,\t%d($fp)\n", pos_arg1);
            fprintf(f,"\tsw\t$t3,\t%d($fp)\n", pos_res);
            break;
        default:
            break;
    }
}

/* Génére le code d'affichage */
void gen_echo(Quad quad)
{
    int pos_arg1 = get_address(quad.operand1.value);

    fprintf(f, "\n\t# On affiche %s\n", quad.operand1.value);
    // On teste le type du premier opérande
    switch (quad.operand1.type)
    {
    case VAR:
        fprintf(f, "\tla\t$a0,\t%s\n", quad.operand1.value);
        fprintf(f, "\tli\t$v0,\t4\n");
        fprintf(f, "\tsyscall\n");
        break;

    case ID:
        fprintf(f, "\tlw\t$a0,\t%d($fp)\n", pos_arg1);
        fprintf(f, "\tli\t$v0,\t1\n");
        fprintf(f, "\tsyscall\n");

    default:
        break;
    }
}

void gen_goto(Quad quad)
{
    fprintf(f, "\n\tj %s\n", gen_flag_quad(quad.result.integer_value));
}

void gen_equal(Quad quad)
{
    int pos_arg1;
    int pos_arg2;

    // TODO: gérer le commentaire
    fprintf(f, "\n");
    // fprintf(f, "\n\t# On compare %s et %s\n", quad.operand1.value, quad.operand2.value);
    switch (quad.operand1.type)
    {
    case INTEGER:
        fprintf(f, "\tli\t$t0,\t%d\n", quad.operand1.integer_value);
        break;
    case ID:
        pos_arg1 = get_address(quad.operand1.value);
        fprintf(f, "\tlw\t$t0,\t%d($fp)\n", pos_arg1);
        break;
    default:
        break;
    }
    switch (quad.operand2.type)
    {
    case INTEGER:
        fprintf(f, "\tli\t$t1,\t%d\n", quad.operand2.integer_value);
        break;
    case ID:
        pos_arg2 = get_address(quad.operand2.value);
        fprintf(f, "\tlw\t$t1,\t%d($fp)\n", pos_arg2);
        break;
    default:
        break;
    }

    fprintf(f, "\tbeq\t$t0,\t$t1,\t%s\n", gen_flag_quad(quad.result.integer_value));
}

void gen_nequal(Quad quad)
{
    int pos_arg1;
    int pos_arg2;

    // TODO: gérer le commentaire
    fprintf(f, "\n");
    // fprintf(f, "\n\t# On compare %s et %s\n", quad.operand1.value, quad.operand2.value);
    switch (quad.operand1.type)
    {
    case INTEGER:
        fprintf(f, "\tli\t$t0,\t%d\n", quad.operand1.integer_value);
        break;
    case ID:
        pos_arg1 = get_address(quad.operand1.value);
        fprintf(f, "\tlw\t$t0,\t%d($fp)\n", pos_arg1);
        break;
    default:
        break;
    }
    switch (quad.operand2.type)
    {
    case INTEGER:
        fprintf(f, "\tli\t$t1,\t%d\n", quad.operand2.integer_value);
        break;
    case ID:
        pos_arg2 = get_address(quad.operand2.value);
        fprintf(f, "\tlw\t$t1,\t%d($fp)\n", pos_arg2);
        break;
    default:
        break;
    }

    fprintf(f, "\tbne\t$t0,\t$t1,\t%s\n", gen_flag_quad(quad.result.integer_value));
}

void gen_stsup(Quad quad)
{
    int pos_arg1;
    int pos_arg2;

    // TODO: gérer le commentaire
    fprintf(f, "\n");
    // fprintf(f, "\n\t# On compare %s et %s\n", quad.operand1.value, quad.operand2.value);
    switch (quad.operand1.type)
    {
    case INTEGER:
        fprintf(f, "\tli\t$t0,\t%d\n", quad.operand1.integer_value);
        break;
    case ID:
        pos_arg1 = get_address(quad.operand1.value);
        fprintf(f, "\tlw\t$t0,\t%d($fp)\n", pos_arg1);
        break;
    default:
        break;
    }
    switch (quad.operand2.type)
    {
    case INTEGER:
        fprintf(f, "\tli\t$t1,\t%d\n", quad.operand2.integer_value);
        break;
    case ID:
        pos_arg2 = get_address(quad.operand2.value);
        fprintf(f, "\tlw\t$t1,\t%d($fp)\n", pos_arg2);
        break;
    default:
        break;
    }

    fprintf(f, "\tbgt\t$t0,\t$t1,\t%s\n", gen_flag_quad(quad.result.integer_value));
}

void gen_supeq(Quad quad)
{
    int pos_arg1;
    int pos_arg2;

    // TODO: gérer le commentaire
    fprintf(f, "\n");
    // fprintf(f, "\n\t# On compare %s et %s\n", quad.operand1.value, quad.operand2.value);
    switch (quad.operand1.type)
    {
    case INTEGER:
        fprintf(f, "\tli\t$t0,\t%d\n", quad.operand1.integer_value);
        break;
    case ID:
        pos_arg1 = get_address(quad.operand1.value);
        fprintf(f, "\tlw\t$t0,\t%d($fp)\n", pos_arg1);
        break;
    default:
        break;
    }
    switch (quad.operand2.type)
    {
    case INTEGER:
        fprintf(f, "\tli\t$t1,\t%d\n", quad.operand2.integer_value);
        break;
    case ID:
        pos_arg2 = get_address(quad.operand2.value);
        fprintf(f, "\tlw\t$t1,\t%d($fp)\n", pos_arg2);
        break;
    default:
        break;
    }

    fprintf(f, "\tbge\t$t0,\t$t1,\t%s\n", gen_flag_quad(quad.result.integer_value));
}

void gen_stinf(Quad quad)
{
    int pos_arg1;
    int pos_arg2;

    // TODO: gérer le commentaire
    fprintf(f, "\n");
    // fprintf(f, "\n\t# On compare %s et %s\n", quad.operand1.value, quad.operand2.value);
    switch (quad.operand1.type)
    {
    case INTEGER:
        fprintf(f, "\tli\t$t0,\t%d\n", quad.operand1.integer_value);
        break;
    case ID:
        pos_arg1 = get_address(quad.operand1.value);
        fprintf(f, "\tlw\t$t0,\t%d($fp)\n", pos_arg1);
        break;
    default:
        break;
    }
    switch (quad.operand2.type)
    {
    case INTEGER:
        fprintf(f, "\tli\t$t1,\t%d\n", quad.operand2.integer_value);
        break;
    case ID:
        pos_arg2 = get_address(quad.operand2.value);
        fprintf(f, "\tlw\t$t1,\t%d($fp)\n", pos_arg2);
        break;
    default:
        break;
    }

    fprintf(f, "\tblt\t$t0,\t$t1,\t%s\n", gen_flag_quad(quad.result.integer_value));
}

void gen_infeq(Quad quad)
{
    int pos_arg1;
    int pos_arg2;

    // TODO: gérer le commentaire
    fprintf(f, "\n");
    // fprintf(f, "\n\t# On compare %s et %s\n", quad.operand1.value, quad.operand2.value);
    switch (quad.operand1.type)
    {
    case INTEGER:
        fprintf(f, "\tli\t$t0,\t%d\n", quad.operand1.integer_value);
        break;
    case ID:
        pos_arg1 = get_address(quad.operand1.value);
        fprintf(f, "\tlw\t$t0,\t%d($fp)\n", pos_arg1);
        break;
    default:
        break;
    }
    switch (quad.operand2.type)
    {
    case INTEGER:
        fprintf(f, "\tli\t$t1,\t%d\n", quad.operand2.integer_value);
        break;
    case ID:
        pos_arg2 = get_address(quad.operand2.value);
        fprintf(f, "\tlw\t$t1,\t%d($fp)\n", pos_arg2);
        break;
    default:
        break;
    }

    fprintf(f, "\tble\t$t0,\t$t1,\t%s\n", gen_flag_quad(quad.result.integer_value));
}

/* On génère le code de terminaison */
void gen_end(char * exit_code)
{
    fprintf(f, "\n\t# On génère le code de terminaison\n");
    fprintf(f, "\tli\t$a0,\t%s\n", exit_code);
    fprintf(f, "\tli\t$v0,\t17\n");
    fprintf(f, "\tsyscall\n");
}

void gen_plus(Quad quad) {
    /* On génère le code de somme */

    fprintf(f, "\n\t# On génère le code de somme\n");
    int pos_res = get_address(quad.result.value);
    int pos_arg1;
    int pos_arg2;
    switch(quad.operand1.type){
        case INTEGER:
            fprintf(f, "\tli\t$t2,\t%s\n", quad.operand1.value);
            break;
        case TEMP:
            pos_arg1 = get_address(quad.operand1.value);
            fprintf(f,"\tlw\t$t3,\t%d($fp)\n", pos_arg1);
            break;
        default:
            break;
    }

    switch(quad.operand2.type){
        case INTEGER:
            fprintf(f, "\taddi\t$t2,\t$t2,\t%s\n", quad.operand2.value);
            break;
        case TEMP:
            pos_arg2 = get_address(quad.operand2.value);
            fprintf(f,"\tlw\t$t3,\t%d($fp)\n", pos_arg2);
            fprintf(f,"\tadd\t$t2,\t$t2,\t$t3");
            break;
        default:
            break;
    }
    fprintf(f, "\tsw\t$t2,\t%d($fp)\n", pos_res);
}

/* Génére code assembleur final dans le .asm */
void gen_mips(FILE *output, char debug)
{
    f = output;

    // On affiche la table des symboles et des quadruplets
    if (debug)
    {
        print_st(symbols_table);
        print_quad_list(quad_list);
    }

    // On intègre les données
    gen_data();

    // On intègre le code
    gen_code();
}



