/****************************************************/
/*                      Main                        */
/*                  Isadora Muniz                   */
/****************************************************/

#include "globals.h"

#define NO_PARSE FALSE

#define NO_ANALYZE FALSE

#define NO_CODE FALSE

#include "util.h"
#if NO_PARSE
#else
#include "parse.h"
#if !NO_ANALYZE
#include "analyze.h"
#if !NO_CODE
#include "cgen.h"
#endif
#endif
#endif


int lineno = 0;
FILE * source;
FILE * listing;
FILE * code;

int EchoSource = FALSE;
int TraceScan = TRUE;
int TraceParse = TRUE;
int TraceAnalyze = TRUE;
int TraceCode = FALSE;

int Error = FALSE;

int main( int argc, char * argv[] )
{ TreeNode * syntaxTree;
  char pgm[120];
  if (argc != 2)
    { fprintf(stderr,"usage: %s <filename>\n",argv[0]);
      exit(1);
    }
  strcpy(pgm,argv[1]) ;
  if (strchr (pgm, '.') == NULL)
     strcat(pgm,".cms");
  source = fopen(pgm,"r");
  if (source==NULL)
  { fprintf(stderr,"Arquivo %s nao encontrado\n",pgm);
    exit(1);
  }
  listing = stdout; /* send listing to screen */
  fprintf(listing,"\n\n\n           Compilador C menos: %s\n",pgm);
#if NO_PARSE
  while (getToken()!=ENDFILE);
#else
  syntaxTree = parse();
  if (TraceParse) {
    fprintf(listing,"\n\n\n           Arvore Sintatica:\n");
    printTree(syntaxTree);
  }
#if !NO_ANALYZE

  if (! Error){
    fprintf(listing,"\n\n\n           Construindo Tabela de Simbolos...\n");
    buildSymtab(syntaxTree);
  } 
    
#if !NO_CODE
  if (! Error)
  { 
    fprintf(listing,"\n\n\n           Quadruplas Gerador de Codigo:\n\n");
    generate_intermediate_code(syntaxTree);
  }
#endif
#endif
#endif
  fclose(source);
  return 0;
}
