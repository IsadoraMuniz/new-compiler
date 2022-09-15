#include "globals.h"
#include "symtab.h"
#include "analyze.h"
#include <string.h>

static int indice = 0;

char *escopoAtual;

static void traverse(Node * no, void (* preProc) (Node *), void (* postProc) (Node *)) { 
	if (no != NULL) { 
        preProc(no);
        { 
            int i;
            for (i=0; i < MAXFILHOS; i++){
                traverse(no->filho[i], preProc, postProc);
            }
        }
        postProc(no);{
            traverse(no->irmao, preProc, postProc);
        }
    }
}

static void nullProc(Node * no) { 
	if (no == NULL) return;
    else return;
}

static void erro_tipo(Node * no, char * msg) { 
  	fprintf(listing, "Erro de tipo na linha %d: %s\n", no->lineno, msg);
  	Error = TRUE;
}

static void erro_declaracao(Node * no, char * msg) { 
  	fprintf(listing, "Erro de declaração na linha %d: %s: %s\n", no->lineno, msg, no->atr.nome);
  	Error = TRUE;
}


static void insertNode( Node * no) { 
  Node *aux;
  int vet = 0;
  switch (no->tipoNo)
  {
    case declaration:
      	switch (no->tipo.dcl) { 
          	case tvariavel:
              	if (busca_indice(no->atr.nome, no->atr.escopo, "variável") == -1) {
                	if (no->typeExp == tvoid) {
                  		erro_declaracao(no, "Variavel declarada como tipo void");
                	}
					if (busca_escopo(no->atr.nome, no->atr.escopo) == -1) { //escopo diferente
                  		if (busca_escopo(no->atr.nome, "global") == 1) {
                    		erro_declaracao(no, "Nome de variavel já usado no escopo global");
                  		} else {
                    		insere_tabela(indice++, no->atr.nome, no->atr.escopo, "variável", no->typeExp, 0, 0, no->lineno);
                  		}
					}
                	else
                		insere_tabela(indice++, no->atr.nome, no->atr.escopo, "variável", no->typeExp, 0, 0, no->lineno); 
              	}
              	else {
                	if (busca_escopo(no->atr.nome, no->atr.escopo) == -1) { //escopo diferente
                  		if (busca_escopo(no->atr.nome, "global") == 1) {
                    		erro_declaracao(no, "Nome de variavel já usado no escopo global");
                  		} else {
                    		insere_tabela(indice++, no->atr.nome, no->atr.escopo, "variável", no->typeExp, 0, 0, no->lineno);
                  		}	
					} else {
                		erro_declaracao(no, "Variável já foi declarada");
                	}
              	}
          	break;

            case tvector:
              if (busca_indice(no->atr.nome, no->atr.escopo, "variável") == -1){
                if (no->typeExp == tvoid) {
                  erro_declaracao(no, "Variavel declarada como tipo void");
                }
                insere_tabela(indice, no->atr.nome, no->atr.escopo, "variável", no->typeExp, 0, 1, no->lineno); 
                indice = indice + no->atr.len; //pois é vetor
              	}
              else {
                if (busca_escopo(no->atr.nome, no->atr.escopo) == -1) { //escopo diferente
                  //verifico se está no global
                  if (busca_escopo(no->atr.nome, "global") == 1) {
                    erro_declaracao(no, "Nome de variavel já usado no escopo global");
                  } else {
                    insere_tabela(indice, no->atr.nome, no->atr.escopo, "variável", no->typeExp, 0, 1, no->lineno);
                    indice = indice + no->atr.len; //pois é vetor
                  }
                } else {
                  erro_declaracao(no, "Variável já foi declarada");
                }
			  }
            break;

            case tparam:
              if (busca_indice(no->atr.nome, no->atr.escopo, "variável") == -1){
                if (no->typeExp == tvoid) {
                  erro_declaracao(no, "Variavel declarada como tipo void");
                }
                insere_tabela(indice++, no->atr.nome, no->atr.escopo, "variável", no->typeExp, 1, -1, no->lineno);
              }
              else {
                if (busca_escopo(no->atr.nome, no->atr.escopo) == -1) { //escopo diferente
                  if (busca_escopo(no->atr.nome, "global") == 1) {
                    erro_declaracao(no, "Nome de variavel já usado no escopo global");
                  } else {
					if (strcmp(eh_vetor(no->atr.nome, no->atr.escopo), "sim")) {
						no->tipo.exp = vector;
						vet = 1;
					}
					else if (strcmp(eh_vetor(no->atr.nome, "global"), "sim")) {
						no->tipo.exp = vector;
						vet = 1;
					}
					printf("v: %d", vet);
                    insere_tabela(indice++, no->atr.nome, no->atr.escopo, "variável", no->typeExp, 1, vet, no->lineno);
					vet = 0;
                  }
                } else {
                  erro_declaracao(no, "Variável já foi declarada");
                }
				}
            break;

          	case tfunction:
            //escopo nas declarações
            	aux = no->filho[1];
				while(aux != NULL) {
					aux->atr.escopo = no->atr.nome;
					aux = aux->irmao;
				}
            	//escopo no parametro
            	aux = no->filho[0];
				while(aux != NULL) {
					if (aux->tipoNo == expression) { //EXPRESSÃO
						if (aux->tipo.exp == type) //INT , VOID
							aux->atr.escopo = no->atr.nome;
						}
					aux = aux->irmao;
            	}
            
				if (busca_indice(no->atr.nome, no->atr.escopo, "função") == -1)
					insere_tabela(0, no->atr.nome, no->atr.escopo, "função", no->typeExp, -1, -1, no->lineno);
				else
					erro_declaracao(no, "Nome já foi utilizado");
			break;

          	case tcall:
				aux = no->filho[0];
				while(aux != NULL) {
					aux->atr.escopo = no->atr.escopo;
					aux = aux->irmao;
				}
				//variavel que acha o item
				if(strcmp(busca_tipo_id(no->atr.nome, "global"), "null") == 0)
					erro_declaracao(no, "A função chamada não existe");

				insere_tabela(0, no->atr.nome, no->atr.escopo, "call", -1 , -1, -1, no->lineno);
            break;
			case twhile:
				//tratar escopo dentro do while
				aux = no->filho[0];
				while(aux != NULL) {
					aux->atr.escopo = no->atr.escopo;
					aux = aux->irmao;
				}
				aux = no->filho[1];
				while(aux != NULL) {
					aux->atr.escopo = no->atr.escopo;
					aux = aux->irmao;
				}
			break;

			case tif:
				//tratar escopo dentro do if e do else
				aux = no->filho[0];
				while(aux != NULL) {
					aux->atr.escopo = no->atr.escopo;
					aux = aux->irmao;
				}
				aux = no->filho[1];
				while(aux != NULL){
					aux->atr.escopo = no->atr.escopo;
					aux = aux->irmao;
				}
				aux = no->filho[2];
				while(aux != NULL){
					aux->atr.escopo = no->atr.escopo;
					aux = aux->irmao;
				}
			break;

			case tigual:
				//tratar escopo dos dois filhos
				aux = no->filho[0];
				while(aux != NULL){
					aux->atr.escopo = no->atr.escopo;
					aux = aux->irmao;
				}
				aux = no->filho[1];
				while(aux != NULL){
					aux->atr.escopo = no->atr.escopo;
					aux = aux->irmao;
				}
			    if (no->filho[1]->tipo.dcl == tcall){
				    if (strcmp(busca_tipo_dado(no->filho[1]->atr.nome, "global"), "int") != 0 && strcmp(busca_tipo_dado(no->filho[1]->atr.nome, "global"), "null") != 0)
						erro_tipo(no->filho[1], "Atribuindo um retorno void");
			    }
				break;
          	case treturn:
            	//tratar escopo do filho
            	aux = no->filho[0];
            	while(aux != NULL) {
              		aux->atr.escopo = no->atr.escopo;
              		aux = aux->irmao;
            	}
            break;

          	default:
            break;
      	}
      	break;
    case expression:
        switch (no->tipo.exp) { 
            case id:
              	if (busca_indice(no->atr.nome, no->atr.escopo, "variável") == -1 && busca_indice(no->atr.nome, "global", "variável") == -1)
               		erro_declaracao(no, "Variavel não foi declarada");
              	else {
					if (no->tipo.exp == 3) {
						vet = 1;
					}
					else {
						vet = 0;
					}
                  	if (busca_escopo(no->atr.nome, "global") == 1){
                		insere_tabela(0, no->atr.nome, "global", "variável", no->typeExp, -1, vet, no->lineno);
                  	} else {
                    	insere_tabela(0, no->atr.nome, no->atr.escopo, "variável", no->typeExp, -1, -1, no->lineno);                    
                    }
				vet = 0;
              	}

            break;

            case type:
            	no->filho[0]->atr.escopo = no->atr.escopo;
              	break;

            case vector:
				aux = no->filho[0];
				while(aux != NULL) {
					aux->atr.escopo = no->atr.escopo;
					aux = aux->irmao;
				}

				if (busca_indice(no->atr.nome, no->atr.escopo, "variável") == -1 && busca_indice(no->atr.nome, "global", "variável") == -1)
               		erro_declaracao(no, "Variavel não foi declarada");
              	else {
					if (busca_escopo(no->atr.nome, "global") == 1){
                    	// tá na tabela, só adiciono as linhas
                		insere_tabela(0, no->atr.nome, "global", "variável", no->typeExp, -1, -1, no->lineno);
                  	} else {
                    	insere_tabela(0, no->atr.nome, no->atr.escopo, "variável", no->typeExp, -1, -1, no->lineno);                    
                    }
              	}
            break;
              
            case op:
              //tratar escopo dos dois filhos
              	aux = no->filho[0];
              	while(aux != NULL) {
            		aux->atr.escopo = no->atr.escopo;
                	aux = aux->irmao;
              	}
              	aux = no->filho[1];
              	while(aux != NULL){
                	aux->atr.escopo = no->atr.escopo;
                	aux = aux->irmao;
              	}
            break;

            default:
            break;
        }
        break;

      	default:
        break;
  	}
}

void constroi_tabela(Node * syntaxTree) { 
  	Node *aux = syntaxTree->irmao;
  	syntaxTree->atr.escopo = "global";
  	while(aux != NULL) {
    	if (aux->tipoNo == expression){
      		if (aux->tipo.exp == type){
        		aux->atr.escopo = "global";
      	}
    	}
   		aux = aux->irmao;
  	}
    
    traverse(syntaxTree, insertNode, nullProc);

    if(busca_indice("main", "global", "função") == -1) {
   		fprintf(listing,"\nA FUNÇÃO MAIN NÃO FOI DECLARADA\n");
   		Error = TRUE; 
   	}
	
	if (TraceAnalyze)
    { 
        fprintf(listing,"\nTabela de Simbolos:\n\n");
        imprime_tabela(listing);
    }
}

static void checagem_no(Node * no) { 

  	if (no->atr.escopo != NULL)
    	escopoAtual = no->atr.escopo;

  	switch (no->tipoNo) { 
    	case expression:
      		switch (no->tipo.exp) { 
        	case op:
          		if ((no->filho[0]->typeExp != tint) || (no->filho[1]->typeExp != tint))
            		erro_tipo(no, "Operador utilizado para variável não inteira");
          		if ((no->atr.op == EHIGUAL) || (no->atr.op == MENORIGUAL) || (no->atr.op == MAIOR) || (no->atr.op == MAIORIGUAL) || (no->atr.op == MENOR) || (no->atr.op == DIFERENTE))
            		no->typeExp = tbool;
          		else
            		no->typeExp = tint;
          	break;

        	case constante:
          		no->typeExp = tint;
          	break;

			case id:
				if (strcmp(busca_tipo_dado(no->atr.nome, escopoAtual), "int") == 0) //se existe nesse escopo como int
					no->typeExp = tint;
				else if (strcmp(busca_tipo_dado(no->atr.nome, "global"), "int") == 0)  //se existe no global como int
					no->typeExp = tint;
				else if (strcmp(busca_tipo_dado(no->atr.nome, escopoAtual), "null") == 0 || strcmp(busca_tipo_dado(no->atr.nome, "global"), "null") == 0) //se existe no escopo atual ou global como outro tipo
					no->typeExp = tvoid;
				else
					erro_tipo(no, "Variável não declarada");
			break;
        	
			case vector:
				if (strcmp(busca_tipo_dado(no->atr.nome, escopoAtual), "int") == 0) //se existe nesse escopo como int
					no->typeExp = tint;
				else if (strcmp(busca_tipo_dado(no->atr.nome, "global"), "int") == 0)  //se existe no global como int
					no->typeExp = tint;
				else if (strcmp(busca_tipo_dado(no->atr.nome, escopoAtual), "null") == 0 || strcmp(busca_tipo_dado(no->atr.nome, "global"), "null") == 0) //se existe no escopo atual ou global como outro tipo
					no->typeExp = tvoid;
				else 
					erro_tipo(no, "Variável não declarada");
          	break;

        	default:
    		break;
    		}
    	break;

		case declaration:
			switch (no->tipo.dcl) { 
				case tif:
					if (no->filho[0]->typeExp == tint)
						erro_tipo(no->filho[0], "A condição não é booleana");
				break;

				case tigual:
					if (no->filho[0]->typeExp != tint)
						erro_tipo(no->filho[0], "Atribuição para um não inteiro");
				break;
				
				default:
				break;
			}
		break;
    
	default:
    break;
  	}
}

void checagem_tipo(Node * syntaxTree) { 
  traverse(syntaxTree, nullProc, checagem_no);
}