/****************************************************/
/* 					                                */
/* Tabela de Simbolos                               */
/* Daiana Santos e Isadora Muniz                    */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

/* SIZE is the size of the hash table */
#define SIZE 211

/* SHIFT is the power of two used as multiplier
   in hash function  */
#define SHIFT 4

/* the hash function */
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


void st_insert(char * name, int lineno, int loc, char* escopo, dataTypes Dtype, IDTypes IType)
{ 
 
    int h = hash(name);
    BucketList l =  hashTable[h];
    while ((l != NULL) && (strcmp(name,l->name) != 0)){
        l = l->next;
    } 
           
    if (l == NULL || (op != 0 && l->escopo != escopo && l->IType != FUN) /* variable not yet in table */
    { 
        l = (BucketList) malloc(sizeof(struct BucketListRec));
        l->name = name;
        l->lines = (LineList) malloc(sizeof(struct LineListRec));
        l->lines->lineno = lineno;
        l->memloc = loc;
        l->lines->next = NULL;
        l->scope = scope;
        l->typeID = typeID;
        l->typeData = typeData;
        l->next = hashTable[h];
        hashTable[h] = l; 
    }
    else
    {
        LineList t = l->lines;
        while (t->next != NULL) 
            t = t->next;
        t->next = (LineList) malloc(sizeof(struct LineListRec));
        t->next->lineno = lineno;
        t->next->next = NULL;  
    }
} /* st_insert */

/* Function st_lookup returns the memory 
 * location of a variable or -1 if not found
 */
int st_lookup (char* name)
{ 
  int h = hash(name);	
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0) && (strcmp(scope,l->scope) != 0))
        l = l->next;
  if (l == NULL) 
      return -1;
  else 
      return l->memloc;
}


char* st_lookup_type (char* name, char* scope)
{ 
  int h = hash(name, scope);	
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0) && (strcmp(scope,l->scope) != 0))
        l = l->next;
  if (l == NULL) 
      return "null";
  else 
      return l->typeData;
}


/* Procedure printSymTab prints a formatted 
 * listing of the symbol table contents 
 * to the listing file
 */
void printSymTab(FILE * listing)
{ int i;
  fprintf(listing,"Localizacao        Nome       Escopo            TipoID          TipoDado         Linha Numeros\n");
  fprintf(listing,"-------------    --------   ------------      ------------    ------------     -----------------\n");
  for (i=0;i<SIZE;++i)
  { if (hashTable[i] != NULL)
    { BucketList l = hashTable[i];
      while (l != NULL)
      { LineList t = l->lines;
        fprintf(listing,"%-8d ",l->memloc);
        fprintf(listing,"\t%-14s  ",l->name);
		fprintf(listing,"%-14s  ",l->scope);
        fprintf(listing,"%-14s  ",l->typeID);
		fprintf(listing,"%-14s  ",l->typeData);	
        while (t != NULL)
        { fprintf(listing,"%4d ",t->lineno);
          t = t->next;
        }
        fprintf(listing,"\n");
        l = l->next;
      }
    }
  }
} /* imprimeTabelaSimbolos */
