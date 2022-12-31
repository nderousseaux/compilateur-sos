// Module permettant de passer du code intermédiaire au code MIPS

#include "includes/imports.h"

FILE *f; // Fichier de sortie


// On calcule le numéro du temporaire modulo 2
int ind(char * s1){
    int taille=strlen(s1);
    int modulo=s1[taille-1]%2;
    return modulo;
}


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
    fprintf(f, "\tfin_de_fichier: .asciiz\t%s\n", "\"\\n\"");
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
        fprintf(f, "\n");
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
            gen_end(quad.operand1.value);
            exit = 1;
            break;

            case OP_PLUS:
                // On génère le code de la somme
                gen_operation(quad);
                break;
            case OP_MOINS:
                if(quad.operand2.type==EMPTY){
                    gen_unaire(quad);
                }
                else{
                    // On génère le code de la soustraction
                    gen_operation(quad);
                }
                break;
            case OP_FOIS:
                // On génère le code de la multiplication
                gen_operation(quad);
                break;
            case OP_DIVISION:
                // On génère le code de la division
                gen_operation(quad);
                break;
            case OP_MODULO:
                // On génère le code du reste
                gen_operation(quad);
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

    fprintf(f, "\n\t# On fait l'affectation %s := %s\n",quad.result.value,quad.operand1.value);
    //On teste le type du premier opérande
    switch(quad.operand1.type){
        case INTEGER:
            fprintf(f,"\tli\t$t2,\t%s\n", quad.operand1.value);
            fprintf(f,"\tsw\t$t2,\t%d($fp)\n", pos_res);
            break;
        case VAR:
            fprintf(f, "\n\t# On met %s dans %s\n", quad.operand1.value, quad.result.value);
            fprintf(f, "\tla\t$t2,\t%s\n", quad.operand1.value);
            fprintf(f, "\tsw\t$t2,\t%d($fp)\n", pos_res);
            break;
        case ID:
            pos_arg1 = get_address(quad.operand1.value);
            fprintf(f,"\tlw\t$t3,\t%d($fp)\n", pos_arg1);
            fprintf(f,"\tsw\t$t3,\t%d($fp)\n", pos_res);
            break;
        case TEMP:
            pos_arg1 = get_address(quad.operand1.value);
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
        fprintf(f, "\tli\t$v0,\t4\n");
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
    /*fprintf(f, "\n\tla $a0, fin_de_fichier");
    fprintf(f, "\n\tli $v0, 4\n");
    fprintf(f, "\tsyscall\n");
    */

    fprintf(f, "\n\t# On génère le code de terminaison\n");
    fprintf(f, "\tli\t$a0,\t%s\n", exit_code);
    fprintf(f, "\tli\t$v0,\t17\n");
    fprintf(f, "\tsyscall\n");
}

void gen_plus(Quad quad) {
    /* On génère le code de somme */

    fprintf(f, "\n\t# On génère le code de somme");
    
    fprintf(f,"\n\t# On fait %s := %s + %s\n",quad.result.value, quad.operand1.value, quad.operand2.value);
    int ind_res=ind(quad.result.value);
    int ind_op1=ind(quad.operand1.value); 
    int ind_op2=ind(quad.operand2.value);

    if(quad.operand1.type==INTEGER && quad.operand2.type==INTEGER){
        fprintf(f, "\tli\t$t%i,\t%s\n", ind_res,quad.operand1.value);
        fprintf(f, "\taddi\t$t%i,\t$t%i,\t%s\n", ind_res, ind_res, quad.operand2.value);
    }
    else if(quad.operand1.type==INTEGER && quad.operand2.type==TEMP){
        fprintf(f, "\taddi\t$t%i,\t$t%i,\t%s\n", ind_res, ind_op2, quad.operand1.value);
    }
    else if(quad.operand1.type==TEMP && quad.operand2.type==INTEGER){
        fprintf(f, "\taddi\t$t%i,\t$t%i,\t%s\n", ind_res, ind_op1, quad.operand2.value);
    }
    else{
        fprintf(f, "\tadd\t$t%i,\t$t%i,\t$t%i\n", ind_res, ind_op1, ind_op2);
    }
}

void gen_operation(Quad quad) {
    /* On génère le code de l'opération */
    char op=operator_to_string(quad.op);
    fprintf(f, "\n\t# On génère le code de l'opération");
    fprintf(f,"\n\t# On fait %s := %s %c %s\n",quad.result.value, quad.operand1.value, op, quad.operand2.value);

    int pos_op1;
    int pos_op2;
    int pos_res = get_address(quad.result.value);

    switch(quad.operand1.type){
        case(INTEGER):
            fprintf(f, "\tli\t$t0,\t%s\n", quad.operand1.value);
            break;
        case(TEMP):
            pos_op1 = get_address(quad.operand1.value);
            fprintf(f, "\tlw\t$t0,\t%d($fp)\n", pos_op1);
            break;
        case(ID):
            pos_op1 = get_address(quad.operand1.value);
            fprintf(f, "\tlw\t$t0,\t%d($fp)\n", pos_op1);
            break;
        default:
            break;
    }

     switch(quad.operand2.type){
        case(INTEGER):
            fprintf(f, "\tli\t$t1,\t%s\n", quad.operand2.value);
            break;
        case(TEMP):
            pos_op2 = get_address(quad.operand2.value);
            fprintf(f, "\tlw\t$t1,\t%d($fp)\n", pos_op2);
            break;
        case(ID):
            pos_op2 = get_address(quad.operand2.value);
            fprintf(f, "\tlw\t$t1,\t%d($fp)\n", pos_op2);
            break;
        default:
            break;
    }
    switch(quad.op){
        case OP_PLUS:
            fprintf(f, "\tadd\t$t0,\t$t0,\t$t1\n");
            break;
        case OP_MOINS:
            fprintf(f, "\tsub\t$t0,\t$t0,\t$t1\n");
            break;
        case OP_FOIS:
            fprintf(f, "\tmult\t$t0,\t$t1\n");
            fprintf(f, "\tmflo\t$t0\n");
            break;
        case OP_DIVISION:
            fprintf(f, "\tdiv\t$t0,\t$t1\n");
            fprintf(f, "\tmflo\t$t0\n");
            break;
        case OP_MODULO:
            fprintf(f, "\tdiv\t$t0,\t$t1\n");
            fprintf(f, "\tmfhi\t$t0\n");
        default:
            break;
    }
    fprintf(f,"\tsw\t$t0,\t%d($fp)\n", pos_res);
}



void gen_moins(Quad quad) {
    /* On génère le code de la soustraction */

    fprintf(f, "\n\t# On génère le code de la soustraction");
    fprintf(f,"\n\t# On fait %s = %s - %s\n",quad.result.value, quad.operand1.value, quad.operand2.value);
    

    int pos_op1;
    int pos_op2;
    int pos_res = get_address(quad.result.value);

    switch(quad.operand1.type){
        case(INTEGER):
            fprintf(f, "\tli\t$t0,\t%s\n", quad.operand1.value);
            break;
        case(TEMP):
            pos_op1 = get_address(quad.operand1.value);
            fprintf(f, "\tlw\t$t0,\t%d($fp)\n", pos_op1);
        default:
            break;
    }

     switch(quad.operand2.type){
        case(INTEGER):
            fprintf(f, "\tli\t$t1,\t%s\n", quad.operand2.value);
            break;
        case(TEMP):
            pos_op2 = get_address(quad.operand2.value);
            fprintf(f, "\tlw\t$t1,\t%d($fp)\n", pos_op2);
            break;
        default:
            break;
    }
    fprintf(f, "\tsub\t$t0,\t$t0,\t$t1\n");
    fprintf(f,"\tsw\t$t0,\t%d($fp)\n", pos_res);
}

void gen_unaire(Quad quad) {
    /* On génère le code de l'opération unaire */

    int pos_op1;
    int pos_res = get_address(quad.result.value);


    fprintf(f,"\n\t# On fait %s := - %s\n",quad.result.value, quad.operand1.value);

    switch(quad.operand1.type){
        case INTEGER:
            fprintf(f, "\tli\t$t0,\t-%s\n",quad.operand1.value);
            break;
        case TEMP:
            pos_op1 = get_address(quad.operand1.value);
            fprintf(f, "\tlw\t$t0,\t%d($fp)\n", pos_op1);
            fprintf(f,"\tsubu\t$t0,\t$zero,\t$t0\n");
            break;
        case ID:
            pos_op1 = get_address(quad.operand1.value);
            fprintf(f, "\tlw\t$t0,\t%d($fp)\n", pos_op1);
            fprintf(f,"\tsubu\t$t0,\t$zero,\t$t0\n");
            break;
        default:
            break;
    }
    fprintf(f,"\tsw\t$t0,\t%d($fp)\n", pos_res);

}

void gen_fois(Quad quad) {
    /* On génère le code de somme */

    fprintf(f, "\n\t# On génère le code de la multiplication");
    fprintf(f,"\n\t# On fait %s = %s * %s\n",quad.result.value, quad.operand1.value, quad.operand2.value);
    
    int pos_op1;
    int pos_op2;
    int pos_res = get_address(quad.result.value);

    switch(quad.operand1.type){
        case(INTEGER):
            fprintf(f, "\tli\t$t0,\t%s\n", quad.operand1.value);
            break;
        case(TEMP):
            pos_op1 = get_address(quad.operand1.value);
            fprintf(f, "\tlw\t$t0,\t%d($fp)\n", pos_op1);
        default:
            break;
    }

     switch(quad.operand2.type){
        case(INTEGER):
            fprintf(f, "\tli\t$t1,\t%s\n", quad.operand2.value);
            break;
        case(TEMP):
            pos_op2 = get_address(quad.operand2.value);
            fprintf(f, "\tlw\t$t1,\t%d($fp)\n", pos_op2);
            break;
        default:
            break;
    }
    fprintf(f, "\tmult\t$t0,\t$t1\n");
    fprintf(f, "\tmflo\t$t0\n");
    fprintf(f, "\tsw\t$t0,\t%d($fp)\n", pos_res);
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



