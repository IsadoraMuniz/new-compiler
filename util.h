/****************************************************/
/* 					                                */
/*                Lib Arvore Sintatica              */
/*                 Isadora Muniz                    */
/****************************************************/

#ifndef _UTIL_H_
#define _UTIL_H_

void printToken( TokenType, const char* );
 
TreeNode * newStmtNode(StatementKind );

TreeNode * newExpNode(ExpressionIdentifier );

char * copyString( char * );

void printTree( TreeNode * );

#endif
