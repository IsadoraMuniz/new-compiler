/* /****************************************************/
/*              Gerador de codigo binario           */
/*                 Isadora Muniz                    */
/****************************************************/
/* 
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
    for (i = length-1; i >= 0; i--) {
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

void make_bin(instruction_cell *aux){

	int program_counter = 0;

	instruction_info instruction;

    int opcode, formato, val_op1;
    int val_op2, val_op3;

	int linha;

	

	code = fopen("binary_code.txt", "w");

	if(code == NULL){
		printf("Operation failed\n");
		return;
	}

	while(aux != NULL){

		switch (aux->opcode){
			
			case ADD:
				opcode = 0;
				formato = 1;
				val_op1 = aux->op1.value;
				val_op2 = aux->op2.value;
				val_op3 = aux->op3.value;
				break;
			
			case SUB:
				opcode = 1;
				formato = 1;
				val_op1 = aux->op1.value;
				val_op2 = aux->op2.value;
				val_op3 = aux->op3.value;
				break;

			case MULT:
				opcode = 4;
				formato = 1;
				val_op1 = aux->op1.value;
				val_op2 = aux->op2.value;
				val_op3 = aux->op3.value;
				break;
			

			case DIV:
				opcode = 5;
				formato = 1;
				val_op1 = aux->op1.value;
				val_op2 = aux->op2.value;
				val_op3 = aux->op3.value;
				break;
			

			case LABEL:
				linha = program_counter;
				opcode = 7;
				formato = 4;
				val_op1 = 0;
				break;

			case SET:
				opcode = 8;
				formato = 1;
				val_op1 = aux->op1.value;
				val_op2 = aux->op2.value;
				val_op3 = aux->op3.value;
				break;
			
			case SDT:
				opcode = 9;
				formato = 1;
				val_op1 = aux->op1.value;
				val_op2 = aux->op2.value;
				val_op3 = aux->op3.value;
				break;
		
			case SGT:
				opcode = 10;
				formato = 1;
				val_op1 = aux->op1.value;
				val_op2 = aux->op2.value;
				val_op3 = aux->op3.value;
				break;
			
			case SGE:
				opcode = 11;
				formato = 1;
				val_op1 = aux->op1.value;
				val_op2 = aux->op2.value;
				val_op3 = aux->op3.value;
				break;

			case SLE:
				opcode = 12;
				formato = 1;
				val_op1 = aux->op1.value;
				val_op2 = aux->op2.value;
				val_op3 = aux->op3.value;
				break;

			case BEQ:
				opcode = 13;
				formato = 2;
				val_op1 = aux->op1.value;
				val_op2 = aux->op2.value;
				val_op3 = aux->op3.value;
				break;

			case SLT:
				opcode = 14;
				formato = 1;
				val_op1 = aux->op1.value;
				val_op2 = aux->op2.value;
				val_op3 = aux->op3.value;
				break;

			case BNE:
				opcode = 15;
				formato = 2;
				val_op1 = aux->op1.value;
				val_op2 = aux->op2.value;
				val_op3 = aux->op3.value;
				break;

			case JUMP:
				opcode = 16;
				formato = 4;
				val_op1 = linha;
				val_op2 = 0;
				break;	
			
			case IN:
				opcode = 17;
				formato = 4;
				val_op1 = 0;
				break;

			case OUT:
				opcode = 18;
				formato = 3;
				val_op1 = aux->op1.value;
				val_op2 = 0;
				break;

			case HALT:
				opcode = 19;
				formato = 4;
				val_op1 = 0;
				break;
			
			case LW:
				opcode = 20;
				formato = 2;
				val_op1 = aux->op1.value;
				val_op2 = aux->op2.value;
				val_op3 = 0;
				break;

			case SW:
				opcode = 21;
				formato = 2;
				val_op1 = aux->op1.value;
				val_op2 = aux->op2.value;
				val_op3 = 0;
				break;

			case MOVE:
				opcode = 22;
				formato = 2;
				val_op1 = aux->op1.value;
				val_op2 = aux->op2.value;
				val_op3 = 0;
				break;

			case WAIT:
				opcode = 23;
				formato = 4;
				val_op1 = 0;
				break;
			
			case LI:
				opcode = 24;
				formato = 3;
				val_op1 = aux->op1.value;
				val_op2 = aux->op2.value;
				break;

			case JR:
				opcode = 27;
				formato = 3;
				val_op1 = aux->op1.value;
				val_op2 = 0;
				break;

			case JAL:
				opcode = 28;
				formato = 4;
				val_op1 = linha;
				val_op2 = 0;
				break;

			case NOP:
				opcode = 63;
				formato = 4;
				val_op1 = 0;
				break;

		}

		
		//fprintf(code, "{5'd%d, ", opcode);
		fprintf(code,"%s", decimalToExtendedBinary(opcode, 5));
		if(formato == 1){
			fprintf(code, "%s%s%s%s\n", decimalToExtendedBinary(val_op1, 5), decimalToExtendedBinary(val_op2, 5), decimalToExtendedBinary(val_op3, 5), "000000000000");
			//fprintf(code, "5'd%d, 5'd%d, 5'd%d, 12'd0 }\n", val_op1, val_op2, val_op3);
		}else if(formato == 2){
			fprintf(code,"%s%s%s\n", decimalToExtendedBinary(val_op1, 5), decimalToExtendedBinary(val_op2, 5), decimalToExtendedBinary(val_op3, 17));
			//fprintf(code, "5'd%d, 5'd%d, 17'd%d }\n", val_op1, val_op2, val_op3);
		}else if(formato == 3){
			fprintf(code,"%s%s\n", decimalToExtendedBinary(val_op1, 5), decimalToExtendedBinary(val_op2, 22));
			//fprintf(code, "5'd%d, 22'd%d }\n", val_op1, val_op2);
		}else{
			fprintf(code,"%s\n", decimalToExtendedBinary(val_op1, 27));
			//fprintf(code, "27'd%d }\n", val_op1);
		}

		program_counter++;
		aux = aux->next;
	}

	fprintf(code,"\n	assign INSTRUCAO = memoria [PC]; \n\nendmodule\n");
	fclose(code);
	printf("\n\n         *** BINARIO GERADO COM SUCESSO!!! ***\n\n");
}
 */







