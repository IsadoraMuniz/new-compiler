/****************************************************/
/*             Lib Gerador de codigos               */
/*                 Isadora Muniz                    */
/****************************************************/

#ifndef _CGEN_H_
#define _CGEN_H_

void gen_quad(TreeNode * tree);

void percorre(TreeNode * t);

typedef enum {geral, id, Const, regTemp, labelk, funck} quadKind;

typedef struct nodeQuad {

    quadKind type;
    char* name;
    int regTemp;
    int value;
    int endereco;

 
}quadNode;


typedef enum {
	nop,
	halt,
	store,
	fun,
	arg,
	setArg, 
	call,
    pop,
    vet,
    push,
    load,
    alloc,
    ret, 
    label,
    beq,
    bne,
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

//Variavel que contem a lista encadeada de quadruplas
lista *lista_quad;

#endif

