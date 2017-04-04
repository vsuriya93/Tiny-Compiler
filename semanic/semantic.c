#include "header.h"

static int location =0;

/* SIZE is the size of the hash table */
#define SIZE 211
/* SHIFT is the power of two used as multiplier
   in hash function  */
#define SHIFT 4
/* the hash function */
static int hash ( char * key )
{ int temp = 0;
  int i = 0;
  while (key[i] != '\0')
  { temp = ((temp << SHIFT) + key[i]) % SIZE;
    ++i;
  }
  return temp;
}
/* the list of line numbers of the source 
 * code in which a variable is referenced
 */
typedef struct LineListRec
   { int lineno;
     struct LineListRec * next;
   } * LineList;
/* The record in the bucket lists for
 * each variable, including name, 
 * assigned memory location, and
 * the list of line numbers in which
 * it appears in the source code
 */
typedef struct BucketListRec
   { char * name;
     LineList lines;
     int memloc ; /* memory location for variable */
     struct BucketListRec * next;
   } * BucketList;
/* the hash table */
static BucketList hashTable[SIZE];
/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */
void st_insert( char * name, int lineno, int loc )
{ int h = hash(name);
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0))
    l = l->next;
  if (l == NULL) /* variable not yet in table */
  { l = (BucketList) malloc(sizeof(struct BucketListRec));
    l->name = name;
    l->lines = (LineList) malloc(sizeof(struct LineListRec));
    l->lines->lineno = lineno;
    l->memloc = loc;
    l->lines->next = NULL;
    l->next = hashTable[h];
    hashTable[h] = l; }
  else /* found in table, so just add line number */
  { LineList t = l->lines;
    while (t->next != NULL) t = t->next;
    t->next = (LineList) malloc(sizeof(struct LineListRec));
    t->next->lineno = lineno;
    t->next->next = NULL;
  }
} /* st_insert */
/* Function st_lookup returns the memory 
 * location of a variable or -1 if not found
 */
int st_lookup ( char * name )
{ int h = hash(name);
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0))
    l = l->next;
  if (l == NULL) return -1;
  else return l->memloc;
}
/* Procedure printSymTab prints a formatted 
 * listing of the symbol table contents 
 * to the listing file
 */
#define listing stdout
void printSymTab()
{ int i;
  fprintf(listing,"Variable Name  Location   Line Numbers\n");
  fprintf(listing,"-------------  --------   ------------\n");
  for (i=0;i<SIZE;++i)
  { if (hashTable[i] != NULL)
    { BucketList l = hashTable[i];
      while (l != NULL)
      { LineList t = l->lines;
        fprintf(listing,"%-14s ",l->name);
        fprintf(listing,"%-8d  ",l->memloc);
        while (t != NULL)
        { fprintf(listing,"%4d ",t->lineno);
          t = t->next;
        }
        fprintf(listing,"\n");
        l = l->next;
    }
  }
} /* printSymTab */
}
static void traverse(TreeNode *t, void (*preProc) (TreeNode *),void (*postProc) (TreeNode*))
{
if(t!=NULL)
{
 preProc(t);
 {
  int i;
  for(i=0;i<MAXCHILDREN;i++)
	traverse(t->child[i],preProc,postProc);
 }
 postProc(t);
 traverse(t->sibling,preProc,postProc);	
}

}

static void nullProc(TreeNode *t)
{
 return;
}

static void insertNode(TreeNode *t)
{
switch(t->nodekind)
{
 case Statement: // check this
		switch(t->kind.stmt)
		{
		 case AssignK:
		 case ReadK:
			if (st_lookup(t->attr.name)==-1) // if variable not present add to symtab
				st_insert(t->attr.name,t->lineno,location++);

			else
				st_insert(t->attr.name,t->lineno,0); // or add only line no
			break;
		}

 		break;
 case Expression:
		switch(t->kind.exp)
		{
		 case IdK:
			if(st_lookup(t->attr.name)==-1) // new identifier
				st_insert(t->attr.name,t->lineno,location++);
			else
				st_insert(t->attr.name,t->lineno,0); // all only line number
			break;
		}
		break;

 default:	break;
	
}

}

void buildSymtab(TreeNode *syntaxTree)
{
traverse(syntaxTree,insertNode,nullProc);
printSymTab();
return;
}

static void checkNode(TreeNode *t)
{
if(t==NULL)
{
printf("\nError in checkNode, node is NULL");
return ;
}

switch(t->nodekind)
{
 case Expression:
		switch(t->kind.exp)
		{
		 case OpK:

			if(t->child[0]==NULL || t->child[1]==NULL)
			{
			printf("\nError in checkNode, node is NULL");
			return ;
			}
			if((t->child[0]->type !=Integer)||(t->child[1]->type!=Integer))
				exit(0); //error
			if((t->attr.op==EQ)||(t->attr.op==LT))
				t->type=Boolean;
			else
				t->type=Integer;
			break;
		case ConstK:
		case IdK:
			t->type=Integer;
			break;
		default:
			break;
		}
		break;
 case Statement:
		switch(t->kind.stmt)
		{
		 case IfK:
			if(t->child[0]==NULL)
			{
			printf("\nError in checkNode IfK, node is NULL\n");
			return ;
			}
			if(t->child[0]->type==Integer)
				exit(0);
			break;
		case AssignK:

			if(t->child[0]==NULL)
			{
			printf("\nError in checkNode AssignK, node is NULL\n");
			return ;
			}
			if(t->child[0]->type!=Integer)
				exit(0);
			break;
		case WriteK:

			if(t->child[0]==NULL)
			{
			printf("\nError in checkNode WriteK, node is NULL\n");
			return ;
			}
			if(t->child[0]->type!=Integer)
				exit(0);
			break;
		case RepeatK:

			if(t->child[1]==NULL)
			{
			printf("\nError in checkNode RepeatK, node is NULL\n");
			return ;
			}
			if(t->child[1]->type==Integer)
				exit(0);
			break;
		default: break;
		}
		break;
 default:
		break;
}

}
void typeCheck(TreeNode *syntaxTree)
{
traverse(syntaxTree,nullProc,checkNode);
}
