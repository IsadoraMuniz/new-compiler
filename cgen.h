/****************************************************/
/*             Lib Gerador de codigos               */
/*                 Isadora Muniz                    */
/****************************************************/

#ifndef _CGEN_H_
#define _CGEN_H_

void generate_intermediate_code(TreeNode * tree);

void traverse(TreeNode * t);

typedef enum {general, id, constant, regTemporary, labelk, funck} quadKind;

typedef struct nodeQuad {
    quadKind type;
    char* name;
    int regTemporary;
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
    quadNode op1, op2, op3;
    struct t_quad *next;
};

typedef struct t_quad quadruple;

typedef struct {
    quadruple *first, *last;
    int tamanho;

} lista;

//Variavel que contem a lista encadeada de quadruplas
lista *lista_quad;

#endif

