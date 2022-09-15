#include "globals.h"
#include "assembly.h"
#include <string.h>

static int linhaAtual = 0;
char reg[10];

int registrador(char *line, int index){  
    int j = 0, i;
    for (i = index+1; i < 40 && line[i] != ',' && line[i] != '\n' && line[i] != ')'; i++)
    {
        reg[j] = line[i];
        j++;
    }
    reg[j] = '\0';
    return i+1;
}

int imediato(char *line, int index){   
    int j = 0, i;
    for (i = index; i < 40 && line[i] != ',' && line[i] != '\n' && line[i] != '('; i++) //pega imediato
    {
        reg[j] = line[i];
        j++;
    }
    reg[j] = '\0';

    return i+1;
}

void addi_bin(char *read, int i) {
    fprintf(code_bin, "mem[%d] = {6'd21, ", linhaAtual);

    i = registrador(read, i);
    fprintf(code_bin, "5'd%d, ", atoi(reg));

    i = registrador(read, i);
    fprintf(code_bin, "5'd%d, ", atoi(reg));
    
    i = imediato(read, i);
    fprintf(code_bin, "16'd%d}; // addi\n", atoi(reg));
}

void jump_bin(char *read, int i) {
    fprintf(code_bin, "mem[%d] = {6'd13, ", linhaAtual);
    i = imediato(read, i);
    fprintf(code_bin, "26'd%d}; // jump\n", atoi(reg));
}

void jr_bin(char *read, int i) {
    fprintf(code_bin, "mem[%d] = {6'd12, 5'd31, 5'd0, 5'd0, 5'd0, 6'd0}; // jr $ra\n", linhaAtual);
}

void load_bin(char *read, int i) {
    fprintf(code_bin, "mem[%d] = {6'd23, ", linhaAtual);

    i = registrador(read, i);
    int reg2 = atoi(reg);

    i = imediato(read, i);
    int imd = atoi(reg);

    i = registrador(read, i);
    int reg1 = atoi(reg); 

    fprintf(code_bin, "5'd%d, 5'd%d, 16'd%d}; // lw\n", reg1, reg2, imd);
}

void store_bin(char *read, int i) {
    fprintf(code_bin, "mem[%d] = {6'd22, ", linhaAtual);

    i = registrador(read, i);
    int reg2 = atoi(reg);

    i = imediato(read, i);
    int imd = atoi(reg);

    i = registrador(read, i);
    int reg1 = atoi(reg); 

    fprintf(code_bin, "5'd%d, 5'd%d, 16'd%d}; // sw\n", reg1, reg2, imd);
}

void jal_bin(char *read, int i) {
    fprintf(code_bin, "mem[%d] = {6'd14, ", linhaAtual);

    i = imediato(read, i);
    fprintf(code_bin, "26'd%d}; // jal\n", atoi(reg));
}

void bne_bin(char *read, int i) {
    fprintf(code_bin, "mem[%d] = {6'd16, ", linhaAtual);

    i = registrador(read, i);
    fprintf(code_bin, "5'd%d, ", atoi(reg));

    i = registrador(read, i);
    fprintf(code_bin, "5'd%d, ", atoi(reg));
    
    i = imediato(read, i);
    fprintf(code_bin, "16'd%d}; // bne\n", atoi(reg));
}


void beq_bin(char *read, int i) {
    fprintf(code_bin, "mem[%d] = {6'd15, ", linhaAtual);

    i = registrador(read, i);
    fprintf(code_bin, "5'd%d, ", atoi(reg));

    i = registrador(read, i);
    fprintf(code_bin, "5'd%d, ", atoi(reg));
    
    i = imediato(read, i);
    fprintf(code_bin, "16'd%d}; // beq\n", atoi(reg));
}

void bge_bin(char *read, int i) {
    fprintf(code_bin, "mem[%d] = {6'd19, ", linhaAtual);

    i = registrador(read, i);
    fprintf(code_bin, "5'd%d, ", atoi(reg));

    i = registrador(read, i);
    fprintf(code_bin, "5'd%d, ", atoi(reg));
    
    i = imediato(read, i);
    fprintf(code_bin, "16'd%d}; // bge\n", atoi(reg));
}

void bgt_bin(char *read, int i) {
    fprintf(code_bin, "mem[%d] = {6'd17, ", linhaAtual);

    i = registrador(read, i);
    fprintf(code_bin, "5'd%d, ", atoi(reg));

    i = registrador(read, i);
    fprintf(code_bin, "5'd%d, ", atoi(reg));
    
    i = imediato(read, i);
    fprintf(code_bin, "16'd%d}; // bgt\n", atoi(reg));
}

void ble_bin(char *read, int i) {
    fprintf(code_bin, "mem[%d] = {6'd20, ", linhaAtual);

    i = registrador(read, i);
    fprintf(code_bin, "5'd%d, ", atoi(reg));

    i = registrador(read, i);
    fprintf(code_bin, "5'd%d, ", atoi(reg));
    
    i = imediato(read, i);
    fprintf(code_bin, "16'd%d}; // ble\n", atoi(reg));
}

void blt_bin(char *read, int i) {
    fprintf(code_bin, "mem[%d] = {6'd18, ", linhaAtual);

    i = registrador(read, i);
    fprintf(code_bin, "5'd%d, ", atoi(reg));

    i = registrador(read, i);
    fprintf(code_bin, "5'd%d, ", atoi(reg));
    
    i = imediato(read, i);
    fprintf(code_bin, "16'd%d}; // blt\n", atoi(reg));
}

void add_bin(char *read, int i) {
    fprintf(code_bin, "mem[%d] = {6'd1, ", linhaAtual);
    i = registrador(read, i);
    fprintf(code_bin, "5'd%d, ", atoi(reg));

    i = registrador(read, i);
    fprintf(code_bin, "5'd%d, ", atoi(reg));
    
    i = registrador(read, i);
    fprintf(code_bin, "5'd%d, 5'd0, 6'd0}; // add\n", atoi(reg));
}

void sub_bin(char *read, int i) {
    fprintf(code_bin, "mem[%d] = {6'd2, ", linhaAtual);
    i = registrador(read, i);
    fprintf(code_bin, "5'd%d, ", atoi(reg));

    i = registrador(read, i);
    fprintf(code_bin, "5'd%d, ", atoi(reg));
    
    i = registrador(read, i);
    fprintf(code_bin, "5'd%d, 5'd0, 6'd0}; // sub\n", atoi(reg));
}

void mult_bin(char *read, int i) {
    fprintf(code_bin, "mem[%d] = {6'd5, ", linhaAtual);
    i = registrador(read, i);
    fprintf(code_bin, "5'd%d, ", atoi(reg));

    i = registrador(read, i);
    fprintf(code_bin, "5'd%d, ", atoi(reg));
    
    i = registrador(read, i);
    fprintf(code_bin, "5'd%d, 5'd0, 6'd0}; // mult\n", atoi(reg));
}

void div_bin(char *read, int i) {
    fprintf(code_bin, "mem[%d] = {6'd6, ", linhaAtual);
    i = registrador(read, i);
    fprintf(code_bin, "5'd%d, ", atoi(reg));

    i = registrador(read, i);
    fprintf(code_bin, "5'd%d, ", atoi(reg));
    
    i = registrador(read, i);
    fprintf(code_bin, "5'd%d, 5'd0, 6'd0}; // div\n", atoi(reg));
}

void instrucaoBin(char *read) {
    char instrucao[15];
    int j = 0, i;
    for (i = 0; read[i] != ' ' && i < 15; i++)  //pega a instrucao do assembly
    {
        instrucao[j] = read[i];
        j++;
    }
    i++;
    instrucao[j] = '\0';

    if (strcmp(instrucao, "addi") == 0) 
    {
        addi_bin(read, i);
    } 
    else if (strcmp(instrucao, "jump") == 0)
    {
        jump_bin(read, i);
    }
    else if (strcmp(instrucao, "lw") == 0)
    {
        load_bin(read, i);
    }
    else if (strcmp(instrucao, "sw") == 0)
    {
        store_bin(read, i);
    }
    else if (strcmp(instrucao, "jal") == 0)
    {
        jal_bin(read, i);
    }
    else if (strcmp(instrucao, "bne") == 0)
    {
        bne_bin(read, i);
    }
    else if (strcmp(instrucao, "beq") == 0)
    {
        beq_bin(read, i);
    }
    else if (strcmp(instrucao, "bge") == 0)
    {
        bge_bin(read, i);
    }
    else if (strcmp(instrucao, "bgt") == 0)
    {
        bgt_bin(read, i);
    }
    else if (strcmp(instrucao, "ble") == 0)
    {
        ble_bin(read, i);
    }
    else if (strcmp(instrucao, "blt") == 0)
    {
        blt_bin(read, i);
    }
    else if (strcmp(instrucao, "add") == 0)
    {
        add_bin(read, i);
    }
    else if (strcmp(instrucao, "sub") == 0)
    {
        sub_bin(read, i);
    }
    else if (strcmp(instrucao, "div") == 0)
    {
        div_bin(read, i);
    }
    else if (strcmp(instrucao, "mult") == 0)
    {
        mult_bin(read, i);
    }
    else if (strcmp(instrucao, "jr") == 0)
    {
        jr_bin(read, i);
    }
}

void binarioGen() {
    char read[51];
    fprintf(code_bin, "mem[%d] = {6'd13,26'd%d}; //jump\n", linhaAtual, linhaMain); //começa pulando para a main
    linhaAtual++;
    while(fgets(read, 50, code_ass) != NULL) {
        instrucaoBin(read);
        linhaAtual++;
    }
}