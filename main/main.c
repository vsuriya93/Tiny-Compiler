/*
Purpose    : This is the driver program for building the parse tree
written by : Suriyanarayanan
Rollnumber : 15563
*/

#include "header.h"
#define TERMINAL_OUTPUT 1  // set this value to 0, to write output to file
FILE* source;  // input file
FILE *output;  // output file

int main()
{
source = fopen("input.txt","r");
if(TERMINAL_OUTPUT)
	output=stdout;
else
 output=fopen("output.txt","w");

TreeNode *syntaxTree;
syntaxTree=parse();  // build tree

printTree(syntaxTree);  // print tree

buildSymtab(syntaxTree);
typeCheck(syntaxTree);
return 0;
}
