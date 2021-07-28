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

//insert a new assembly instruction to the list
void insert_assembly_code(OPCODE opcode) {

    instruction_cell *novo;
    novo=(instruction_cell*)malloc(sizeof(instruction_cell));
    
    novo->proximo= NULL;
    novo->opcode = opcode;

    if(assembly_list->tamanho==0) {

        assembly_list->primeiro=novo;

    } else {

        assembly_list->ultimo->proximo=novo;

    }

    assembly_list->ultimo=novo;
    assembly_list->tamanho++;

}

//Funcao que imprime a lista de instrucoes assembly
void print_assembly_code(instruction_cell *Noaux){

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


void percorre_lista (void){

	quadruple *aux = lista_quad->primeiro;

    int argmt = 21;
    int i;


	while(aux != NULL){

		switch (aux->instruction_name){
      		case nop:
                insert_assembly_code(NOP);
      			break;
                  
            case halt:
                insert_assembly_code(HALT);
                break;

            case store:

                insert_assembly_code(LI);
                assembly_list->ultimo->op1.type = regTemp;
                assembly_list->ultimo->op1.value = 27;

                assembly_list->ultimo->op2.type = Const;
                assembly_list->ultimo->op2.value = aux->op1.adress;

                if(aux->op2.type == regTemp){

                    insert_assembly_code(ADD);
                    assembly_list->ultimo->op1.type = regTemp;
                    assembly_list->ultimo->op1.value = 27;

                    assembly_list->ultimo->op2.type = regTemp;
                    assembly_list->ultimo->op2.value = 27;

                    assembly_list->ultimo->op3.type = regTemp;
                    assembly_list->ultimo->op3.value = aux->op2.value;

                }

                insert_assembly_code(SW);
                assembly_list->ultimo->op1.type = regTemp;
                assembly_list->ultimo->op1.value = 27;

                assembly_list->ultimo->op2.type = regTemp;
                assembly_list->ultimo->op2.value = aux->op3.value;


                break;

            case load:
                insert_assembly_code(LI);
                assembly_list->ultimo->op1.type = regTemp;
                assembly_list->ultimo->op1.value = 27;

                assembly_list->ultimo->op2.type = Const;
                assembly_list->ultimo->op2.value = aux->op2.adress;

                if(aux->op3.type == regTemp){

                    insert_assembly_code(ADD);
                    assembly_list->ultimo->op1.type = regTemp;
                    assembly_list->ultimo->op1.value = 27;

                    assembly_list->ultimo->op2.type = regTemp;
                    assembly_list->ultimo->op2.value = 27;

                    assembly_list->ultimo->op3.type = regTemp;
                    assembly_list->ultimo->op3.value = aux->op3.value;

                }

                insert_assembly_code(LW);
                assembly_list->ultimo->op1.type = regTemp;
                assembly_list->ultimo->op1.value = aux->op1.value;

                assembly_list->ultimo->op2.type = regTemp;
                assembly_list->ultimo->op2.value = 27;
                break;


            case fun: 
                insert_assembly_code(LABEL);
                assembly_list->ultimo->op1.type = funck;
                assembly_list->ultimo->op1.name = aux->op1.name;
                assembly_list->ultimo->op1.value = aux->op1.value;
                break;

            case arg:
                i = 22;

                while(i<=argmt){
                    
                    insert_assembly_code(SW);
                    assembly_list->ultimo->op1.type = regTemp;
                    assembly_list->ultimo->op1.value = i;

                    assembly_list->ultimo->op2.type = regTemp;
                    assembly_list->ultimo->op2.value = aux->op3.value;

                    i++;
                } 

                argmt = 21;

                break;

            case setArg:
                
                argmt++;

                insert_assembly_code(MOVE);
                assembly_list->ultimo->op1.type = regTemp;
                assembly_list->ultimo->op1.value = argmt;

                assembly_list->ultimo->op2.type = regTemp;
                assembly_list->ultimo->op2.value = aux->op3.value;

                break;

            case call:             
                if(strcmp(aux->op3.name, "output") == 0){


                    insert_assembly_code(OUT);
                    assembly_list->ultimo->op1.type = regTemp;
                    assembly_list->ultimo->op1.value = 22;


                }else if(strcmp(aux->op3.name, "input") == 0){
                    
                    insert_assembly_code(IN);
                    

                    insert_assembly_code(MOVE);
                    assembly_list->ultimo->op1.type = regTemp;
                    assembly_list->ultimo->op1.value = aux->op1.value;
                    assembly_list->ultimo->op2.type = regTemp;
                    assembly_list->ultimo->op2.value = 30;
                }
                else{
                    insert_assembly_code(JAL);
                    assembly_list->ultimo->op2.type = funck;
                    assembly_list->ultimo->op2.name = aux->op3.name;
                    assembly_list->ultimo->op1.value = aux->op1.value;
                    if(aux->op1.type == regTemp){
                        insert_assembly_code(MOVE);
                        assembly_list->ultimo->op1.type = regTemp;
                        assembly_list->ultimo->op1.value = aux->op1.value;
                        assembly_list->ultimo->op2.type = regTemp;
                        assembly_list->ultimo->op2.value = 28;
                    }
                }
                break;

            case end: 

                if(strcmp(aux->op1.name, "main") != 0){
                    insert_assembly_code(JR);
                    assembly_list->ultimo->op1.type = regTemp;
                    assembly_list->ultimo->op1.value = 31;  

                } 
                 
                break;

            case alloc:  
                /*
                    Sem funcionalidade

                */

                break;

            case ret: 

                insert_assembly_code(MOVE);
                assembly_list->ultimo->op1.type = regTemp;
                assembly_list->ultimo->op1.value = 28;

                assembly_list->ultimo->op2.type = regTemp;
                assembly_list->ultimo->op2.value = aux->op1.value;

                insert_assembly_code(JR);
                assembly_list->ultimo->op1.type = regTemp;
                assembly_list->ultimo->op1.value = 31;

                break;

            case label: 
                
                insert_assembly_code(LABEL);
                assembly_list->ultimo->op1.type = labelk;
                assembly_list->ultimo->op1.value = aux->op3.value;
                break;

            case IGLIGL:    
                insert_assembly_code(SET);

                assembly_list->ultimo->op1.type = regTemp;
                assembly_list->ultimo->op1.value = aux->op1.value;

                assembly_list->ultimo->op2.type = regTemp;
                assembly_list->ultimo->op2.value = aux->op2.value;

                assembly_list->ultimo->op3.type = regTemp;
                assembly_list->ultimo->op3.value = aux->op3.value;   
                break;


            case DIF:     
                insert_assembly_code(SDT);

                assembly_list->ultimo->op1.type = regTemp;
                assembly_list->ultimo->op1.value = aux->op1.value;

                assembly_list->ultimo->op2.type = regTemp;
                assembly_list->ultimo->op2.value = aux->op2.value;

                assembly_list->ultimo->op3.type = regTemp;
                assembly_list->ultimo->op3.value = aux->op3.value;  
                break;

            case MEN:     
                insert_assembly_code(SLT);

                assembly_list->ultimo->op1.type = regTemp;
                assembly_list->ultimo->op1.value = aux->op1.value;

                assembly_list->ultimo->op2.type = regTemp;
                assembly_list->ultimo->op2.value = aux->op2.value;

                assembly_list->ultimo->op3.type = regTemp;
                assembly_list->ultimo->op3.value = aux->op3.value;  
                break;

            case MAI:     
                insert_assembly_code(SGT);

                assembly_list->ultimo->op1.type = regTemp;
                assembly_list->ultimo->op1.value = aux->op1.value;

                assembly_list->ultimo->op2.type = regTemp;
                assembly_list->ultimo->op2.value = aux->op2.value;

                assembly_list->ultimo->op3.type = regTemp;
                assembly_list->ultimo->op3.value = aux->op3.value;  
                break;

            case MAIGL:     
                insert_assembly_code(SGE);

                assembly_list->ultimo->op1.type = regTemp;
                assembly_list->ultimo->op1.value = aux->op1.value;

                assembly_list->ultimo->op2.type = regTemp;
                assembly_list->ultimo->op2.value = aux->op2.value;

                assembly_list->ultimo->op3.type = regTemp;
                assembly_list->ultimo->op3.value = aux->op3.value;  
                break;

            case MEIGL:     
                insert_assembly_code(SLE);

                assembly_list->ultimo->op1.type = regTemp;
                assembly_list->ultimo->op1.value = aux->op1.value;

                assembly_list->ultimo->op2.type = regTemp;
                assembly_list->ultimo->op2.value = aux->op2.value;

                assembly_list->ultimo->op3.type = regTemp;
                assembly_list->ultimo->op3.value = aux->op3.value;  
                break;


            case SOM:    
                insert_assembly_code(ADD);

                assembly_list->ultimo->op1.type = regTemp;
                assembly_list->ultimo->op1.value = aux->op1.value;

                assembly_list->ultimo->op2.type = regTemp;
                assembly_list->ultimo->op2.value = aux->op2.value;

                assembly_list->ultimo->op3.type = regTemp;
                assembly_list->ultimo->op3.value = aux->op3.value;

                break;

            case SUBT:   
                insert_assembly_code(SUB);

                assembly_list->ultimo->op1.type = regTemp;
                assembly_list->ultimo->op1.value = aux->op1.value;

                assembly_list->ultimo->op2.type = regTemp;
                assembly_list->ultimo->op2.value = aux->op2.value;

                assembly_list->ultimo->op3.type = regTemp;
                assembly_list->ultimo->op3.value = aux->op3.value; 
                
                break;

            case MUL:  
                insert_assembly_code(MULT);

                assembly_list->ultimo->op1.type = regTemp;
                assembly_list->ultimo->op1.value = aux->op1.value;

                assembly_list->ultimo->op2.type = regTemp;
                assembly_list->ultimo->op2.value = aux->op2.value;

                assembly_list->ultimo->op3.type = regTemp;
                assembly_list->ultimo->op3.value = aux->op3.value; 
                  
                break;

            case DIVI:  
                insert_assembly_code(DIV);

                assembly_list->ultimo->op1.type = regTemp;
                assembly_list->ultimo->op1.value = aux->op1.value;

                assembly_list->ultimo->op2.type = regTemp;
                assembly_list->ultimo->op2.value = aux->op2.value;

                assembly_list->ultimo->op3.type = regTemp;
                assembly_list->ultimo->op3.value = aux->op3.value;  
                 
                break;

            case imed:    
                insert_assembly_code(LI);

                assembly_list->ultimo->op1.type = regTemp;
                assembly_list->ultimo->op1.value = aux->op1.value;

                assembly_list->ultimo->op3.type = Const;
                assembly_list->ultimo->op3.value = aux->op3.value;
                break;

            case jump:   
                insert_assembly_code(JUMP);
                assembly_list->ultimo->op1.type = labelk;
                assembly_list->ultimo->op1.value = aux->op3.value;
                break;

            case beq:    
                insert_assembly_code(BEQ);
                assembly_list->ultimo->op1.type = regTemp;
                assembly_list->ultimo->op1.value = aux->op1.value;

                assembly_list->ultimo->op2.type = regTemp;
                assembly_list->ultimo->op2.value = aux->op2.value;

                assembly_list->ultimo->op3.type = labelk;
                assembly_list->ultimo->op3.value = aux->op3.value;
                break;

            case bne:    
                insert_assembly_code(BNE);
                assembly_list->ultimo->op1.type = regTemp;
                assembly_list->ultimo->op1.value = aux->op1.value;

                assembly_list->ultimo->op2.type = regTemp;
                assembly_list->ultimo->op2.value = aux->op2.value;

                assembly_list->ultimo->op3.type = labelk;
                assembly_list->ultimo->op3.value = aux->op3.value;
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

    
    assembly_list = (TList_assembly*)malloc(sizeof(TList_assembly));

    assembly_list->primeiro = NULL;
    assembly_list->ultimo = NULL;
    assembly_list->tamanho = 0;

	percorre_lista();

	print_assembly_code(assembly_list->primeiro);

    make_bin(assembly_list->primeiro);
}

