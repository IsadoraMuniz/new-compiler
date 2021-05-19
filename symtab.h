/****************************************************/
/* 					                                */
/* Tabela de Simbolos                               */
/* Daiana Santos e Isadora Muniz                    */
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
     int memloc ; /* memory location for variable */
     struct BucketListRec * next;
   } * BucketList;

BucketList hashTable[SIZE];
/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */

int hash ( char * key );

 void st_insert( char * name, int lineno, int loc, char* escopo, dataTypes Dtype, IDTypes IType );

/* Function st_lookup returns the memory 
 * location of a variable or -1 if not found
 */
int st_lookup (char * name);


/* Procedure printSymTab prints a formatted 
 * listing of the symbol table contents 
 * to the listing file
 */
void printSymTab(FILE * listing);

void busca_main ();
dataTypes getFunType(char* nome);

#endif
