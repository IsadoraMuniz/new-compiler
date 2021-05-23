/****************************************************/
/* 					                                        */
/*               Lib Tabela de Simbolos             */
/*                 Isadora Muniz                    */
/****************************************************/

#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "globals.h"

#define SIZE 211

typedef struct LineListRec
   { int lineno;
     struct LineListRec * next;
   } * LineList;

typedef struct BucketListRec
   { char * name;
     dataTypes Dtype;
     IDTypes IType;
     char* escopo;
     LineList lines;
     int memloc ; 
     struct BucketListRec * next;
   } * BucketList;

BucketList hashTable[SIZE];

int hash ( char * key );

 void st_insert( char * name, int lineno, int loc, char* escopo, dataTypes Dtype, IDTypes IType );

int st_lookup (char * name);

void printSymTab(FILE * listing);

void busca_main ();

dataTypes getFunType(char* nome);

#endif
