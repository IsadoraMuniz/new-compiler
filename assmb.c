/****************************************************/
/*              Gerador de codigo assembly          */
/*                 Isadora Muniz                    */
/****************************************************/

#include "globals.h"
#include "symtab.h"
#include "analyze.h"
#include "cgen.h"
#include "assmb.h"
#include "bin.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//Insere uma nova instrucao na lista
void insere_ASSMB(OPCODE opcode) {

    INSTRU *novo;
    novo=(INSTRU*)malloc(sizeof(INSTRU));
    
    novo->proximo= NULL;
    novo->opcode = opcode;

    if(listaAsmb->tamanho==0) {

        listaAsmb->primeiro=novo;

    } else {

        listaAsmb->ultimo->proximo=novo;

    }

    listaAsmb->ultimo=novo;
    listaAsmb->tamanho++;

}

//Funcao que imprime a lista de instrucoes assembly
void imprime_ASSMB(INSTRU *Noaux){

    char *aux;
    bool label_flag = true;
    bool fun_flag = true;

    printf("\n\n\n             CODIGO ASSEMBLY GERADO:\n\n");

    while(Noaux != NULL) {
        
        switch (Noaux->opcode){
            case NOP:       aux = "nop";break;
            case HALT:      aux = "halt";break;
            case SW:        aux = "sw";break;
            case LW:        aux = "lw";break;
            case LABEL:     
                aux = "";
                if(Noaux->op1.type == funck){
                    fun_flag = true;
                }
                label_flag = true;
                
                break;
            case BEQ:       aux = "beq";break;
            case SET:       aux = "set";break;
            case SDT:       aux = "sdt";break;
            case SLT:       aux = "slt";break;
            case SLE:       aux = "sle";break;
            case SGT:       aux = "sgt";break;
            case SGE:       aux = "sge";break;
            case BNE:       aux = "bne";break;
            case JUMP:      aux = "jump ";break;
            case LI:        aux = "li";break;
            case MOVE:      aux = "move";break;
            case ADD:       aux = "add";break;
            case SUB:       aux = "sub";break;
            case MULT:      aux = "mult";break;
            case DIV:       aux = "div";break;
            case IN:        aux = "in";break;
            case JAL:       aux = "jal";break;
            case JR:        aux = "jr";break;
            case OUT:       aux = "out";break;
            case WAIT:      aux = "wait";break;
    
        }

        if(!fun_flag){
            printf("        ");
        }

        if(!label_flag){
            printf("    ");
        }

        printf("%s", aux);

        switch (Noaux->op1.type){
            case regTemp:printf(" $r%d", Noaux->op1.value);break;
            case Const: printf(" %d",Noaux->op1.value );break;
            case labelk: 
                printf("L%d:",Noaux->op1.value );
                label_flag = false;
                break;
            case funck: 
                printf("%s:\n",Noaux->op1.name);
                fun_flag = false;
                break;
        }

        switch (Noaux->op2.type){

            case regTemp:printf(", $r%d", Noaux->op2.value);break;
            case Const: printf(", %d",Noaux->op2.value );break;
            case funck: printf(" %s",Noaux->op2.name);break;
        }

        switch (Noaux->op3.type){
            case regTemp:printf(", $r%d", Noaux->op3.value);break;
            case Const: printf(", %d",Noaux->op3.value );break;
            case labelk: printf(", L%d",Noaux->op3.value );break;
        }

        printf("\n");
        Noaux = Noaux->proximo;
    }
}

//Funcao que percorre lista de quadruplas e 
//insere na lista de instrucoes assembly
void percorre_lista (void){

	quadruple *aux = lista_quad->primeiro;

    int argmt = 21;
    int i;


	while(aux != NULL){
    //printf("%d\n", aux->instrucao);

		switch (aux->instruction_name){
      		case nop:
                insere_ASSMB(NOP);
      			break;
                  
            case halt:
                insere_ASSMB(HALT);
                break;

            case store:

                insere_ASSMB(LI);
                listaAsmb->ultimo->op1.type = regTemp;
                listaAsmb->ultimo->op1.value = 27;

                listaAsmb->ultimo->op2.type = Const;
                listaAsmb->ultimo->op2.value = aux->op1.adress;

                if(aux->op2.type == regTemp){

                    insere_ASSMB(ADD);
                    listaAsmb->ultimo->op1.type = regTemp;
                    listaAsmb->ultimo->op1.value = 27;

                    listaAsmb->ultimo->op2.type = regTemp;
                    listaAsmb->ultimo->op2.value = 27;

                    listaAsmb->ultimo->op3.type = regTemp;
                    listaAsmb->ultimo->op3.value = aux->op2.value;

                }

                insere_ASSMB(SW);
                listaAsmb->ultimo->op1.type = regTemp;
                listaAsmb->ultimo->op1.value = 27;

                listaAsmb->ultimo->op2.type = regTemp;
                listaAsmb->ultimo->op2.value = aux->op3.value;


                break;

            case load:
                insere_ASSMB(LI);
                listaAsmb->ultimo->op1.type = regTemp;
                listaAsmb->ultimo->op1.value = 27;

                listaAsmb->ultimo->op2.type = Const;
                listaAsmb->ultimo->op2.value = aux->op2.adress;

                if(aux->op3.type == regTemp){

                    insere_ASSMB(ADD);
                    listaAsmb->ultimo->op1.type = regTemp;
                    listaAsmb->ultimo->op1.value = 27;

                    listaAsmb->ultimo->op2.type = regTemp;
                    listaAsmb->ultimo->op2.value = 27;

                    listaAsmb->ultimo->op3.type = regTemp;
                    listaAsmb->ultimo->op3.value = aux->op3.value;

                }

                insere_ASSMB(LW);
                listaAsmb->ultimo->op1.type = regTemp;
                listaAsmb->ultimo->op1.value = aux->op1.value;

                listaAsmb->ultimo->op2.type = regTemp;
                listaAsmb->ultimo->op2.value = 27;
                break;


            case fun: 
                insere_ASSMB(LABEL);
                listaAsmb->ultimo->op1.type = funck;
                listaAsmb->ultimo->op1.name = aux->op1.name;
                listaAsmb->ultimo->op1.value = aux->op1.value;
                break;

            case arg:
                i = 22;

                while(i<=argmt){
                    
                    insere_ASSMB(SW);
                    listaAsmb->ultimo->op1.type = regTemp;
                    listaAsmb->ultimo->op1.value = i;

                    listaAsmb->ultimo->op2.type = regTemp;
                    listaAsmb->ultimo->op2.value = aux->op3.value;

                    i++;
                } 

                argmt = 21;

                break;

            case setArg:
                
                argmt++;

                insere_ASSMB(MOVE);
                listaAsmb->ultimo->op1.type = regTemp;
                listaAsmb->ultimo->op1.value = argmt;

                listaAsmb->ultimo->op2.type = regTemp;
                listaAsmb->ultimo->op2.value = aux->op3.value;

                break;

            case call:             
                if(strcmp(aux->op3.name, "output") == 0){


                    insere_ASSMB(OUT);
                    listaAsmb->ultimo->op1.type = regTemp;
                    listaAsmb->ultimo->op1.value = 22;


                }else if(strcmp(aux->op3.name, "input") == 0){
                    
                    insere_ASSMB(IN);
                    

                    insere_ASSMB(MOVE);
                    listaAsmb->ultimo->op1.type = regTemp;
                    listaAsmb->ultimo->op1.value = aux->op1.value;
                    listaAsmb->ultimo->op2.type = regTemp;
                    listaAsmb->ultimo->op2.value = 30;
                }
                else{
                    insere_ASSMB(JAL);
                    listaAsmb->ultimo->op2.type = funck;
                    listaAsmb->ultimo->op2.name = aux->op3.name;
                    listaAsmb->ultimo->op1.value = aux->op1.value;
                    if(aux->op1.type == regTemp){
                        insere_ASSMB(MOVE);
                        listaAsmb->ultimo->op1.type = regTemp;
                        listaAsmb->ultimo->op1.value = aux->op1.value;
                        listaAsmb->ultimo->op2.type = regTemp;
                        listaAsmb->ultimo->op2.value = 28;
                    }
                }
                break;

            case end: 

                if(strcmp(aux->op1.name, "main") != 0){
                    insere_ASSMB(JR);
                    listaAsmb->ultimo->op1.type = regTemp;
                    listaAsmb->ultimo->op1.value = 31;  

                } 
                 
                break;

            case alloc:  
                /*
                    Sem funcionalidade

                */

                break;

            case ret: 

                insere_ASSMB(MOVE);
                listaAsmb->ultimo->op1.type = regTemp;
                listaAsmb->ultimo->op1.value = 28;

                listaAsmb->ultimo->op2.type = regTemp;
                listaAsmb->ultimo->op2.value = aux->op1.value;

                insere_ASSMB(JR);
                listaAsmb->ultimo->op1.type = regTemp;
                listaAsmb->ultimo->op1.value = 31;

                break;

            case label: 
                
                insere_ASSMB(LABEL);
                listaAsmb->ultimo->op1.type = labelk;
                listaAsmb->ultimo->op1.value = aux->op3.value;
                break;

            case IGLIGL:    
                insere_ASSMB(SET);

                listaAsmb->ultimo->op1.type = regTemp;
                listaAsmb->ultimo->op1.value = aux->op1.value;

                listaAsmb->ultimo->op2.type = regTemp;
                listaAsmb->ultimo->op2.value = aux->op2.value;

                listaAsmb->ultimo->op3.type = regTemp;
                listaAsmb->ultimo->op3.value = aux->op3.value;   
                break;


            case DIF:     
                insere_ASSMB(SDT);

                listaAsmb->ultimo->op1.type = regTemp;
                listaAsmb->ultimo->op1.value = aux->op1.value;

                listaAsmb->ultimo->op2.type = regTemp;
                listaAsmb->ultimo->op2.value = aux->op2.value;

                listaAsmb->ultimo->op3.type = regTemp;
                listaAsmb->ultimo->op3.value = aux->op3.value;  
                break;

            case MEN:     
                insere_ASSMB(SLT);

                listaAsmb->ultimo->op1.type = regTemp;
                listaAsmb->ultimo->op1.value = aux->op1.value;

                listaAsmb->ultimo->op2.type = regTemp;
                listaAsmb->ultimo->op2.value = aux->op2.value;

                listaAsmb->ultimo->op3.type = regTemp;
                listaAsmb->ultimo->op3.value = aux->op3.value;  
                break;

            case MAI:     
                insere_ASSMB(SGT);

                listaAsmb->ultimo->op1.type = regTemp;
                listaAsmb->ultimo->op1.value = aux->op1.value;

                listaAsmb->ultimo->op2.type = regTemp;
                listaAsmb->ultimo->op2.value = aux->op2.value;

                listaAsmb->ultimo->op3.type = regTemp;
                listaAsmb->ultimo->op3.value = aux->op3.value;  
                break;

            case MAIGL:     
                insere_ASSMB(SGE);

                listaAsmb->ultimo->op1.type = regTemp;
                listaAsmb->ultimo->op1.value = aux->op1.value;

                listaAsmb->ultimo->op2.type = regTemp;
                listaAsmb->ultimo->op2.value = aux->op2.value;

                listaAsmb->ultimo->op3.type = regTemp;
                listaAsmb->ultimo->op3.value = aux->op3.value;  
                break;

            case MEIGL:     
                insere_ASSMB(SLE);

                listaAsmb->ultimo->op1.type = regTemp;
                listaAsmb->ultimo->op1.value = aux->op1.value;

                listaAsmb->ultimo->op2.type = regTemp;
                listaAsmb->ultimo->op2.value = aux->op2.value;

                listaAsmb->ultimo->op3.type = regTemp;
                listaAsmb->ultimo->op3.value = aux->op3.value;  
                break;


            case SOM:    
                insere_ASSMB(ADD);

                listaAsmb->ultimo->op1.type = regTemp;
                listaAsmb->ultimo->op1.value = aux->op1.value;

                listaAsmb->ultimo->op2.type = regTemp;
                listaAsmb->ultimo->op2.value = aux->op2.value;

                listaAsmb->ultimo->op3.type = regTemp;
                listaAsmb->ultimo->op3.value = aux->op3.value;

                break;

            case SUBT:   
                insere_ASSMB(SUB);

                listaAsmb->ultimo->op1.type = regTemp;
                listaAsmb->ultimo->op1.value = aux->op1.value;

                listaAsmb->ultimo->op2.type = regTemp;
                listaAsmb->ultimo->op2.value = aux->op2.value;

                listaAsmb->ultimo->op3.type = regTemp;
                listaAsmb->ultimo->op3.value = aux->op3.value; 
                
                break;

            case MUL:  
                insere_ASSMB(MULT);

                listaAsmb->ultimo->op1.type = regTemp;
                listaAsmb->ultimo->op1.value = aux->op1.value;

                listaAsmb->ultimo->op2.type = regTemp;
                listaAsmb->ultimo->op2.value = aux->op2.value;

                listaAsmb->ultimo->op3.type = regTemp;
                listaAsmb->ultimo->op3.value = aux->op3.value; 
                  
                break;

            case DIVI:  
                insere_ASSMB(DIV);

                listaAsmb->ultimo->op1.type = regTemp;
                listaAsmb->ultimo->op1.value = aux->op1.value;

                listaAsmb->ultimo->op2.type = regTemp;
                listaAsmb->ultimo->op2.value = aux->op2.value;

                listaAsmb->ultimo->op3.type = regTemp;
                listaAsmb->ultimo->op3.value = aux->op3.value;  
                 
                break;

            case imed:    
                insere_ASSMB(LI);

                listaAsmb->ultimo->op1.type = regTemp;
                listaAsmb->ultimo->op1.value = aux->op1.value;

                listaAsmb->ultimo->op3.type = Const;
                listaAsmb->ultimo->op3.value = aux->op3.value;
                break;

            case jump:   
                insere_ASSMB(JUMP);
                listaAsmb->ultimo->op1.type = labelk;
                listaAsmb->ultimo->op1.value = aux->op3.value;
                break;

            case beq:    
                insere_ASSMB(BEQ);
                listaAsmb->ultimo->op1.type = regTemp;
                listaAsmb->ultimo->op1.value = aux->op1.value;

                listaAsmb->ultimo->op2.type = regTemp;
                listaAsmb->ultimo->op2.value = aux->op2.value;

                listaAsmb->ultimo->op3.type = labelk;
                listaAsmb->ultimo->op3.value = aux->op3.value;
                break;

            case bne:    
                insere_ASSMB(BNE);
                listaAsmb->ultimo->op1.type = regTemp;
                listaAsmb->ultimo->op1.value = aux->op1.value;

                listaAsmb->ultimo->op2.type = regTemp;
                listaAsmb->ultimo->op2.value = aux->op2.value;

                listaAsmb->ultimo->op3.type = labelk;
                listaAsmb->ultimo->op3.value = aux->op3.value;
                break;

            default:
                printf("no nao achado\n");
                break;


		}

		aux = aux->proximo;
	}

}

//Monta a lista de instrucoes assembly e
//chama a funcao que monta a memoria de instrucoes
void assmb_gen(void){

    
    listaAsmb = (lista_ASSMB*)malloc(sizeof(lista_ASSMB));

    listaAsmb->primeiro = NULL;
    listaAsmb->ultimo = NULL;
    listaAsmb->tamanho = 0;

	percorre_lista();

	imprime_ASSMB(listaAsmb->primeiro);

    make_bin(listaAsmb->primeiro);
}


