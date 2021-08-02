

/****************************************************/
/*              Gerador de codigo binario           */
/*                 Isadora Muniz                    */
/****************************************************/

#include "globals.h"
#include "cgen.h"
#include "assmb.h"
#include "bin.h"
#include <stdio.h>
#include <string.h>

#define MAX 10000


char *decimalToExtendedBinary(int decimal, int length) {
    char *binary = (char*) malloc((length + 1) * sizeof(char));
    
    int i;
    for (i = length - 1; i >= 0; i--) {
        binary[i] = decimal % 2 ? '1' : '0';
        decimal = decimal / 2;
    }
    binary[length] = '\0';

    return binary;
}

instruction_info assign_instruction(int opcode, int formato,
							int val_op1, int val_op2,int val_op3) {
	
	instruction_info new_instruction;

	new_instruction.opcode = opcode;
	new_instruction.formato = formato;
	new_instruction.val_op1 = val_op1;
	new_instruction.val_op1 = val_op2;
	new_instruction.val_op1 = val_op3;

	return new_instruction;

}

void generate_binary(instruction_cell *aux){

	int PC = 0;

	int linha;

	int opcode, formato, val_op1;
    int val_op2, val_op3;

	instruction_info instruction;

	code = fopen("output-files/binary_code.txt", "w");

	if(code == NULL){
		printf("Operation failed\n");
		return;
	}

	while(aux != NULL){


		switch (aux->opcode){
			
			case ADD:

				instruction = assign_instruction(0, 1, aux->op1.value, aux->op2.value, aux->op3.value); 
				
				break;
			
			case SUB:
				instruction = assign_instruction(1, 1, aux->op1.value, aux->op2.value, aux->op3.value); 
				
				break;

			case MULT:
				instruction = assign_instruction(4, 1, aux->op1.value, aux->op2.value, aux->op3.value); 
				
				break;
			

			case DIV:
				instruction = assign_instruction(5, 1, aux->op1.value, aux->op2.value, aux->op3.value); 
			
				break;
			

			case LABEL:
				instruction = assign_instruction(7, 4, aux->op1.value, 0, 0); 
				linha = PC;
				
				break;

			case SET:
				instruction = assign_instruction(8, 1, aux->op1.value, aux->op2.value, aux->op3.value); 
				
				break;
			
			case SDT:
				instruction = assign_instruction(9, 1, aux->op1.value, aux->op2.value, aux->op3.value); 
				
				break;
		
			case SGT:
				instruction = assign_instruction(10, 1, aux->op1.value, aux->op2.value, aux->op3.value); 
				
				break;
			
			case SGE:
				instruction = assign_instruction(11, 1, aux->op1.value, aux->op2.value, aux->op3.value); 
				
				break;

			case SLE:
				instruction = assign_instruction(12, 1, aux->op1.value, aux->op2.value, aux->op3.value); 
				
				break;

			case BEQ:
				instruction = assign_instruction(13, 2, aux->op1.value, aux->op2.value, aux->op3.value); 
				
				break;

			case SLT:
				instruction = assign_instruction(14, 1, aux->op1.value, aux->op2.value, aux->op3.value); 
				
				break;

			case BNE:
				instruction = assign_instruction(15, 2, aux->op1.value, aux->op2.value, aux->op3.value); 
				
				break;

			case JUMP:
				instruction = assign_instruction(16, 4, linha, 0, 0); 

				break;	
			
			case IN:
				instruction = assign_instruction(17, 4, 0, 0, 0);
				
				break;

			case OUT:
				instruction = assign_instruction(18, 3, aux->op1.value, 0, 0);
				
				break;

			case HALT:
				instruction = assign_instruction(19, 4, 0, 0, 0);
				
				break;
			
			case LW:
				instruction = assign_instruction(20, 2, aux->op1.value, aux->op2.value, 0);
				
				break;

			case SW:
				instruction = assign_instruction(21, 2, aux->op1.value, aux->op2.value, 0);
				
				break;

			case MOVE:
				instruction = assign_instruction(22, 2, aux->op1.value, aux->op2.value, 0);
				
				break;

			case WAIT:
				instruction = assign_instruction(23, 4, 0, 0, 0);
				
				break;
			
			case LI:
				instruction = assign_instruction(24, 3, aux->op1.value, aux->op2.value, 0);
				
				break;

			case JR:
				instruction = assign_instruction(27, 3, aux->op1.value, 0, 0);
				
				break;

			case JAL:
				instruction = assign_instruction(28, 4, linha, 0, 0);
			
				break;

			case NOP:
				instruction = assign_instruction(63, 4, 0, 0, 0);
			
				break;

		}

		fprintf(code,"%s", decimalToExtendedBinary(instruction.opcode, 5));
		if(instruction.formato == 1){
			fprintf(code, "%s%s%s%s\n", decimalToExtendedBinary(instruction.val_op1, 5), decimalToExtendedBinary(instruction.val_op2, 5), decimalToExtendedBinary(instruction.val_op3, 5), "000000000000");
		}else if(instruction.formato == 2){
			fprintf(code,"%s%s%s\n", decimalToExtendedBinary(instruction.val_op1, 5), decimalToExtendedBinary(instruction.val_op2, 5), decimalToExtendedBinary(instruction.val_op3, 17));
		}else if(instruction.formato == 3){
			fprintf(code,"%s%s\n", decimalToExtendedBinary(instruction.val_op1, 5), decimalToExtendedBinary(instruction.val_op2, 22));
			
		}else{
			fprintf(code,"%s\n", decimalToExtendedBinary(instruction.val_op1, 27));
		}

		PC++;
		aux = aux->next;
	}

	fclose(code);
	printf("\n\n	   Codigo binario gerado !\n\n");
}