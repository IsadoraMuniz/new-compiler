/****************************************************/
/* 					                                */
/* Analisador Semantico                             */
/* Daiana Santos e Isadora Muniz                    */
/****************************************************/

#include "globals.h"
#include "symtab.h"
#include "analyze.h"

// static void typeError(TreeNode * t, char * message)
// { fprintf(listing,"Erro de tipo em %s na linha %d: %s\n",t->attr.name, t->lineno,message);
//   Error = TRUE;
// }

/* counter for variable memory locations */
static int location = 0;
char* escopo = "global";

void atualizaEscopo(TreeNode * t)
{
  if (t->child[0] != NULL && t->child[0]->kind.exp == functionK) escopo = t->child[0]->attr.name;
}

/* Procedure traverse is a generic recursive 
 * syntax tree traversal routine:
 * it applies preProc in preorder and postProc 
 * in postorder to tree pointed to by t
 */
static void traverse( TreeNode * t,
               void (* preProc) (TreeNode *),
               void (* postProc) (TreeNode *) )
{ 

	if (t != NULL)
 	{ 
		atualizaEscopo(t);
    preProc(t);
    	{ 
	  		int i;
	        for (i=0; i < MAXCHILDREN; i++)
        		traverse(t->child[i],preProc,postProc);
    	}
    	if(t->child[0]!= NULL && t->child[0]->kind.exp == functionK) escopo = "global";
      postProc(t);
    	traverse(t->sibling,preProc,postProc);
  	}
}

/* nullProc is a do-nothing procedure to 
 * generate preorder-only or postorder-only
 * traversals from traverse
 */
static void nullProc(TreeNode * t)
{ 
	if (t==NULL) 
		return;
  	else 
		return;
}

/* Procedure insertNode inserts 
 * identifiers stored in t into 
 * the symbol table 
 */
static void insertNode( TreeNode * t)
{ 
	
	switch (t->nodekind)
  { 
		case statementK:
      if(t->kind.stmt == assignK)
      {
          if (st_lookup(t->child[0]->attr.name) == -1){
          /* não encontrado na tabela, cariavel não declarada */
            fprintf(listing,"Erro: A variavel %s não foi declarada. [%d]\n", t->child[0]->attr.name, t->lineno);
            Error = TRUE;
          }
          else
          /* encontrada na tabela, verificar escopo e adicionar linha */
            st_insert(t->child[0]->attr.name,t->lineno,0,escopo,INTTYPE,VAR);
          t->child[0]->add = 1;
      }
      break;

      case expressionK:
        switch (t->kind.exp)
        { 
          case typeK:

            if(t->child[0] != NULL){
               
              switch (t->child[0]->kind.exp)
              {
                case variableK:
                  if (st_lookup(t->attr.name) == -1){
                /* não encontrado na tabela, inserir*/
                  st_insert(t->child[0]->attr.name,t->lineno,location++, escopo,INTTYPE, VAR);
                  
                }
                else
                /* encontrado na tabela, verificar escopo */
                  st_insert(t->child[0]->attr.name,t->lineno,0, escopo,INTTYPE, VAR);
                break;

                case functionK:
                  if (st_lookup(t->attr.name) == -1){
                /* não encontrado na tabela, inserir*/
                  st_insert(t->child[0]->attr.name,t->child[0]->lineno,location++, "global",t->child[0]->type,FUN);
              }
                else
                /* encontrado na tabela, verificar escopo */
                  fprintf(listing,"Erro: Multiplas declarações da função %s. [%d]\n", t->child[0]->attr.name, t->lineno);
              
              break;
              default:
              break;
              }
          }
          break;

          case paramK:
          st_insert(t->attr.name,t->lineno,location++, escopo,INTTYPE, VAR);
          break;
        case idK:
          if(t->add != 1){
            if (st_lookup(t->attr.name) == -1){
              fprintf(listing,"Erro: A variavel %s não foi declarada. [%d]\n", t->attr.name, t->lineno);
              Error = TRUE;
            }
            else {
              st_insert(t->attr.name,t->lineno,0, escopo,INTTYPE,FUN);
            }
          }
          break;
        case activationK:
          if (st_lookup(t->attr.name) == -1 && strcmp(t->attr.name, "output")!=0 && strcmp(t->attr.name,"input")!=0){
            fprintf(listing,"Erro: A função %s não foi declarada. [%d]\n", t->attr.name, t->lineno);
            Error = TRUE;
          }
          else {
            st_insert(t->attr.name,t->lineno,0, escopo,0,FUN);
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


			// case callK:

			// 	if (st_lookup(t->attr.name, t->attr.scope) == -1 && st_lookup(t->attr.name, "global") == -1 )
      //       		typeError(t,"Erro semantico: Chamada Invalida. Nao foi declarada.");	

      //     		else
      //       		st_insert(t->attr.name,t->lineno,location++, t->attr.scope, "call", "-"); 
		  /*case idK:
			if (st_lookup(t->attr.name, t->attr.scope) == -1 && st_lookup(t->attr.name, "global") == -1)
				 typeError(t,"Erro semantico : variavel nao foi declarado");
			else
            	st_insert(t->attr.name,t->lineno,0, t->attr.scope, "variable", "integer");	
	      break;
	      case vectorK:
	      	if (st_lookup(t->attr.name, t->attr.scope) == -1 && st_lookup(t->attr.name, "global") == -1)
				 typeError(t,"Erro semantico : nao foi declarado");
			else
            	st_insert(t->attr.name,t->lineno,0, t->attr.scope, "vector", "integer");	
		  break;
		  case vectorIdK:
		  	if (st_lookup(t->attr.name, t->attr.scope) == -1 && st_lookup(t->attr.name, "global") == -1)
				 typeError(t,"Erro semantico : nao foi declarado");
			else
            	st_insert(t->attr.name,t->lineno,0, t->attr.scope, "vector index", "integer");	
		  case typeK:
          break;
        default:
        break;
      }
   			  case returnK:
          
          break;

            case paramK:
                st_insert(t->attr.name,t->lineno,location++, t->attr.scope,"variable", "integer");
            break;

      default:
      break;
      
      break;
     
  }
}*/

void buildSymtab(TreeNode * syntaxTree)
{ 
    
  traverse(syntaxTree,insertNode,nullProc);
  busca_main();
  typeCheck(syntaxTree); 
  
  if (TraceAnalyze && !Error)
  { 
    fprintf(listing,"\nTabela de simbolos:\n\n");
    printSymTab(listing);
  }
}

static void typeError(TreeNode * t, char * message)
{ fprintf(listing,"Erro de tipo na linha %d: %s\n",t->lineno,message);
  Error = TRUE;
}

void checkNode(TreeNode * t)
{

  switch (t->nodekind)
  { case expressionK:
      switch (t->kind.exp)
      { case operationK:
          if (((t->child[0]->kind.exp == activationK) &&( getFunType(t->child[0]->attr.name)) == VOIDTYPE) ||
              ((t->child[1]->kind.exp == activationK) && (getFunType(t->child[1]->attr.name) == VOIDTYPE)))
                typeError(t->child[0],"Ativação de função do tipo void na expressão");
          break;

        default:
          break;
      }
      break;
    case statementK:
      switch (t->kind.stmt)
      {
        case assignK:
          
          if (t->child[1]->kind.exp == activationK && getFunType(t->child[1]->attr.name) == VOIDTYPE)
            typeError(t->child[0],"Função com retorno void não pode ser atribuido a uma variavel");
          break;

        default:
          break;
      }
      break;
    default:
      break;

  }
}

// static void checkNode(TreeNode * t)
// { switch (t->nodekind)
//   { 
//    case expressionK:
//       switch (t->kind.exp)
//       { 
//         case operationK:
//         break;
//         default:
//         break;
//       }
//       break;
//     case statementK:
//       switch (t->kind.stmt)
//       { 
//         case ifK:
//           if (t->child[0]->type == integerK && t->child[1]->type == integerK)
//            typeError(t->child[0],"se teste nao eh Boolean");
//         break;
//         case assignK:
//           if (t->child[0]->type == voidK || t->child[1]->type == voidK)
//             typeError(t->child[0],"atribuicao de valor nao-inteiro");
// 		  else if(t->child[1]->kind.stmt == callK)
// 		  {
// 		    if(strcmp(st_lookup_type(t->child[1]->attr.name, "global"), "void")==0)
// 				typeError(t->child[1],"atribuicao de returno vazio");
// 			}
//         break;
//         default:
//         break;
//       }
//     break;
//     default:
//     break;
//   }
// }

void typeCheck(TreeNode * syntaxTree)
{ 
    traverse(syntaxTree,nullProc,checkNode);
}
