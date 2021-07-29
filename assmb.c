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

    instruction_cell *new;
    new=(instruction_cell*)malloc(sizeof(instruction_cell));
    
    new->next= NULL;
    new->opcode = opcode;

    if(assembly_list->tamanho==0) {

        assembly_list->first=new;

    } 
    else {

        assembly_list->last->next=new;

    }

    assembly_list->last=new;
    assembly_list->tamanho++;

}

void assign_op_1(TList_assembly* assembly_list, int op_type_1, int op_value_1) {

    assembly_list->last->op1.type = op_type_1;
    assembly_list->last->op1.value = op_value_1;
}

void assign_operation(TList_assembly* assembly_list, int op_type_1, int value_1,
                        int op_type_2, int value_2, int op_type_3, int value_3) {

    assembly_list->last->op1.type = op_type_1;
    assembly_list->last->op1.value = value_1;

    assembly_list->last->op2.type = op_type_2;
    assembly_list->last->op2.value = value_2;

    assembly_list->last->op3.type = op_type_3;
    assembly_list->last->op3.value = value_3;

}


//print the list of assembly code
void print_assembly_code(instruction_cell *Noaux){

    char *aux;
    bool label_flag = true;
    bool fun_flag = true;

    printf("\n\n\n             CODIGO ASSEMBLY GERADO:\n\n");

    while(Noaux != NULL) {
        
        switch (Noaux->opcode){
            case NOP:       aux = "nop";    break;
            case HALT:      aux = "halt";   break;
            case SW:        aux = "sw";     break;
            case LW:        aux = "lw";     break;
            case LABEL:     
                aux = "";
                if(Noaux->op1.type == funck){
                    fun_flag = true;
                }
                label_flag = true;
                
                break;
            case BEQ:       aux = "beq";    break;
            case SET:       aux = "set";    break;
            case SDT:       aux = "sdt";    break;
            case SLT:       aux = "slt";    break;
            case SLE:       aux = "sle";    break;
            case SGT:       aux = "sgt";    break;
            case SGE:       aux = "sge";    break;
            case BNE:       aux = "bne";    break;
            case JUMP:      aux = "jump ";  break;
            case LI:        aux = "li";     break;
            case MOVE:      aux = "move";   break;
            case ADD:       aux = "add";    break;
            case SUB:       aux = "sub";    break;
            case MULT:      aux = "mult";   break;
            case DIV:       aux = "div";    break;
            case IN:        aux = "in";     break;
            case JAL:       aux = "jal";    break;
            case JR:        aux = "jr";     break;
            case OUT:       aux = "out";    break;
            case WAIT:      aux = "wait";   break;
    
        }

        //spacing flag condition
        if(!fun_flag){ 
            printf("        ");
        }

        if(!label_flag){
            printf("    ");
        }

        printf("%s", aux);

        switch (Noaux->op1.type){
            case regTemporary:printf(" $r%d", Noaux->op1.value);break;
            case constant: printf(" %d",Noaux->op1.value );break;
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

            case regTemporary:printf(", $r%d", Noaux->op2.value);break;
            case constant: printf(", %d",Noaux->op2.value );break;
            case funck: printf(" %s",Noaux->op2.name);break;
        }

        switch (Noaux->op3.type){
            case regTemporary:printf(", $r%d", Noaux->op3.value);break;
            case constant: printf(", %d",Noaux->op3.value );break;
            case labelk: printf(", L%d",Noaux->op3.value );break;
        }

        printf("\n");
        Noaux = Noaux->next;
    }
}


void iterate_list(void){

	quadruple *aux = lista_quad->first;

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
                assign_operation(assembly_list, regTemporary, 27, 
                                    constant,  aux->op1.adress, -1, -1);
                
                if(aux->op2.type == regTemporary) {

                    insert_assembly_code(ADD);
                    assign_operation(assembly_list, regTemporary, 27, regTemporary,  
                                        27, regTemporary, aux->op2.value);
                }

                insert_assembly_code(SW);
                assign_operation(assembly_list, regTemporary, 27, 
                                    regTemporary,  aux->op3.value, -1, -1);

                break;

            case load:
                insert_assembly_code(LI);
                assign_operation(assembly_list, regTemporary, 27, 
                                    constant,  aux->op2.adress, -1, -1);

                if(aux->op3.type == regTemporary){

                    insert_assembly_code(ADD);
                    assign_operation(assembly_list, regTemporary, 27, regTemporary,  
                                        27, regTemporary, aux->op3.value);

                }

                insert_assembly_code(LW);
                assign_operation(assembly_list, regTemporary, aux->op1.value, regTemporary,  
                                        27, -1, -1);
                break;


            case fun: 
                insert_assembly_code(LABEL);
                assign_op_1(assembly_list, funck, aux->op1.value);
                assembly_list->last->op1.name = aux->op1.name;
               
                break;

            case arg:
                i = 22;

                while(i<=argmt){
                    
                    insert_assembly_code(SW);
                    assign_operation(assembly_list, regTemporary, i, 
                                    regTemporary,  aux->op3.value, -1, -1);

                    i++;
                } 

                argmt = 21;

                break;

            case setArg:
                
                argmt++;

                insert_assembly_code(MOVE);
                assign_operation(assembly_list, regTemporary, argmt, 
                                    regTemporary, aux->op3.value, -1, -1);

                break;

            case call:             
                if(strcmp(aux->op3.name, "output") == 0){


                    insert_assembly_code(OUT);
                    assign_op_1(assembly_list, regTemporary, 22);


                }else if(strcmp(aux->op3.name, "input") == 0){
                    
                    insert_assembly_code(IN);
                    

                    insert_assembly_code(MOVE);
                    assign_operation(assembly_list, regTemporary, aux->op1.value, 
                                    regTemporary,  30, -1, -1);
                }
                else {

                    insert_assembly_code(JAL);
                    assign_operation(assembly_list, -1, aux->op1.value, 
                                    funck, -1, -1, -1);
                    assembly_list->last->op2.name = aux->op3.name;
                    
                    if(aux->op1.type == regTemporary){
                        insert_assembly_code(MOVE);
                        assign_operation(assembly_list, regTemporary, aux->op1.value, 
                                    regTemporary,  28, -1, -1);
                    }
                }
                break;

            case end: 

                if(strcmp(aux->op1.name, "main") != 0){
                    insert_assembly_code(JR);
                    assign_op_1(assembly_list, regTemporary, 31); 
                } 
                 
                break;

            case alloc:  
    

                break;

            case ret: 

                insert_assembly_code(MOVE);
                assign_operation(assembly_list, regTemporary, 28, 
                                    regTemporary,  aux->op1.value, -1, -1);

                insert_assembly_code(JR);
                assign_op_1(assembly_list, regTemporary, 31); 

                break;

            case label: 
                
                insert_assembly_code(LABEL);
                assign_op_1(assembly_list, labelk, aux->op3.value); 

                break;

            case IGLIGL:    
                insert_assembly_code(SET);
                assign_operation(assembly_list, regTemporary, aux->op1.value, 
                                    regTemporary,  aux->op2.value, regTemporary, aux->op3.value);  
                break;


            case DIF:     
                insert_assembly_code(SDT);
                assign_operation(assembly_list, regTemporary, aux->op1.value, 
                                    regTemporary,  aux->op2.value, regTemporary, aux->op3.value);
                break;

            case MEN:     
                insert_assembly_code(SLT);
                assign_operation(assembly_list, regTemporary, aux->op1.value, 
                                    regTemporary,  aux->op2.value, regTemporary, aux->op3.value);
                 
                break;

            case MAI:     
                insert_assembly_code(SGT);
                assign_operation(assembly_list, regTemporary, aux->op1.value, 
                                    regTemporary,  aux->op2.value, regTemporary, aux->op3.value);
    
                break;

            case MAIGL:     
                insert_assembly_code(SGE);
                assign_operation(assembly_list, regTemporary, aux->op1.value, 
                                    regTemporary,  aux->op2.value, regTemporary, aux->op3.value);
                 
                break;

            case MEIGL:     
                insert_assembly_code(SLE);
                assign_operation(assembly_list, regTemporary, aux->op1.value, 
                                    regTemporary,  aux->op2.value, regTemporary, aux->op3.value);
                 
                break;


            case SOM:    
                insert_assembly_code(ADD);
                assign_operation(assembly_list, regTemporary, aux->op1.value, 
                                    regTemporary,  aux->op2.value, regTemporary, aux->op3.value);

                break;

            case SUBT:   
                insert_assembly_code(SUB);
                assign_operation(assembly_list, regTemporary, aux->op1.value, 
                                    regTemporary,  aux->op2.value, regTemporary, aux->op3.value);
                
                break;

            case MUL:  
                insert_assembly_code(MULT);
                assign_operation(assembly_list, regTemporary, aux->op1.value, 
                                    regTemporary,  aux->op2.value, regTemporary, aux->op3.value);
                   
                break;

            case DIVI:  
                insert_assembly_code(DIV);
                assign_operation(assembly_list, regTemporary, aux->op1.value, 
                                    regTemporary,  aux->op2.value, regTemporary, aux->op3.value); 
                 
                break;

            case imed:    
                insert_assembly_code(LI);

                assign_operation(assembly_list, regTemporary, aux->op1.value, 
                                    regTemporary,  aux->op2.value, regTemporary, aux->op3.value);
                break;

            case jump:   
                insert_assembly_code(JUMP);
                assign_op_1(assembly_list, labelk, aux->op3.value);
    
                break;

            case beq:    
                insert_assembly_code(BEQ);
                assign_operation(assembly_list, regTemporary, aux->op1.value, 
                                    regTemporary,  aux->op2.value, regTemporary, aux->op3.value);
                break;

            case bne:    
                insert_assembly_code(BNE);
                assign_operation(assembly_list, regTemporary, aux->op1.value, 
                                    regTemporary,  aux->op2.value, regTemporary, aux->op3.value);
                break;

            default:
                printf("node not found\n");
                break;
		}

		aux = aux->next;
	}

}

//generate assembly code
void generate_assembly(void){

    
    assembly_list = (TList_assembly*)malloc(sizeof(TList_assembly));

    assembly_list->first = NULL;
    assembly_list->last = NULL;
    assembly_list->tamanho = 0;

	iterate_list();
	print_assembly_code(assembly_list->first);
    make_bin(assembly_list->first);
}


