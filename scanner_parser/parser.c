/*
Purpose    : Build the syntax tree and returns the root of the tree to main()
written by : Suriyanarayanan
Rollnumber : 15563
*/

#include "header.h"

TreeNode *newStmtNode(StmtKind kind)  // create and initialize treeNode
{
TreeNode *t = (TreeNode*)malloc(1*sizeof(TreeNode));
if (t!=NULL)
 {
  int i;
  for(i=0;i<MAXCHILDREN;i++) 
	t->child[i]=NULL;
  t->sibling = NULL;
  t->nodekind = Statement;
  t->kind.stmt = kind;
  t->lineno = lineno;
  
 }
 return t;
}

TreeNode *newExpNode(ExpKind kind)	// create and initialize treeNode
{
TreeNode *t = (TreeNode*)malloc(1*sizeof(TreeNode));
if (t!=NULL)
 {
  int i;
  for(i=0;i<MAXCHILDREN;i++) 
	t->child[i]=NULL;
  t->sibling = NULL;
  t->nodekind = Expression;
  t->kind.exp = kind;
  t->lineno = lineno;
  t->type = Void;
  
 }
 return t;
}
static void match(TokenType expected)   // checks if currentToken == Expected Token
{
 if(token == expected)
	token =  getToken();
 else
  {
  printf("\nMATCH Error: Unexpected token... Expected %d got %d\n",expected,token);
  }
}

char *copyString(char *s)  // returns a copy of string s
{
 int n;
 char *t;
 if (s==NULL)
	return NULL;
 n=strlen(s)+1;
 t=malloc(n);
 strcpy(t,s);
 return t;
}

TreeNode * factor() 
{
 TreeNode *t=NULL;
 switch(token)
 {
 case NUM: t=newExpNode(ConstK);   
	if(t!=NULL && token==NUM)
		t->attr.val=atoi(tokenString); 
	match(NUM);
	break;

 case ID:
	t=newExpNode(IdK);  
	if(t!=NULL && token==ID)
        {
	t->attr.name=copyString(tokenString);
        }
	match(ID);
        break;
 case LPAREN:
	match(LPAREN);
	t=expr();       
	match(RPAREN);
        break;
 default:
	printf("\nFactor: Unexpected Token Error\n");
        token=getToken();
        break;
	
 }
 return t;
}

TreeNode *term()
{
TreeNode *t = factor();  
if(t!=NULL)
{
while(token==TIMES || token==OVER)  
 {
   TreeNode *temp= newExpNode(OpK);
   if (temp!=NULL) 
   {
	temp->child[0]=t;
	temp->attr.op=token;
	t=temp;
	match(token);
	t->child[1]=factor();
   }
 }
 }
 return t;
}

TreeNode * simple_expr()
{
 TreeNode *t = term();
 if(t!=NULL)
 {
 while(token==PLUS || token == MINUS)
 {
  TreeNode *temp= newExpNode(OpK);
  if(temp!=NULL)
    {
	temp->child[0]=t;
	temp->attr.op=token;
	t=temp;
	match(token);
	t->child[1]=term();
   }
 }
 }
 return t;
}

TreeNode *expr()
{
TreeNode *t = simple_expr();
if(t!=NULL && (token== LT || token == EQ))
 {
  TreeNode *temp= newExpNode(OpK);
  if(temp!=NULL)
    {
	temp->child[0]=t;
	temp->attr.op=token;
	t=temp;
	match(token);
	t->child[1]=simple_expr();
    }
 }
 return t;
}

TreeNode *if_stmt()
{
TreeNode *t= newStmtNode(IfK);
if(t!=NULL)
 {
  match(IF);
  t->child[0]=expr();
  match(THEN);
  t->child[1]=stmt_sequence();
 
  if(token==ELSE)
  {
    match(ELSE);
    t->child[2]=stmt_sequence();
  }
  match(END);
 }
 return t;
}

TreeNode * repeat_stmt()
{
 TreeNode *t=newStmtNode(RepeatK);
 if (t!=NULL)
 {
  match(REPEAT);
  t->child[0]=stmt_sequence();
  match(UNTIL);
  t->child[1]=expr();
 }
 return t;
}

TreeNode *assign_stmt()
{
 TreeNode *t = newStmtNode(AssignK);
 if(t!=NULL && token==ID)
 {
  t->attr.name=copyString(tokenString);
 } 
 if(t!=NULL)
 {
  match(ID);
  match(ASSIGN);
  t->child[0]=expr();
 }
 return t;
}

TreeNode * read_stmt()
{
 TreeNode *t = newStmtNode(ReadK);
 if(t!=NULL)
 {
  match(READ);
  if(token==ID)
  {
    t->attr.name=copyString(tokenString);

  }
  match(ID);
 }
 return t;
}

TreeNode * write_stmt()
{
 TreeNode *t = newStmtNode(WriteK);
 if(t!=NULL)
 {
  match(WRITE);
  t->child[0]=expr();
 }
 return t;
}
TreeNode *statement()
{
TreeNode *t = NULL;
switch(token)
{
 case IF: t= if_stmt(); break;
 case REPEAT: t=repeat_stmt(); break;
 case ID: t=assign_stmt(); break;
 case READ: t=read_stmt();  break;
 case WRITE: t=write_stmt(); break;
 default :
	printf("\nInvalid token encountered\n\n");
	token=getToken();
	break;
}
return t;
}

TreeNode * stmt_sequence()
{
TreeNode *t =  statement();
TreeNode *temp= t;
if (t==NULL)
	return t;
else
{
 while(token!=END && token!=ELSE && token!=UNTIL && token!=ENDFILE)
 {
  TreeNode *s;
  match(SEMI);
  s=statement();
  if(s!=NULL)
  {
    temp->sibling=s;
    temp=s;
  }
 }
 return t;
}

}

TreeNode * parse(void)
{
TreeNode *t;
token=getToken();    // Invokes the scanner for getting the token
t =  stmt_sequence();
if(token!=ENDFILE)
	printf("\bError:");

return t;
}

static void printSpaces()
{
int i;
for(i=0;i<indent;i++)
	fprintf(output," ");
return ; 
}

void printTree(TreeNode *tree)
{
INDENT;
while(tree!=NULL)
{
printSpaces();
if(tree->nodekind==Statement)
{
switch(tree->kind.stmt)
 {
	case IfK:
	fprintf(output,"If\n");
	break;
	case RepeatK:
	fprintf(output,"Repear\n");
	break;
	case AssignK:
	fprintf(output,"Assigne to: %s\n",tree->attr.name);
	break;
	case ReadK:
	fprintf(output,"Read : %s\n",tree->attr.name);
	break;
	case WriteK:
	fprintf(output,"Write\n");
	break;
	default:
	fprintf(output,"Unknown Expression ");
	break;
 }
}
else if(tree->nodekind==Expression)
{
 switch(tree->kind.exp)
 {
	case OpK:
	fprintf(output,"Op:\n");
	break;
	case ConstK:
	fprintf(output,"Const : %d\n",tree->attr.val);
	break;
	case IdK:
	fprintf(output,"Id: %s\n",tree->attr.name);
	break;
	default:
	fprintf(output,"Unknown Expression!! %s\n",tree->attr.name);
	break;
 }
}
else {
	fprintf(output,"Unknown Expression!!");
 }
int i;
for(i=0;i<MAXCHILDREN;i++)
	printTree(tree->child[i]);
tree=tree->sibling; 
}
UNINDENT;
return ;
}


