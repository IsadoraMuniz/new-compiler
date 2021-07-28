/****************************************************/
/*             Lib Gerador de codigos               */
/*                 Isadora Muniz                    */
/****************************************************/

#ifndef _CGEN_H_
#define _CGEN_H_

void code_gen(TreeNode * tree);

void percorre(TreeNode * t);

typedef enum {geral, id, Const, regTemp, labelk, funck} quadKind;

typedef struct nodeQuad {
    quadKind type;
    char* name;
    int regTemp;
    int value;
    int adress;

}quadNode;


typedef enum {
	nop,
	halt,
	store,
	fun,
	arg,
	setArg, 
	call,
    load,
    alloc,
    ret, 
    label,
    beq,
    bne,
    jump,
    imed,
	end
} instruction;

struct t_quad {

    instruction instruction_name;
    quadNode op1;
    quadNode op2;
    quadNode op3;

    struct t_quad *proximo;
};

typedef struct t_quad quadruple;

typedef struct {
    quadruple *primeiro;
    quadruple *ultimo;
    int tamanho;

} lista;

//Variavel que contem a lista encadeada de quadruplas
lista *lista_quad;

#endif

