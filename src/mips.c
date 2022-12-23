// Module permettant de passer du code intermédiaire au code MIPS

#include "includes/mips.h"
#include "includes/symbols.h"

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
        case OP_EQUAL:
            gen_equal(quad);
            break;
        default:
            break;
        }
    }

    // Si on n'a pas trouvé d'instruction de sortie, on génère le code de terminaison
    if (!exit)
    {
        gen_end(0);
    }
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
    // On récupère la variable dans la pile
    //  int pos_arg1 = get_address(quad.operand1.value);
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
        // TOOD -> $1
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

void gen_equal(Quad quad)
{
    if (quad.operand1.type == INTEGER && quad.operand2.type == INTEGER)
    {
        fprintf(f, "li $t0,%d\n", quad.operand1.integer_value);
        fprintf(f, "li $t1,%d\n", quad.operand2.integer_value);
        fprintf(f, "beq $t0,$t1,test\n");
    }
}

/* On génère le code de terminaison */
void gen_end(int exit_code)
{
    fprintf(f, "\n\t# On génère le code de terminaison\n");
    fprintf(f, "\tli\t$a0,\t%d\n", exit_code);
    fprintf(f, "\tli\t$v0,\t17\n");
    fprintf(f, "\tsyscall\n");
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
