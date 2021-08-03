/****************************************************/
/*              Gerador de codigos                  */
/*                 Isadora Muniz                    */
/****************************************************/

#include "globals.h"
#include "symtab.h"
#include "analyze.h"
#include "assmb.h"
#include "cgen.h"
#include <stdio.h>
#include <string.h>

char *escopoAux = "global";
int temp_counter = 0;
int n_line = 0;

void insert(instruction instruction_name, char *op1, char *op2, char *op3)
{
    quadruple *new;
    new = (quadruple *)malloc(sizeof(quadruple));

    new->next = NULL;
    new->instruction_name = instruction_name;
    new->op1.name = op1;
    new->op2.name = op2;
    new->op3.name = op3;

    if (lista_quad->tamanho == 0)
        lista_quad->first = new;

    else
        lista_quad->last->next = new;

    lista_quad->last = new;
    lista_quad->tamanho++;
}

void temp_counter_func(void)
{
    temp_counter++;

    if (temp_counter == 4)
    {
        temp_counter = 1;
    }
}

void print_quadruple(quadruple *aux_quadruple)
{
    char *aux;
    FILE *output_file = NULL;

    output_file = fopen("output-files/intermediate_code.txt", "w");

    if (output_file == NULL)
    {
        printf("Operation failed\n");
        return;
    }

    while (aux_quadruple != NULL)
    {
        switch (aux_quadruple->instruction_name)
        {

        case nop:
            aux = "nop";
            break;
        case halt:
            aux = "halt";
            break;
        case store:
            aux = "store";
            break;
        case fun:
            aux = "fun";
            break;
        case arg:
            aux = "arg";
            break;
        case setArg:
            aux = "setArg";
            break;
        case call:
            aux = "call";
            break;
        case end:
            aux = "end";
            break;
        case load:
            aux = "load";
            break;
        case alloc:
            aux = "alloc";
            break;
        case ret:
            aux = "ret";
            break;
        case label:
            aux = "label";
            break;
        case IGLIGL:
            aux = "==";
            break;
        case DIF:
            aux = "!=";
            break;
        case MEN:
            aux = "<";
            break;
        case MAI:
            aux = ">";
            break;
        case MEIGL:
            aux = "<=";
            break;
        case MAIGL:
            aux = ">=";
            break;
        case SOM:
            aux = "+";
            break;
        case SUBT:
            aux = "-";
            break;
        case MUL:
            aux = "*";
            break;
        case DIVI:
            aux = "/";
            break;
        case imed:
            aux = "imed";
            break;
        case jump:
            aux = "jump";
            break;
        case beq:
            aux = "beq";
            break;
        }

        fprintf(output_file, "( %-6s,", aux);

        switch (aux_quadruple->op1.type)
        {
        case general:

            fprintf(output_file, " %-10s,", aux_quadruple->op1.name);
            break;
        case id:
            break;
        case regTemporary:

            fprintf(output_file, " t_%-8d,", aux_quadruple->op1.value);

            break;
        case constant:

            fprintf(output_file, " %-10d,", aux_quadruple->op1.value);
            break;
        case labelk:

            fprintf(output_file, " L_%-8d,", aux_quadruple->op1.value);
            break;
        }

        switch (aux_quadruple->op2.type)
        {
        case general:

            fprintf(output_file, " %-10s,", aux_quadruple->op2.name);

            break;
        case id:
            break;
        case regTemporary:

            fprintf(output_file, " t_%-8d,", aux_quadruple->op2.value);
            break;
        case constant:

            fprintf(output_file, " %-10d,", aux_quadruple->op2.value);
            break;
        case labelk:

            fprintf(output_file, " L_%-8d,", aux_quadruple->op2.value);
            break;
        }

        switch (aux_quadruple->op3.type)
        {
        case general:

            fprintf(output_file, " %-10s )\n", aux_quadruple->op3.name);
            break;
        case id:
            break;
        case regTemporary:

            fprintf(output_file, " t_%-8d )\n", aux_quadruple->op3.value);
            break;
        case constant:

            fprintf(output_file, " %-10d )\n", aux_quadruple->op3.value);
            break;
        case labelk:
            fprintf(output_file, " L_%-8d )\n", aux_quadruple->op3.value);
            break;
        }

        aux_quadruple = aux_quadruple->next;
    }
}

int create_quadruple(TreeNode *tree)
{
    quadruple *aux_quadruple;
    TreeNode *aux;
    int right, left, aux_value;
    int adress = 0;

    if (tree->nodekind == statementK)
    {
        switch (tree->kind.stmt)
        {
        case ifK:

            left = create_quadruple(tree->child[0]);
            insert(beq, "", "", "");
            lista_quad->last->op1.type = regTemporary;
            lista_quad->last->op1.value = left;

            lista_quad->last->op2.type = regTemporary;
            lista_quad->last->op2.value = 0;

            lista_quad->last->op3.type = labelk;

            n_line++;
            lista_quad->last->op3.value = n_line;
            aux_value = n_line;
            traverse(tree->child[1]);

            if (tree->child[2] == NULL)
            {
                insert(label, "", "", "");
                lista_quad->last->op3.type = labelk;
                lista_quad->last->op3.value = aux_value;
                break;
            }
            else
            {
                insert(jump, "", "", "");
                lista_quad->last->op3.type = labelk;

                n_line++;
                lista_quad->last->op3.value = n_line;
                insert(label, "", "", "");
                lista_quad->last->op3.type = labelk;
                lista_quad->last->op3.value = aux_value;

                traverse(tree->child[2]);
                insert(label, "", "", "");
                lista_quad->last->op3.type = labelk;
                lista_quad->last->op3.value = aux_value + 1;
            }
            break;

        case whileK:

            insert(label, "", "", "");
            lista_quad->last->op3.type = labelk;
            n_line++;
            lista_quad->last->op3.value = n_line;
            left = create_quadruple(tree->child[0]);

            insert(beq, "", "", "");
            lista_quad->last->op1.type = regTemporary;
            lista_quad->last->op1.value = left;

            lista_quad->last->op2.type = regTemporary;
            lista_quad->last->op2.value = 0;

            lista_quad->last->op3.type = labelk;
            n_line++;
            lista_quad->last->op3.value = n_line;

            aux_value = n_line;
            traverse(tree->child[1]);

            insert(jump, "", "", "");
            lista_quad->last->op3.type = labelk;
            lista_quad->last->op3.value = aux_value - 1;

            insert(label, "", "", "");
            lista_quad->last->op3.type = labelk;
            lista_quad->last->op3.value = aux_value;

            break;

        case assignK:
            right = create_quadruple(tree->child[1]);

            adress = busca_end(tree->child[0]->attr.name, escopoAux);

            if (tree->child[0]->child[0] != NULL)
            {

                left = create_quadruple(tree->child[0]->child[0]);
                insert(store, tree->child[0]->attr.name, "", "");
                lista_quad->last->op2.type = regTemporary;
                lista_quad->last->op2.value = left;
            }
            else
            {
                insert(store, tree->child[0]->attr.name, "", "");
            }

            lista_quad->last->op1.adress = adress;

            lista_quad->last->op3.type = regTemporary;
            lista_quad->last->op3.value = right;

            break;

        case returnK:
            if (tree->child[0] != NULL)
            {
                left = create_quadruple(tree->child[0]);
                insert(ret, "", "", "");
                lista_quad->last->op1.type = regTemporary;
                lista_quad->last->op1.value = left;
            }
            else
            {
                insert(ret, "", "", "");
            }
            break;

        default:
            break;
        }
    }
    else if (tree->nodekind == expressionK)
    {
        switch (tree->kind.exp)
        {
        case operationK:

            if (tree->child[0]->kind.exp != operationK && tree->child[1]->kind.exp == operationK)
            {

                right = create_quadruple(tree->child[1]);
                left = create_quadruple(tree->child[0]);
            }
            else
            {
                left = create_quadruple(tree->child[0]);
                right = create_quadruple(tree->child[1]);
            }
            insert(tree->attr.op, "", "", "");
            temp_counter++;
            lista_quad->last->op1.type = regTemporary;
            lista_quad->last->op1.value = temp_counter;

            lista_quad->last->op2.type = regTemporary;
            lista_quad->last->op2.value = left;

            lista_quad->last->op3.type = regTemporary;
            lista_quad->last->op3.value = right;
            return temp_counter;
            break;

        case constantK:
            insert(imed, "", "", "");
            lista_quad->last->op1.type = regTemporary;
            temp_counter_func();
            lista_quad->last->op1.value = temp_counter;
            lista_quad->last->op3.type = constant;
            lista_quad->last->op3.value = tree->attr.val;
            return temp_counter;
            break;

        case idK:

            adress = busca_end(tree->attr.name, escopoAux);
            if (tree->child[0] != NULL)
            {
                right = create_quadruple(tree->child[0]);
                insert(load, "", tree->attr.name, "");
                lista_quad->last->op3.type = regTemporary;
                lista_quad->last->op3.value = right;
            }
            else
            {
                insert(load, "", "", tree->attr.name);
            }
            lista_quad->last->op2.adress = adress;
            lista_quad->last->op1.type = regTemporary;
            temp_counter_func();
            lista_quad->last->op1.value = temp_counter;
            return temp_counter;
            break;

        case variableK:

            adress = busca_end(tree->attr.name, escopoAux);
            if (tree->child[0] != NULL)
            {
                if (tree->child[0]->kind.exp == constantK)
                {
                    insert(alloc, tree->attr.name, "", escopoAux);
                    lista_quad->last->op2.type = constant;
                    lista_quad->last->op2.value = tree->child[0]->attr.val;
                }
            }
            else
            {
                insert(alloc, tree->attr.name, "", escopoAux);
                lista_quad->last->op2.type = constant;
                lista_quad->last->op2.value = 1;
            }
            lista_quad->last->op1.adress = adress;
            break;

        case functionK:
            insert(fun, tree->attr.name, "", "");
            escopoAux = tree->attr.name;
            n_line++;
            lista_quad->last->op1.value = n_line;
            traverse(tree->child[0]);
            traverse(tree->child[1]);
            break;

        case activationK:

            aux = tree->child[0];
            while (aux != NULL)
            {
                aux_value = create_quadruple(aux);
                insert(setArg, "", "", "");
                lista_quad->last->op3.type = regTemporary;
                lista_quad->last->op3.value = aux_value;
                aux = aux->sibling;
            }
            insert(call, "", "", tree->attr.name);

            if (getFunType(tree->attr.name) == INTTYPE)
            {
                if (strcmp(tree->attr.name, "output") != 0)
                {
                    temp_counter_func();
                    lista_quad->last->op1.type = regTemporary;
                    lista_quad->last->op1.value = temp_counter;
                }
            }
            return temp_counter;
            break;

        case typeK:
            traverse(tree->child[0]);
            break;

        case paramK:

            adress = busca_end(tree->attr.name, escopoAux);
            insert(arg, tree->attr.name, "", escopoAux);
            lista_quad->last->op1.adress = adress;
            break;

        default:
            break;
        }
    }
    return -1;
}

void traverse(TreeNode *t)
{

    if (t != NULL)
    {
        create_quadruple(t);
        traverse(t->sibling);
        if (t->nodekind == expressionK && t->kind.exp == functionK)
        {
            insert(end, t->attr.name, "", "");
        }
    }
}

void generate_intermediate_code(TreeNode *tree)
{
    lista_quad = (lista *)malloc(sizeof(lista));

    lista_quad->first = NULL;
    lista_quad->last = NULL;
    lista_quad->tamanho = 0;

    insert(nop, "", "", "");
    traverse(tree);
    insert(halt, "", "", "");
    print_quadruple(lista_quad->first);
    generate_assembly();
}
