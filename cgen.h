/****************************************************/
/*             Lib Gerador de codigos               */
/*                 Isadora Muniz                    */
/****************************************************/

#ifndef _CGEN_H_
#define _CGEN_H_


void code_gen(TreeNode * tree);

void percorre(TreeNode * t);

typedef enum {geral, id, Const, regTemp, labelk} typeQuad;

typedef struct nodeQuad {

    typeQuad type;
    char* name;
    int regTemp;
    int value;

 
}quadNode;


typedef enum {
	nop,
	halt,
	store,
	vet,
	fun,
	arg,
	setArg, 
	call,
    load,
    alloc,
    push, 
    pop,
    label,
    beq,
    jump,
    imed,
	end
} instr;

struct t_quad {

    instr instrucao;
    quadNode op1;
    quadNode op2;
    quadNode op3;

    struct t_quad *proximo;
};

typedef struct t_quad quadrupla;

typedef struct {
    quadrupla *primeiro;
    quadrupla *ultimo;
    int tamanho;

} lista;

lista *lista_quad;

#endif
