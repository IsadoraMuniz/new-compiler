/****************************************************/
/*          Lib Gerador de codigo binario           */
/*                 Isadora Muniz                    */
/****************************************************/

#ifndef _BIN_H_
#define _BIN_H_

void generate_binary(instruction_cell *aux);

typedef struct {
    int opcode, formato, val_op1;
    int val_op2, val_op3;

} instruction_info;

#endif
