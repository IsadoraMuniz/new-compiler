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

char *scope_var = "global";
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

    if (quadruple_list->tamanho == 0)
        quadruple_list->first = new;

    else
        quadruple_list->last->next = new;

    quadruple_list->last = new;
    quadruple_list->tamanho++;
}

void temp_counter_func(void)
{
    temp_counter++;

    if (temp_counter == 4)
    {
        temp_counter = 1;
    }
}

void assign_single_op(lista *assembly_list, int operande, int op_type, int op_value)
{
    if(operande == 1) {
        assembly_list->last->op1.type = op_type;
        assembly_list->last->op1.value = op_value;
    }
    else if(operande == 2) {
        assembly_list->last->op2.type = op_type;
        assembly_list->last->op2.value = op_value;
    }
    else {
        assembly_list->last->op3.type = op_type;
        assembly_list->last->op3.value = op_value;
    } 
    
}

void assign_quadruple(lista *quadruple_list, int op_type_1, int value_1,
                      int op_type_2, int value_2, int op_type_3, int value_3)
{

    quadruple_list->last->op1.type = op_type_1;
    quadruple_list->last->op1.value = value_1;

    quadruple_list->last->op2.type = op_type_2;
    quadruple_list->last->op2.value = value_2;

    quadruple_list->last->op3.type = op_type_3;
    quadruple_list->last->op3.value = value_3;
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
            n_line++;
            assign_quadruple(quadruple_list, regTemporary, left, regTemporary, 0, labelk, n_line);
            aux_value = n_line;
            traverse(tree->child[1]);

            if (tree->child[2] == NULL)
            {
                insert(label, "", "", "");
                assign_single_op(quadruple_list, 3, labelk, aux_value);
                break;
            }
            else
            {
                insert(jump, "", "", "");
                n_line++;
                assign_single_op(quadruple_list, 3, labelk, n_line);
               
                insert(label, "", "", "");
                assign_single_op(quadruple_list, 3, labelk, aux_value);

                traverse(tree->child[2]);
                insert(label, "", "", "");
                assign_single_op(quadruple_list, 3, labelk, aux_value+1);
                
            }
            break;

        case whileK:

            insert(label, "", "", "");
            n_line++;
            assign_single_op(quadruple_list, 3, labelk, n_line);
            left = create_quadruple(tree->child[0]);

            insert(beq, "", "", "");
            n_line++;
            assign_quadruple(quadruple_list, regTemporary, left, regTemporary, 0, labelk, n_line);
            aux_value = n_line;
            traverse(tree->child[1]);

            insert(jump, "", "", "");
            assign_single_op(quadruple_list, 3, labelk, aux_value - 1);
           

            insert(label, "", "", "");
            assign_single_op(quadruple_list, 3, labelk, aux_value);

            break;

        case assignK:
            right = create_quadruple(tree->child[1]);

            adress = busca_end(tree->child[0]->attr.name, scope_var);

            if (tree->child[0]->child[0] != NULL)
            {

                left = create_quadruple(tree->child[0]->child[0]);
                insert(store, tree->child[0]->attr.name, "", "");
                assign_single_op(quadruple_list, 2, regTemporary, left);
               
            }
            else
            {
                insert(store, tree->child[0]->attr.name, "", "");
            }

            quadruple_list->last->op1.adress = adress;
            assign_single_op(quadruple_list, 3, regTemporary, right);

            break;

        case returnK:
            if (tree->child[0] != NULL)
            {
                left = create_quadruple(tree->child[0]);
                insert(ret, "", "", "");
                assign_single_op(quadruple_list, 1, regTemporary, left);
            }
            else
                insert(ret, "", "", "");
            
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
            assign_quadruple(quadruple_list, regTemporary, temp_counter, regTemporary, left, regTemporary, right);
            
            return temp_counter;
            break;

        case constantK:
            insert(imed, "", "", "");
            temp_counter_func();
            assign_single_op(quadruple_list, 1, regTemporary, temp_counter);
            assign_single_op(quadruple_list, 3, constant, tree->attr.val);
            
            return temp_counter;
            break;

        case idK:

            adress = busca_end(tree->attr.name, scope_var);
            if (tree->child[0] != NULL)
            {
                right = create_quadruple(tree->child[0]);
                insert(load, "", tree->attr.name, "");
                assign_single_op(quadruple_list, 3, regTemporary, right);
               
            }
            else
                insert(load, "", "", tree->attr.name);
            
            quadruple_list->last->op2.adress = adress;
            temp_counter_func();
            assign_single_op(quadruple_list, 1, regTemporary, temp_counter);
            
            return temp_counter;
            break;

        case variableK:

            adress = busca_end(tree->attr.name, scope_var);
            if (tree->child[0] != NULL)
            {
                if (tree->child[0]->kind.exp == constantK)
                {
                    insert(alloc, tree->attr.name, "", scope_var);
                    assign_single_op(quadruple_list, 2, constant, tree->child[0]->attr.val); 
                }
            }
            else
            {
                insert(alloc, tree->attr.name, "", scope_var);
                assign_single_op(quadruple_list, 2, constant, 1);
            }
            quadruple_list->last->op1.adress = adress;
            break;

        case functionK:
            insert(fun, tree->attr.name, "", "");
            scope_var = tree->attr.name;
            n_line++;
            quadruple_list->last->op1.value = n_line;
            traverse(tree->child[0]);
            traverse(tree->child[1]);
            break;

        case activationK:

            aux = tree->child[0];
            while (aux != NULL)
            {
                aux_value = create_quadruple(aux);
                insert(setArg, "", "", "");
                assign_single_op(quadruple_list, 3, regTemporary, aux_value);
                
                aux = aux->sibling;
            }
            insert(call, "", "", tree->attr.name);

            if (getFunType(tree->attr.name) == INTTYPE)
            {
                if (strcmp(tree->attr.name, "output") != 0)
                {
                    temp_counter_func();
                    assign_single_op(quadruple_list, 1, regTemporary, temp_counter);   
                }
            }
            return temp_counter;
            break;

        case typeK:
            traverse(tree->child[0]);
            break;

        case paramK:
            adress = busca_end(tree->attr.name, scope_var);
            insert(arg, tree->attr.name, "", scope_var);
            quadruple_list->last->op1.adress = adress;
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
    quadruple_list = (lista *)malloc(sizeof(lista));

    quadruple_list->first = NULL;
    quadruple_list->last = NULL;
    quadruple_list->tamanho = 0;

    insert(nop, "", "", "");
    traverse(tree);
    insert(halt, "", "", "");
    print_quadruple(quadruple_list->first);
    generate_assembly();
}
