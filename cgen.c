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

char* escopoAux = "global";

int temp = 0;
int linha = 0;

void temporario(void){
    temp++;

    if(temp == 4){
        temp = 1;
    }
}

void insere(instr instrucao, char* op1, char* op2, char* op3) {

    quadrupla *novo;
    novo=(quadrupla*)malloc(sizeof(quadrupla));
    
    novo->proximo= NULL;
    novo->instrucao = instrucao;
    novo->op1.name = op1;
    novo->op2.name = op2;
    novo->op3.name = op3;

    if(lista_quad->tamanho==0) {

        lista_quad->primeiro=novo;

    } else {

        lista_quad->ultimo->proximo=novo;

    }

    lista_quad->ultimo=novo;
    lista_quad->tamanho++;

}

void imprime(quadrupla *quadrupla_aux) {
    char* aux;

    while(quadrupla_aux!=NULL) {

        switch (quadrupla_aux->instrucao){
            case nop:       aux = "nop";break;
            case halt:      aux = "halt";break;
            case store:     aux = "store";break;
            case fun:       aux = "fun";break;
            case arg:       aux = "arg";break;
            case setArg:    aux = "setArg";break;
            case call:      aux = "call";break;
            case end:       aux = "end";break;
            case vet:       aux = "vet";break;
            case load:      aux = "load";break;
            case alloc:     aux = "alloc";break;
            case ret:      aux = "ret";break;
            case push:      aux = "push";break;
            case pop:       aux = "pop";break;
            case label:     aux = "label";break;
            case IGLIGL:        aux = "==";break;
            case DIF:       aux = "!=";break;
            case MEN:        aux = "<";break;
            case MAI:        aux = ">";break;
            case MEIGL:       aux = "<=";break;
            case MAIGL:       aux = ">=";break;
            case SOM:      aux = "+";break;
            case SUBT:     aux = "-";break;
            case MUL:     aux = "*";break;
            case DIVI:      aux = "/";break;
            case imed:      aux = "imed";break;
            case jump:      aux = "jump";break;
            case beq:      aux = "beq";break;
        }

        printf("( %-6s,", aux);

        switch (quadrupla_aux->op1.type){
            case geral:printf(" %-10s,",quadrupla_aux->op1.name );break;
            case id: break;
            case regTemp:printf(" t_%-8d,", quadrupla_aux->op1.value);break;
            case Const: printf(" %-10d,",quadrupla_aux->op1.value );break;
            case labelk: printf(" L_%-8d,",quadrupla_aux->op1.value );break;
        }

        switch (quadrupla_aux->op2.type){
            case geral:printf(" %-10s,",quadrupla_aux->op2.name );break;
            case id: break;
            case regTemp:printf(" t_%-8d,", quadrupla_aux->op2.value);break;
            case Const: printf(" %-10d,",quadrupla_aux->op2.value );break;
            case labelk: printf(" L_%-8d,",quadrupla_aux->op2.value );break;
        }

        switch (quadrupla_aux->op3.type){
            case geral:printf(" %-10s )\n",quadrupla_aux->op3.name );break;
            case id: break;
            case regTemp:printf(" t_%-8d )\n", quadrupla_aux->op3.value);break;
            case Const: printf(" %-10d )\n",quadrupla_aux->op3.value );break;
            case labelk: printf(" L_%-8d )\n",quadrupla_aux->op3.value );break;
        }

        quadrupla_aux = quadrupla_aux->proximo;

    }
}

int make_quad(TreeNode * tree) {

    quadrupla *quadrupla_aux;
    TreeNode * aux;
    int direita, esquerda, auxiliar;
    int endr = 0;

    if (tree->nodekind==statementK) { 
        switch (tree->kind.stmt) {
        case ifK:

            esquerda = make_quad(tree->child[0]);

            insere(beq,"","","");
            lista_quad->ultimo->op1.type = regTemp;
            lista_quad->ultimo->op1.value = esquerda;
            lista_quad->ultimo->op3.type = labelk;
            linha++;
            lista_quad->ultimo->op3.value = linha;

            auxiliar = linha;
            percorre(tree->child[1]);

            if(tree->child[2] == NULL){
                insere(label,"", "", "");
                lista_quad->ultimo->op3.type = labelk;
                lista_quad->ultimo->op3.value = auxiliar;
                break;
            }else{
                insere(jump, "", "", "");
                lista_quad->ultimo->op3.type = labelk;
                linha++;
                lista_quad->ultimo->op3.value = linha;

                insere(label,"", "", "");
                lista_quad->ultimo->op3.type = labelk;
                lista_quad->ultimo->op3.value = auxiliar;

                percorre(tree->child[2]);

                insere(label,"", "", "");
                lista_quad->ultimo->op3.type = labelk;
                lista_quad->ultimo->op3.value = auxiliar+1;

            }
            break;


        case whileK:
            linha++;

            insere(label,"", "", "");
            lista_quad->ultimo->op3.type = labelk;
            lista_quad->ultimo->op3.value = linha;
            esquerda = make_quad(tree->child[0]);

            insere(beq,"","","");
            lista_quad->ultimo->op1.type = regTemp;
            lista_quad->ultimo->op1.value = esquerda;

            lista_quad->ultimo->op2.type = regTemp;
            lista_quad->ultimo->op2.value = 0;

            lista_quad->ultimo->op3.type = labelk;
            linha++;
            lista_quad->ultimo->op3.value = linha;

            auxiliar = linha;
            percorre(tree->child[1]);

            insere(jump, "", "", "");
            lista_quad->ultimo->op3.type = labelk;
            lista_quad->ultimo->op3.value = auxiliar-1;

            insere(label,"", "", "");
            lista_quad->ultimo->op3.type = labelk;
            lista_quad->ultimo->op3.value = auxiliar;
            
            break;


        case assignK:            
            direita = make_quad(tree->child[1]);

            endr = busca_end(tree->child[0]->attr.name, escopoAux);
            
            
            if(tree->child[0]->child[0] != NULL){

                esquerda = make_quad(tree->child[0]->child[0]);
                insere(store, tree->child[0]->attr.name, "", "");
                lista_quad->ultimo->op2.type = regTemp;
                lista_quad->ultimo->op2.value = esquerda;

            }else{
                insere(store, tree->child[0]->attr.name, "", "");
            }
            
            lista_quad->ultimo->op1.endereco = endr;

            lista_quad->ultimo->op3.type = regTemp;
            lista_quad->ultimo->op3.value = direita;

            
            break; 


        case returnK:
            if(tree->child[0] != NULL){
                esquerda = make_quad(tree->child[0]);
                insere(ret, "", "", "");
                lista_quad->ultimo->op1.type = regTemp;
                lista_quad->ultimo->op1.value = esquerda;
            }
            else{
                insere(ret, "", "", "");
            }
            break;

        default:
            break;
        }
    } else if (tree->nodekind==expressionK) {
        switch (tree->kind.exp) {
        case operationK:

            if(tree->child[0]->kind.exp != operationK && tree->child[1]->kind.exp == operationK){

                direita = make_quad(tree->child[1]);
                esquerda = make_quad(tree->child[0]);
                
            }else{
                esquerda = make_quad(tree->child[0]);
                direita = make_quad(tree->child[1]);

            }
                insere(tree->attr.op, "", "", "");
                temp++;
                lista_quad->ultimo->op1.type = regTemp;
                lista_quad->ultimo->op1.value = temp;

                lista_quad->ultimo->op2.type = regTemp;
                lista_quad->ultimo->op2.value = esquerda;

                
                lista_quad->ultimo->op3.type = regTemp;
                lista_quad->ultimo->op3.value = direita;
                return temp;            
            break;

        case constantK:
            insere(imed, "", "", "");
            lista_quad->ultimo->op1.type = regTemp;
            temporario();
            lista_quad->ultimo->op1.value = temp; 
            lista_quad->ultimo->op3.type = Const;
            lista_quad->ultimo->op3.value = tree->attr.val;
            return temp;
            break;


        case idK:

                endr = busca_end(tree->attr.name, escopoAux);
                if(tree->child[0] != NULL){
                    direita = make_quad(tree->child[0]);
                    insere(vet, "", tree->attr.name, "");
                    lista_quad->ultimo->op3.type = regTemp;
                    lista_quad->ultimo->op3.value = direita;

                }else{
                    insere(load, "", "", tree->attr.name);
                }
                lista_quad->ultimo->op1.type = regTemp;
                temporario();
                lista_quad->ultimo->op1.value = temp; 
                return temp;
                break;


        case variableK:

            endr = busca_end(tree->attr.name, escopoAux);
            if(tree->child[0] != NULL){
                if(tree->child[0]->kind.exp == constantK){
                    insere(alloc, tree->attr.name, "", escopoAux);
                    lista_quad->ultimo->op2.type = Const;
                    lista_quad->ultimo->op2.value = tree->child[0]->attr.val;
                }

            }else{
                insere(alloc, tree->attr.name, "", escopoAux);
                lista_quad->ultimo->op2.type = Const;
                lista_quad->ultimo->op2.value = 1;
            }
            lista_quad->ultimo->op1.endereco = endr;
            break;


        case functionK:
            insere(fun, tree->attr.name, "", "");
            escopoAux = tree->attr.name;
            linha++;
            lista_quad->ultimo->op1.value = linha;
            percorre(tree->child[0]);
            percorre(tree->child[1]);
            break;


        case activationK:
            
            aux = tree->child[0];
            while(aux != NULL){

                auxiliar = make_quad(aux);
                insere(setArg, "", "", "");
                lista_quad->ultimo->op3.type = regTemp;
                lista_quad->ultimo->op3.value = auxiliar;
                
                aux = aux->sibling;
            }
            insere(call, "", "",tree->attr.name);

            if(getFunType(tree->attr.name) == INTTYPE){

                if(strcmp(tree->attr.name,"output") != 0){
                    temporario();
                    lista_quad->ultimo->op1.type = regTemp;
                    lista_quad->ultimo->op1.value = temp;
                }
            }

            return temp;
            break;


        case typeK:
            percorre(tree->child[0]);
            break;


        case paramK:
            endr = busca_end(tree->attr.name, escopoAux);
           

            insere(arg, tree->attr.name, "", escopoAux);

            lista_quad->ultimo->op1.endereco = endr;
            break;


        default:
            break;
        }
    }
    return -1;

}

void percorre( TreeNode * t) {
    int i, x;
    
    if (t != NULL) {

        x = make_quad(t);

        percorre(t->sibling);
        if(t->nodekind==expressionK && t->kind.exp == functionK){
            insere(end, t->attr.name, "", "");
        }
        
    }
}

void gen_quad(TreeNode * tree) {

    lista_quad=(lista*)malloc(sizeof(lista));

    lista_quad->primeiro=NULL;
    lista_quad->ultimo=NULL;
    lista_quad->tamanho=0;

    insere(nop, "", "", "");

    percorre(tree);

    insere(halt, "", "", "");

    imprime(lista_quad->primeiro);

    assmb_gen();

}

