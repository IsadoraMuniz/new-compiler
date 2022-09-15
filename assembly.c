#include "globals.h"
#include "symtab.h"
#include <string.h>
#include "cgen.h"
#include "assembly.h"

int *posicao_label;
int contParams = 0;
int argAtual = 0;
int contArg = 0;
static int linhaAtual = 1;
static int qtdFunc = 0;
char function[40];

typedef struct {
    char instruc[10];
    char reg1[40];
    char reg2[40];
    char regD[10];
    char label[10];
} assembly;

assembly *instrucao;

typedef struct {
    char name[40];
    int posicao;
} struct_label;

struct_label *pos_label;

char regist[40];

typedef enum {
$zero, $at, $v0, $v1, $a0, $a1, $a2, $a3, $t0, $t1, $t2, $t3, $t4, $t5, $t6, $t7, $t8, $t9, $t10, $t11, $t12, $t13,
$t14, $t15, $t16, $t17, $k0, $k1, $gp, $sp, $fp, $ra
} banco_regs;

int parametro(char *line, int index){
    int j = 0, i;
    for (i = index; i < 40 && line[i] != ',' ; i++)
    {
        regist[j] = line[i];
        j++;
    }
    regist[j] = '\0';
    return i+1;
}

void ultimo_parametro(char *line, int index){
    int j = 0, i;
    for (i = index; i < 40 && line[i] != ')' ; i++) {
        regist[j] = line[i];
        j++;
    }
    regist[j] = '\0';
}

int busca_registrador(char *regAux){ //retorna o número do registrador no processador
    if (strcmp(regAux, "$zero") == 0 || strcmp(regist, "-") == 0 || strcmp(regist, "0") == 0){
        return $zero;
    } else if(strcmp(regAux, "$at") == 0){
        return $at;
    } else if(strcmp(regAux, "$v0") == 0){
        return $v0;
    } else if(strcmp(regAux, "$v1") == 0){
        return $v1;
    } else if(strcmp(regAux, "$a0") == 0){
        return $a0;
    } else if(strcmp(regAux, "$a1") == 0){
        return $a1;
    } else if(strcmp(regAux, "$a2") == 0){
        return $a2;
    } else if(strcmp(regAux, "$a3") == 0){
        return $a3;
    } else if(strcmp(regAux, "$t0") == 0){
        return $t0;
    } else if(strcmp(regAux, "$t1") == 0){
        return $t1;
    } else if(strcmp(regAux, "$t2") == 0){
        return $t2;
    } else if(strcmp(regAux, "$t3") == 0){
        return $t3;
    } else if(strcmp(regAux, "$t4") == 0){
        return $t4;
    } else if(strcmp(regAux, "$t5") == 0){
        return $t5;
    } else if(strcmp(regAux, "$t6") == 0){
        return $t6;
    } else if(strcmp(regAux, "$t7") == 0){
        return $t7;
    } else if(strcmp(regAux, "$t8") == 0){
        return $t8;
    } else if(strcmp(regAux, "$t9") == 0){
        return $t9;
    } else if(strcmp(regAux, "$t10") == 0){
        return $t10;
    } else if(strcmp(regAux, "$t11") == 0){
        return $t11;
    } else if(strcmp(regAux, "$t12") == 0){
        return $t12;
    } else if(strcmp(regAux, "$t13") == 0){
        return $t13;
    } else if(strcmp(regAux, "$t14") == 0){
        return $t14;
    } else if(strcmp(regAux, "$t15") == 0){
        return $t15;
    } else if(strcmp(regAux, "$t16") == 0){
        return $t16;
    } else if(strcmp(regAux, "$t17") == 0){
        return $t17;
    } else if(strcmp(regAux, "$k0") == 0){
        return $k0;
    } else if(strcmp(regAux, "$k1") == 0){
        return $k1;
    } else if(strcmp(regAux, "$gp") == 0){
        return $gp;
    } else if(strcmp(regAux, "$sp") == 0){
        return $sp;
    } else if(strcmp(regAux, "$fp") == 0){
        return $fp;
    } else if(strcmp(regAux, "$ra") == 0){
        return $ra;
    }
}

void addi(char *line, int index){

    strcpy(instrucao[linhaAtual].instruc, "addi");

    index = parametro(line, index); 

    char aux[5];
    sprintf(aux, "$%d,", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].reg2, aux); 

    index = parametro(line, index);

    strcpy(instrucao[linhaAtual].regD, regist);

    strcpy(instrucao[linhaAtual].reg1, "$0,");
}

void jump(char *line, int index){

    strcpy(instrucao[linhaAtual].instruc, "jump");

    index = parametro(line, index);

    strcpy(instrucao[linhaAtual].reg1, regist);
    strcpy(instrucao[linhaAtual].reg2, ""); 
    strcpy(instrucao[linhaAtual].regD, ""); 
}

void label(char *line, int index){
    int j = 0, i;
    for (i = index+1; i < 40 && line[i] != ',' ; i++)
    {
        regist[j] = line[i];
        j++;
    }
    regist[j] = '\0';
    
    posicao_label[atoi(regist)] = linhaAtual;
    index = parametro(line, index);
    strcpy(instrucao[linhaAtual].label, regist);
    linhaAtual--;
}

void funcao(char *line, int index) {
    // armazeno qual é a label relacionada à essa funcao

    index = parametro(line, index);
    index = parametro(line, index);

    sprintf(function, "%s", regist);
    if (strcmp(function, "main") == 0) {
        linhaMain = linhaAtual;
    }
    //aloco mais uma posicao pro vetor
    pos_label = (struct_label *) realloc(pos_label, (qtdFunc+1) * sizeof(struct_label));

    //coloco o nome da funcao no vetor
    strcpy(pos_label[qtdFunc].name, regist);
    pos_label[qtdFunc].posicao = linhaAtual;

    qtdFunc++;
    strcpy(instrucao[linhaAtual].label, regist);
    linhaAtual--;
}

void load(char *line, int index){
    strcpy(instrucao[linhaAtual].instruc, "lw");

    index = parametro(line, index);

    char aux[10];
    sprintf(aux, "$%d,", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].reg1, aux);

    index = parametro(line, index);
    int pos;
   
    pos = busca_indice(regist, function, "variável");
    if (pos == -1) {
        pos = busca_indice(regist, "global", "variável");
    }
    sprintf(aux, "%d", pos);
    strcpy(instrucao[linhaAtual].reg2, aux);

    strcpy(instrucao[linhaAtual].regD, "($0)");
}

void load_v(char *line, int index){
    strcpy(instrucao[linhaAtual].instruc, "lw");

    index = parametro(line, index);

    char aux[10];
    sprintf(aux, "$%d,", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].reg1, aux);

    index = parametro(line, index);
    int pos;
   
    pos = busca_indice(regist, function, "variável");
    if (pos == -1) {
        pos = busca_indice(regist, "global", "variável");
    }
    sprintf(aux, "%d", pos);
    strcpy(instrucao[linhaAtual].reg2, aux);

    ultimo_parametro(line, index);
    sprintf(aux, "($%d)", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].regD, aux);
}

void store(char *line, int index){

    strcpy(instrucao[linhaAtual].instruc, "sw");

    index = parametro(line, index);

    char aux[10];
    sprintf(aux, "$%d,", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].reg1, aux); 

    index = parametro(line, index);
    int pos;
    pos = busca_indice(regist, function, "variável");
    if (pos == -1) {
        pos = busca_indice(regist, "global", "variável");
    }
    sprintf(aux, "%d", pos);
    strcpy(instrucao[linhaAtual].reg2, aux); 

    ultimo_parametro(line, index);

    sprintf(aux, "($%d)", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].regD, aux);
}

void call(char *line, int index){
    char aux[5];

    strcpy(instrucao[linhaAtual].instruc, "jal");

    index = parametro(line, index);

    index = parametro(line, index);

    int i = 0;
    while ( strcmp(pos_label[i].name, regist) != 0 )
    {
        i++;
    }
    sprintf(aux, "%d", pos_label[i].posicao);
    strcpy(instrucao[linhaAtual].reg1, aux);
    strcpy(instrucao[linhaAtual].reg2, "");
    strcpy(instrucao[linhaAtual].regD, "");
    contArg = contParams;
    contParams = 0;
}

void if_conditional(char *line, int index) {
    index = parametro(line, index);
    index = parametro(line, index);

    strcpy(instrucao[linhaAtual-1].regD, regist);
    linhaAtual--;
}

void branchNE(char *line, int index) {

    strcpy(instrucao[linhaAtual].instruc, "bne");

    index = parametro(line, index);

    char aux[5];
    sprintf(aux, "$%d,", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].reg1, aux);

    index = parametro(line, index);

    sprintf(aux, "$%d,", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].reg2, aux);

    strcpy(instrucao[linhaAtual].regD, "");
}

void branchEQ(char *line, int index) {
    strcpy(instrucao[linhaAtual].instruc, "beq");

    index = parametro(line, index);

    char aux[5];
    sprintf(aux, "$%d,", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].reg1, aux);

    index = parametro(line, index);

    sprintf(aux, "$%d,", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].reg2, aux);

    strcpy(instrucao[linhaAtual].regD, "");
}

void branchGE(char *line, int index) {
    strcpy(instrucao[linhaAtual].instruc, "bge");

    index = parametro(line, index);

    char aux[5];
    sprintf(aux, "$%d,", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].reg1, aux);

    index = parametro(line, index);

    sprintf(aux, "$%d,", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].reg2, aux);

    strcpy(instrucao[linhaAtual].regD, "");
}

void branchGT(char *line, int index) {
    strcpy(instrucao[linhaAtual].instruc, "bgt");

    index = parametro(line, index);

    char aux[5];
    sprintf(aux, "$%d,", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].reg1, aux);

    index = parametro(line, index);

    sprintf(aux, "$%d,", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].reg2, aux);

    strcpy(instrucao[linhaAtual].regD, "");
}

void branchLE(char *line, int index) {
    strcpy(instrucao[linhaAtual].instruc, "ble");

    index = parametro(line, index);

    char aux[5];
    sprintf(aux, "$%d,", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].reg1, aux);

    index = parametro(line, index);

    sprintf(aux, "$%d,", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].reg2, aux);

    strcpy(instrucao[linhaAtual].regD, "");
}

void branchLT(char *line, int index) {
    strcpy(instrucao[linhaAtual].instruc, "blt");

    index = parametro(line, index);

    char aux[5];
    sprintf(aux, "$%d,", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].reg1, aux);

    index = parametro(line, index);

    sprintf(aux, "$%d,", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].reg2, aux);

    strcpy(instrucao[linhaAtual].regD, "");
}

void ret(char *line, int index) {
    strcpy(instrucao[linhaAtual].instruc, "jr");
    strcpy(instrucao[linhaAtual].reg1, "$ra");
    strcpy(instrucao[linhaAtual].reg2, ""); 
    strcpy(instrucao[linhaAtual].regD, "");
}

void add(char *line, int index) {
    strcpy(instrucao[linhaAtual].instruc, "add");

    index = parametro(line, index);

    char aux[5];
    sprintf(aux, "$%d,", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].reg1, aux);

    index = parametro(line, index);

    sprintf(aux, "$%d,", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].reg2, aux);

    ultimo_parametro(line, index);

    sprintf(aux, "$%d", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].regD, aux);
}

void sub(char *line, int index) {
    strcpy(instrucao[linhaAtual].instruc, "sub");

    index = parametro(line, index);

    char aux[5];
    sprintf(aux, "$%d,", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].reg1, aux);
    index = parametro(line, index);

    sprintf(aux, "$%d,", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].reg2, aux);

    ultimo_parametro(line, index);

    sprintf(aux, "$%d", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].regD, aux);
}

void div_(char *line, int index) {
    strcpy(instrucao[linhaAtual].instruc, "div");

    index = parametro(line, index);

    char aux[5];
    sprintf(aux, "$%d,", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].reg1, aux);

    index = parametro(line, index);

    sprintf(aux, "$%d,", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].reg2, aux);

    ultimo_parametro(line, index);

    sprintf(aux, "$%d", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].regD, aux);
}

void mult(char *line, int index) {
    strcpy(instrucao[linhaAtual].instruc, "mult");

    index = parametro(line, index);

    char aux[5];
    sprintf(aux, "$%d,", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].reg1, aux);

    index = parametro(line, index);

    sprintf(aux, "$%d,", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].reg2, aux);

    ultimo_parametro(line, index);

    sprintf(aux, "$%d", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].regD, aux);
}

void param(char *line, int index) {
    strcpy(instrucao[linhaAtual].instruc, "add");

    index = parametro(line, index);

    char aux[5];
    char num[5];
    sprintf(aux, "$%d,", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].reg1, aux);

    strcpy(instrucao[linhaAtual].reg2, "$0,");

    sprintf(num, "$a%d", contParams);    
    sprintf(aux, "$%d", busca_registrador(num));
    contParams++;
    strcpy(instrucao[linhaAtual].regD, aux);
}

void arg(char *line, int index) {
    strcpy(instrucao[linhaAtual].instruc, "sw");

    index = parametro(line, index);
    
   
    if (strcmp(instrucao[linhaAtual-1].instruc, "sw") != 0)
        argAtual = 0;

    char aux[5];
    char num[5]; 
    sprintf(num, "$a%d", argAtual);
    sprintf(aux, "$%d,", busca_registrador(num));
    argAtual++;
    strcpy(instrucao[linhaAtual].reg1, aux);

    index = parametro(line, index);
    int pos;
    pos = busca_indice(regist, function, "variável");
    if (pos == -1) {
        pos = busca_indice(regist, "global", "variável");
    }
    sprintf(aux, "%d", pos);
    strcpy(instrucao[linhaAtual].reg2, aux);

    sprintf(aux, "($%d)", 0);
    strcpy(instrucao[linhaAtual].regD, aux);
}

void alloc(char *line, int index) {
    strcpy(instrucao[linhaAtual].instruc, "sw");

    strcpy(instrucao[linhaAtual].reg1, "$0,");

    index = parametro(line, index);
    int pos;
    pos = busca_indice(regist, function, "variável");
    if (pos == -1) {
        pos = busca_indice(regist, "global", "variável");
    }
    char aux[5];
    sprintf(aux, "%d", pos);
    strcpy(instrucao[linhaAtual].reg2, aux);

    sprintf(aux, "($%d)", 0);
    strcpy(instrucao[linhaAtual].regD, aux);
}

void load_varg(char *line, int index) {
    strcpy(instrucao[linhaAtual].instruc, "addi");

    index = parametro(line, index);

    char aux[10];
    sprintf(aux, "$%d,", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].reg2, aux);

    index = parametro(line, index);
    int pos;
   
    pos = busca_indice(regist, function, "variável");
    if (pos == -1) {
        pos = busca_indice(regist, "global", "variável");
    }
    sprintf(aux, "%d", pos);
    strcpy(instrucao[linhaAtual].regD, aux); 

    strcpy(instrucao[linhaAtual].reg1, "$0,");
}

void load_vp(char *line, int index) {
    strcpy(instrucao[linhaAtual].instruc, "lw");
    
    index = parametro(line, index);
    char primeiro_param[10];
    sprintf(primeiro_param, "$%d,", busca_registrador(regist));

    char aux[10];
    sprintf(aux, "$%d,", busca_registrador("$t16"));
    strcpy(instrucao[linhaAtual].reg1, aux);

    index = parametro(line, index); 

    int pos;
    pos = busca_indice(regist, function, "variável");
    if (pos == -1) {
        pos = busca_indice(regist, "global", "variável");
    }
    sprintf(aux, "%d", pos);
    strcpy(instrucao[linhaAtual].reg2, aux);
    strcpy(instrucao[linhaAtual].regD, "($0)");
    linhaAtual++;
    instrucao = (assembly *) realloc(instrucao, (linhaAtual+1) * sizeof(assembly));
    ultimo_parametro(line, index);
    strcpy(instrucao[linhaAtual].instruc, "add");
    
    sprintf(aux, "$%d,", busca_registrador(regist));
    strcpy(instrucao[linhaAtual].reg1, aux); 

    sprintf(aux, "$%d,", busca_registrador("$t16"));
    strcpy(instrucao[linhaAtual].reg2, aux); 

    sprintf(aux, "$%d,", busca_registrador("$t17"));
    strcpy(instrucao[linhaAtual].regD, aux); 
    linhaAtual++;
    instrucao = (assembly *) realloc(instrucao, (linhaAtual+1) * sizeof(assembly));
    strcpy(instrucao[linhaAtual].instruc, "lw");
    
    strcpy(instrucao[linhaAtual].reg1, primeiro_param);

    strcpy(instrucao[linhaAtual].reg2, "0"); 

    sprintf(aux,"($%d)", busca_registrador("$t17")); 
    strcpy(instrucao[linhaAtual].regD, aux);
}

void nome_instrucao(char *read) {
    char instruc[15];
    int j = 0, i;
    for (i = 1; read[i] != ',' && i < 15; i++)
    {
        instruc[j] = read[i];
        j++;
    }
    i++;
    instruc[j] = '\0';

    if (strcmp(instruc, "CONST") == 0) 
    {
        addi(read, i);
    } 
    else if (strcmp(instruc, "GOTO") == 0)
    {
        jump(read, i);
    }
    else if (strcmp(instruc, "LABEL") == 0)
    {
        label(read, i);
    }
    else if (strcmp(instruc, "FUNC") == 0)
    {
        funcao(read, i);
    }
    else if (strcmp(instruc, "LOAD") == 0)
    {
        load(read, i);
    }
    else if (strcmp(instruc, "STORE") == 0)
    {
        store(read, i);
    }
    else if (strcmp(instruc, "CALL") == 0)
    {
        call(read, i);
    }
    else if (strcmp(instruc, "IF") == 0)
    {
        if_conditional(read, i);
    }
    else if (strcmp(instruc, "DIFERENTE") == 0)
    {
        branchNE(read, i);
    }
    else if (strcmp(instruc, "IGUAL") == 0)
    {
        branchEQ(read, i);
    }
    else if (strcmp(instruc, "MAIORIGUAL") == 0)
    {
        branchGE(read, i);
    }
    else if (strcmp(instruc, "MAIOR") == 0)
    {
        branchGT(read, i);
    }
    else if (strcmp(instruc, "MENORIGUAL") == 0)
    {
        branchLE(read, i);
    }
    else if (strcmp(instruc, "MENOR") == 0)
    {
        branchLT(read, i);
    }
    else if (strcmp(instruc, "END") == 0) {
        linhaAtual--;
    }
    else if (strcmp(instruc, "RETURN") == 0) {
        ret(read, i);
    }
    else if (strcmp(instruc, "SOMA") == 0)
    {
        add(read, i);
    }
    else if (strcmp(instruc, "SUB") == 0)
    {
        sub(read, i);
    }
    else if (strcmp(instruc, "DIV") == 0)
    {
        div_(read, i);
    }
    else if (strcmp(instruc, "MULT") == 0)
    {
        mult(read, i);
    }
    else if (strcmp(instruc, "ALLOC") == 0) {
        alloc(read, i);
    }
    else if (strcmp(instruc, "ALLOC_V") == 0) {
        alloc(read, i);
    }
    else if (strcmp(instruc, "ARG") == 0) {
        arg(read, i);
    }
    else if (strcmp(instruc, "PARAMETRO") == 0) {
        param(read, i);
    }
    else if (strcmp(instruc, "LOAD_VP") == 0) {
        load_vp(read, i);
    }
    else if (strcmp(instruc, "LOAD_VARG") == 0) {
        load_varg(read, i);
    }
    else if (strcmp(instruc, "LOAD_V") == 0) {
        load_v(read, i);
    }
    else {
        strcpy(instrucao[linhaAtual].instruc, instruc);
    }
}

void imprime(){
    for (int i = 1; i < linhaAtual; i++)
    {
        fprintf(code_ass, "%s %s%s%s", instrucao[i].instruc, instrucao[i].reg1, instrucao[i].reg2, instrucao[i].regD); 
        if (strcmp(instrucao[i].label, "") != 0)
        {
            fprintf(code_ass, "\t//%s", instrucao[i].label);
        }
        
        fprintf(code_ass, "\n");    
    }
}

void labels(){
    char aux[10];
    int j;
    for (int i = 0; i < linhaAtual; i++)
    {
        if(strcmp(instrucao[i].instruc, "jump") == 0){
            if (instrucao[i].reg1[0] == 'L')   
            {
                j = 1;
                while(instrucao[i].reg1[j] != '\0'){
                    aux[j-1] = instrucao[i].reg1[j];
                    j++;
                }
                aux[j] = '\0';

                char aux2[10];
                sprintf(aux2, "%d", posicao_label[atoi(aux)]);   
                strcpy(instrucao[i].reg1, aux2);
            }
        } else if(strcmp(instrucao[i].instruc, "bne") == 0 || strcmp(instrucao[i].instruc, "beq") == 0 ||
                strcmp(instrucao[i].instruc, "blt") == 0 || strcmp(instrucao[i].instruc, "bgt") == 0 ||
                strcmp(instrucao[i].instruc, "ble") == 0 || strcmp(instrucao[i].instruc, "bge") == 0){  
            j = 1;
            while(instrucao[i].regD[j] != '\0'){
                aux[j-1] = instrucao[i].regD[j];
                j++;
            }
            aux[j] = '\0';

            char aux2[10];
            sprintf(aux2, "%d", posicao_label[atoi(aux)]);
            strcpy(instrucao[i].regD, aux2);
        }
    }
}


void assemblyGen(){
    char read[51];
    strcpy(function, "");
    instrucao = (assembly *) malloc(sizeof(assembly));
    posicao_label = (int *) malloc(sizeof(int) * returnLabel()+1);
    pos_label = (struct_label *) malloc(sizeof(struct_label));

    while(fgets(read, 50, code) != NULL){

        while(read[0] != '(')
            fgets(read, 50, code);
        
        instrucao = (assembly *) realloc(instrucao, (linhaAtual+1) * sizeof(assembly));
        nome_instrucao(read);
        linhaAtual++;
    }
    labels();
    imprime();

    free(instrucao);
    free(posicao_label);
    free(pos_label);
}