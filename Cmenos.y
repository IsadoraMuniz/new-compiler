/****************************************************/
/* 					                                */
/*               Analisador Sintatico               */
/*                 Isadora Muniz                    */
/****************************************************/

%{
	
	#define YYPARSER    
    #include "globals.h"
    #include "util.h"
    #include "scan.h"
    #include "parse.h"

    #define YYSTYPE TreeNode *
    int yyerror(char* message);
    static TreeNode * savedTree;   
    static int yylex(void);

%}

%token IF ELSE WHILE INT VOID RETURN
%token NUM ID
%token IGL IGLIGL DIF MEN MAI MEIGL MAIGL 
%token SOM SUB
%token MUL DIV
%token APR FPR ACO FCO VIR ACH FCH PEV 
%token ERR ENDFILE 
%expect 1 

%%

programa: 			declaracaoLista 
 					{ savedTree = $1; }
                    
	;

declaracaoLista:		declaracaoLista declaracao 
						{ YYSTYPE t = $1;
                            if(t != NULL)
		   	  			    {
                                while(t->sibling != NULL)
                                    t = t->sibling;
                                t->sibling = $2;
                                $$ = $1;
                            }
                            else
                                $$ = $2;
                        }
	|			declaracao
				{ $$ = $1; }
	;
declaracao:		varDeclaracao 
				{$$ = $1;}
	|			funDeclaracao
				{$$ = $1;}
	;
varDeclaracao:	INT identificador PEV
            {	
                $$ = newExpNode(typeK);
                $$->attr.name = "INT";
                $$->type = integerK;
                $$->size = 1;
                $$->child[0] = $2;
                $2->kind.stmt = variableK;
                $2->type = INTTYPE;
                }

|	    INT identificador ACO numero FCO PEV
                {
                    $$ = newExpNode(typeK);
                    $$->type = integerK;
                    $$->attr.name = "INT";
                    $$->size = $4->attr.val;
                    $$->child[0] = $2;
                    $2->nodekind = expressionK;
                    $2->kind.exp = variableK;
                    $2->type = INTTYPE; 
                    $2->child[0] = $4;
                }
	;

TipoEspecificador:	INT
                {
			$$ = newExpNode(typeK);
			$$->type = integerK;
		  	$$->attr.name = "INT";
            $$->size = 1;
			
		}
		|	VOID
                {
			$$ = newExpNode(typeK);
			$$->type = voidK;
           	$$->attr.name = "VOID";
            $$->size = 1;
			
		}
	;	

funDeclaracao:		INT identificador APR params FPR compostodecl
                {
                        $$ = newExpNode(typeK);
 			            $$->attr.name = "INT";
                        $2->type = INTTYPE;
                       	$$->child[0] = $2;
                        $2->child[0] = $4;
                       	$2->child[1] = $6;
			            $2->nodekind = expressionK;
                       	$2->kind.exp = functionK;

                 }
		| VOID identificador APR params FPR compostodecl
		{
			$$ = newExpNode(typeK);
 			$$->attr.name = "VOID";
            $2->type = VOIDTYPE;
            $$->child[0] = $2;
            $2->child[0] = $4;
            $2->child[1] = $6;
			$2->nodekind = expressionK;
            $2->kind.stmt = functionK;
		}	
	;
params:			paramlista
				{ $$ = $1; }
	 | VOID
          {
            $$ = newExpNode(typeK);
            $$->attr.name = "VOID";
            $$->size = 1;
            $$->child[0] = NULL;
          }
       ;

paramlista:		paramlista VIR param
				{
                           YYSTYPE t = $1;
                           if(t != NULL)
						   {
                              while(t->sibling != NULL)
                                  t = t->sibling;
                              t->sibling = $3;
                              $$ = $1;
                            }
                            else
                              $$ = $3;
                 }
	|			param
				{ $$ = $1; }
	;
param:	    TipoEspecificador identificador
            {
			    $$ = $1;
                $$->child[0] = $2;
                $$->child[0]->kind.exp = paramK;
 

                          
            } 
	|		TipoEspecificador identificador ACO FCO
            {
                $$ = $1;
                $$->size = 0;
                $$->child[0] = $2;
                $$->child[0]->kind.exp = paramK;

            } 
	;	
compostodecl:		ACH localdeclaracoes statementlista FCH
                 {
                            YYSTYPE t = $2;
                            if(t != NULL)
						    {
                               while(t->sibling != NULL)
                                  t = t->sibling;
                                t->sibling = $3;
                                $$ = $2;
                            }
                            else
                               $$ = $3;
                  }
	;
localdeclaracoes:	localdeclaracoes varDeclaracao
                  {
                            YYSTYPE t = $1;
                            if(t != NULL)
							{
                            	while(t->sibling != NULL)
                                	 t = t->sibling;
                             	t->sibling = $2;
                             	$$ = $1;
                            }
                            else
                               $$ = $2;
                   }
	|			vazio
	;
statementlista:		statementlista statement
                   {
                           YYSTYPE t = $1;
                           if(t != NULL)
						   {
                              while(t->sibling != NULL)
                                   t = t->sibling;
                              t->sibling = $2;
                              $$ = $1;
                           }
                           else
                             $$ = $2;
                   }
	|			vazio
	;
statement:			expressaodecl
				{ $$ = $1; }
	|			compostodecl
				{ $$ = $1; }
	|			selecaodecl
				{ $$ = $1; }
	|			iteracaodecl
				{ $$ = $1; }
	|			retornodecl
				{ $$ = $1; }
	|			ERR
				{ 
					$$ = newExpNode(operationK);
                    $$->attr.op = ERR;
					$$->type = voidK; 
				}
expressaodecl:	expressao PEV
				{ $$ = $1; } 
	|			PEV 
				{ }
	;
selecaodecl:		IF APR expressao FPR statement 
                   {
                             $$ = newStmtNode(ifK);
                             $$->child[0] = $3;
                             $$->child[1] = $5;
                   }

	|			IF APR expressao FPR statement ELSE statement
                   {
							 
                             $$ = newStmtNode(ifK);
                             $$->child[0] = $3;
                             $$->child[1] = $5;
                             $$->child[2] = $7;
                   }
	;
iteracaodecl:		WHILE APR expressao FPR statement
                   {
                             $$ = newStmtNode(whileK);
                             $$->child[0] = $3;
                             $$->child[1] = $5;
                   }
	;
retornodecl :		RETURN PEV
                   {
                            $$ = newStmtNode(returnK);
                   } 
	| 	 		RETURN expressao PEV
                   {
                            $$ = newStmtNode(returnK);
                            $$->child[0] = $2;
                   }

	;
expressao:			var IGL expressao

                    {
                            $$ = newStmtNode(assignK);
                            $$->child[0] = $1;
                            $$->child[1] = $3;
                    } 
	| 			simplesexpressao
				{ $$ = $1; }
	;
var:			identificador
				{ $$ = $1; } 
|			identificador ACO expressao FCO
            {
                $$ = $1;
                $$->type = INTTYPE;
                $$->child[0] = $3;
            }
	;
simplesexpressao:	somaexpressao relacional somaexpressao                       
				{
                    $$ = $2;
                    $$->child[0] = $1;
                    $$->child[1] = $3;
                }
 
	| 			somaexpressao { $$ = $1; }
	;

relacional: 		MEIGL
                {
                    $$ = newExpNode(operationK);
                    $$->attr.op = MEIGL;                            
							
                }
	|			MEN
                {
                    $$ = newExpNode(operationK);
                    $$->attr.op = MEN;                            
							
                }
	|			MAI
                {
                    $$ = newExpNode(operationK);
                    $$->attr.op = MAI;                            
							
                }
	|			MAIGL
                {
                    $$ = newExpNode(operationK);
                    $$->attr.op = MAIGL;                            
							
                }
	|			IGLIGL
                 {
                    $$ = newExpNode(operationK);
                    $$->attr.op = IGLIGL;  
							                     
                 }

	|			DIF
                 {
                    $$ = newExpNode(operationK);
                    $$->attr.op = DIF;
							                           
                 }
	;
somaexpressao:		somaexpressao soma termo 
                 {
                    $$ = $2;
                    $$->child[0] = $1;
                    $$->child[1] = $3;
                 }
	| 			termo
				{ $$ = $1; } 
	;
soma:			SOM
                {
                    $$ = newExpNode(operationK);
                    $$->attr.op = SOM;                            
                } 
	| 			SUB
				{
                    $$ = newExpNode(operationK);
                    $$->attr.op = SUB;                            
                }
	;

termo:			termo mult fator
                {
                    $$ = $2;
                    $$->child[0] = $1;
                    $$->child[1] = $3;
                } 
	|	 		fator
				{ $$ = $1; } 
	;
mult:			MUL
                {
                    $$ = newExpNode(operationK);
                    $$->attr.op = MUL;                            
                }
	|			DIV
 				{
                    $$ = newExpNode(operationK);
                    $$->attr.op = DIV;                            
                }
	;
fator:			APR expressao FPR
				{ $$ = $2; }  
	|			var
				{ $$ = $1; } 
	|			ativacao
				{ $$ = $1; } 
	| 			numero
				{ $$ = $1; } 
	;
ativacao : 		identificador APR arglista FPR
                {
                    $$ = newExpNode(activationK);
                    $$->attr.name = $1->attr.name;
                    $$->lineno = $1->lineno;
                    $$->child[0] = $3;
                
                }
	;

arglista : 		arglista VIR expressao
              {
                    YYSTYPE t = $1;
                    if(t != NULL)
                    {
                    while(t->sibling != NULL)
                        t = t->sibling;
                        t->sibling = $3;
                        $$ = $1;
                    }
                    else
                        $$ = $3;
                    }
	|			expressao
				{ $$ = $1; } 
	|			vazio
	;
	
vazio: {		
	 $$ = NULL;
}
identificador :	ID
				{
                    $$ = newExpNode(idK);
                    $$->attr.name = copyString(tokenString);
                 }
    ;
numero :   		NUM
                {
                    $$ = newExpNode(constantK);
                    $$->attr.val = atoi(tokenString);
			        $$->type = INTTYPE;
				}
    ;

%%

int yyerror(char* message){
     if(strcmp(tokenString,"ERROR")!=0){
        fprintf(listing,"Erro Lexico na linha %d\n",lineno);
        fprintf(listing,"Token atual: ");
        printToken(yychar,tokenString);
     }else{
        fprintf(listing,"Erro de Sintaxe na linha %d: %s\n",lineno,message);
        fprintf(listing,"Token atual: ");
        printToken(yychar,tokenString);
     }

   Error = TRUE;
    return 0;
}

static int yylex(void){
    return getToken();
}

TreeNode * parse(void){
    yyparse();
    return savedTree;
}
