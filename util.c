/****************************************************/
/* 					                                        */
/*                Arvore Sintatica                  */
/*                 Isadora Muniz                    */
/****************************************************/

#include "globals.h"
#include "util.h"

void printToken( TokenType token, const char* tokenString )
{ 
  switch (token)
  { 
	case IF: fprintf(listing, "Palavra Reservada: %s\n",tokenString); break;
    case ELSE: fprintf(listing, "Palavra Reservada: %s\n",tokenString); break;
    case INT: fprintf(listing, "Palavra Reservada: %s\n",tokenString); break;
    case RETURN: fprintf(listing, "Palavra Reservada: %s\n",tokenString); break;
    case VOID: fprintf(listing, "Palavra Reservada: %s\n",tokenString); break;
    case WHILE: fprintf(listing, "Palavra Reservada: %s\n",tokenString); break;
    case IGL: fprintf(listing,"=\n"); break;
    case MEN: fprintf(listing,"<\n"); break;
    case IGLIGL: fprintf(listing,"==\n"); break;
    case MAI: fprintf(listing,">\n"); break; 
    case MEIGL: fprintf(listing, "<=\n"); break;
    case MAIGL: fprintf(listing, ">=\n"); break; 
    case DIF: fprintf(listing, "!=\n"); break;
    case ACO: fprintf(listing, "[\n"); break;
    case FCO: fprintf(listing, "]\n"); break;
    case ACH: fprintf(listing, "{\n"); break;
    case FCH: fprintf(listing, "}\n"); break;	
    case APR: fprintf(listing,"(\n"); break;
    case FPR: fprintf(listing,")\n"); break;
    case PEV: fprintf(listing,";\n"); break;
    case VIR: fprintf(listing,",\n"); break;
    case SOM: fprintf(listing,"+\n"); break;
    case SUBT: fprintf(listing,"-\n"); break;
    case MUL: fprintf(listing,"*\n"); break;
    case DIVI: fprintf(listing,"/\n"); break;
    case ENDFILE: fprintf(listing,"EOF\n"); break; 
    case NUM:
      fprintf(listing, "Numero: %s\n",tokenString);
      break;
    case ID:
      fprintf(listing, "Identificador: %s\n",tokenString);
      break;
    case ERR:
      fprintf(listing, "Erro: %s\n",tokenString);
      break;
    default: 
      fprintf(listing,"Token desconhecido: %d\n",token);
  }
}

TreeNode * newStmtNode(StatementKind kind)
{ 
    TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
    int i;
    if (t==NULL)
        fprintf(listing,"Erro de acesso de memoria na linha %d\n",lineno);
    else 
    {
        for (i=0;i<MAXCHILDREN;i++) 
            t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = statementK;
        t->kind.stmt = kind;
        t->lineno = lineno;
    }
    return t;
}

TreeNode * newExpNode(ExpressionIdentifier kind)
{ 
    TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
    int i;
    if (t==NULL)
        fprintf(listing,"Erro de acesso de memoria na linha %d\n",lineno);
    else 
    {
        for (i=0;i<MAXCHILDREN;i++) 
            t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = expressionK;
        t->kind.exp = kind;
        t->lineno = lineno;
        t->type = VOID;
    }
    return t;
}

char * copyString(char * s)
{ 
    int n;
    char * t;
    if (s==NULL) 
        return NULL;
    n = strlen(s)+1;
    t = malloc(n);
    if (t==NULL)
        fprintf(listing,"Erro de acesso de memoria na linha %d\n",lineno);
    else 
        strcpy(t,s);
    return t;
}

static int indentno = 0;


#define INDENT indentno+=2
#define UNINDENT indentno-=2

static void printSpaces(void)
{ 
  int i;
    for (i=0;i<indentno;i++)
      fprintf(listing," ");
}

void printTree( TreeNode * tree )
{ int i;
  INDENT;
  while (tree != NULL) {
    printSpaces();
    if (tree->nodekind==statementK)
    { switch (tree->kind.stmt) {
        case ifK:
          fprintf(listing,"If\n");
          break;
        case assignK:
          fprintf(listing,"Atribuicao\n");
          break;
        case whileK:
         fprintf(listing,"While\n");
        break;
        case returnK:
 	       fprintf(listing, "Return\n");
 	      break;
        default:
          fprintf(listing,"Tipo StatementNode desconhecido\n");
          break;
      }
    }

  else if(tree->nodekind== expressionK)
  { switch (tree->kind.exp){
    case operationK:
      fprintf(listing,"Op: ");
      printToken(tree->attr.op,"\0");
    break;
    case variableK:
    fprintf(listing,"Variavel: %s\n", tree->attr.name);
    break;
    case functionK:
    fprintf(listing,"Funcao: %s\n", tree->attr.name);
      break;
    case constantK:
    fprintf(listing,"Constante: %d\n", tree->attr.val);
    break;      
    case idK:
    fprintf(listing,"Id: %s\n",tree->attr.name);
    break;
    case activationK:
    fprintf(listing,"Ativação: %s\n",tree->attr.name);
    break;
    case typeK:
      fprintf(listing,"Tipo: %s\n",tree->attr.name);
      break;
    case paramK:
    fprintf(listing,"Var: %s\n",tree->attr.name);
    break; 
    case vectorK:
    fprintf(listing,"Vetor: %s\n",tree->attr.name);
    break;
    case vectorIdK:
    fprintf(listing,"Indice [%d]\n",tree->attr.val);
    break;             
    default:
    fprintf(listing,"Tipo ExpNode desconhecido\n");
    break;
    }
  }
    else fprintf(listing,"Tipo de no desconhecido\n");
    for (i=0;i<MAXCHILDREN;i++)
         printTree(tree->child[i]);
    tree = tree->sibling;
  }
  UNINDENT;
}
