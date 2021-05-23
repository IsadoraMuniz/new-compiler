/****************************************************/
/*               Tabela de Simbolos                 */
/*                 Isadora Muniz                    */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"


#define SIZE 211

#define SHIFT 4

int hash (char* name)
{ 
    int temp = 0;
    int i = 0;
    while (name[i] != '\0')
    { 
        temp = ((temp << SHIFT) + name[i]) % SIZE;
        ++i;
    }
    i = 0;
   return temp;
}


void st_insert(char * name, int lineno, int op, char* escopo, dataTypes DType, IDTypes IType)
{ 
 
    int h = hash(name);
    BucketList l =  hashTable[h];
    
    while ((l != NULL) && ((strcmp(name,l->name) != 0))){
        l = l->next;
    } 
      
    if (l == NULL || (op != 0 && l->escopo != escopo && l->IType != FUN)) 
    { 
        l = (BucketList) malloc(sizeof(struct BucketListRec));
        l->name = name;
        l->lines = (LineList) malloc(sizeof(struct LineListRec));
        l->lines->lineno = lineno;
        l->memloc = op;
        l->IType = IType;
        l->Dtype = DType;
        l->escopo = escopo;
        l->lines->next = NULL;
        l->next = hashTable[h];
        hashTable[h] = l;
    }
    else if( l->IType == FUN  && IType == VAR){
    fprintf(listing,"Erro: Nome da variavel %s já utilizada como nome de função.[%d]\n",name, lineno);
    Error = TRUE;
  }
  else if( l->escopo == escopo && op != 0)
  {
    fprintf(listing,"Erro: Variavel %s já declarada neste escopo.[%d]\n",name, lineno);
    Error = TRUE;
  }
  else if(l->escopo != escopo && (strcmp(l->escopo,"global") != 0) ){
    
    while ((l != NULL)){
      if((strcmp(l->escopo, "global")==0)&& ((strcmp( name,l->name) == 0))){
        LineList t = l->lines;
        while (t->next != NULL) t = t->next;
        t->next = (LineList) malloc(sizeof(struct LineListRec));
        t->next->lineno = lineno;
        t->next->next = NULL;
        break;
      }
      l = l->next;
    }
    if(l == NULL){
      fprintf(listing,"Erro: Variavel %s não declarada neste escopo.[%d]\n",name, lineno);
      Error = TRUE;
    }
  }
  else if(op == 0)
  {
    LineList t = l->lines;
    while (t->next != NULL) t = t->next;
    t->next = (LineList) malloc(sizeof(struct LineListRec));
    t->next->lineno = lineno;
    t->next->next = NULL;
  }
} 

int st_lookup (char* name)
{ 
  int h = hash(name);	
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0))
        l = l->next;
  if (l == NULL) 
      return -1;
  else 
      return l->memloc;
}

void busca_main ()
{
  int h = hash("main");
  BucketList l =  hashTable[h];
  while ((l != NULL) && ((strcmp("main",l->name) != 0 || l->IType == VAR)))
    l = l->next;
  if (l == NULL) {
    fprintf(listing,"Erro: Função main não declarada\n");
    Error = TRUE;
  }

}
dataTypes getFunType(char* nome){
  int h = hash(nome);
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp(nome,l->name) != 0))
    l = l->next;

  if (l == NULL) return -1;
  else return l->Dtype;
}

void printSymTab(FILE * listing)
{ int i;
  fprintf(listing," Nome      Escopo      TipoID        TipoDado     Linha Numeros\n");
  fprintf(listing,"------    ---------   ----------    ----------    ------------\n");
  for (i=0;i<SIZE;++i)
  { if (hashTable[i] != NULL)
    { BucketList l = hashTable[i];
      while (l != NULL)
      { LineList t = l->lines;
        fprintf(listing,"%-10s  ",l->name);
		    fprintf(listing,"%-12s  ",l->escopo);

        char* id, *data;
        if(l->IType == VAR){
          id = "VAR";
        }
        else if(l->IType == FUN){
          id = "FUN";
        }
        if(l->Dtype == INTTYPE){
          data = "INT";
        }
        else if(l->Dtype == VOIDTYPE){
          data = "VOID";
        }
        fprintf(listing,"%-10s  ",id);
        fprintf(listing,"%-10s  ",data);
       
        while (t != NULL)
        { fprintf(listing,"%4d ",t->lineno);
          t = t->next;
        }
        fprintf(listing,"\n");
        l = l->next;
      }
    }
  }
} 
