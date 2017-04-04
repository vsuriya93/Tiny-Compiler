/*
Purpose    : Contains function prototypes, structs and extern variables for the program
written by : Suriyanarayanan
Rollnumber : 15563
*/

#include<stdio.h>
#include<string.h>
#include<malloc.h>

#define MAXCHILDREN 3  //Max number of children per node
#define BUFLEN 256  // Length of the line read at each time
#define TRUE 1
#define FALSE 0
#define MAXTOKENLEN 100  // maximum allowed length of the token
#define MAXRESERVED 8  // Maximum number of reserved word like READ... are defined

typedef enum {Statement,Expression} NodeKind; // 2 kinds of nodes are there
typedef enum {IfK,RepeatK,AssignK,ReadK,WriteK} StmtKind; // Types of statements accepted
typedef enum {OpK,ConstK,IdK} ExpKind; // Types of expressions
typedef enum {ENDFILE,ERROR,IF,THEN,ELSE,END,REPEAT,UNTIL,READ,WRITE,ID,NUM,ASSIGN,EQ,LT,PLUS,MINUS,TIMES,OVER,LPAREN,RPAREN,SEMI} TokenType; // Types of token
typedef enum {Void,Integer,Boolean} ExpType; // Type of the expression
typedef enum {START,DONE,INCOMMENT,INASSIGN,INNUM,INID} StateType; // The possible states for the token, read char by char


static struct  // Reserved word struct
{
char *str;
TokenType tok;
}reservedWords[MAXRESERVED]={{"if",IF},{"then",THEN},{"else",ELSE},{"end",END},{"repeat",REPEAT},{"until",UNTIL},{"read",READ},{"write",WRITE}};

struct treeNode  // nodes of the syntaxTree
{
struct treeNode *child[MAXCHILDREN];  // number of the children the syntax tree can have
struct treeNode *sibling;
int lineno;
NodeKind nodekind;
union {                //
	StmtKind stmt; 
	ExpKind exp;
      }kind;
union {
	TokenType op;
	int val;
	char *name;
      } attr;
ExpType type; // check which type of expressions		
};
typedef struct treeNode TreeNode;

extern FILE *source; // input file
extern FILE *output;  // output file

int lineno;
static TokenType token ;

static char lineBuf[BUFLEN];
static int linepos=0;
static int bufsize=0;
static int EOF_flag=FALSE;
char tokenString[MAXTOKENLEN+1];

static int indent=0;       // for printing outpout in readable form
#define INDENT indent+=2
#define UNINDENT indent-=2

// function prototypes for the parser

static TreeNode * stmt_sequence(void);
static TreeNode * statement(void);
static TreeNode * if_stmt(void);
static TreeNode * repeat_stmt(void);
static TreeNode * assign_stmt(void);
static TreeNode * read_stmt(void);
static TreeNode * write_stmt(void);
static TreeNode * expr(void);
static TreeNode * simple_exp(void);
static TreeNode * term(void);
static TreeNode * factor(void);

// function prototype for semantic analyzer

void buildSymoblTable(TreeNode *);
void st_insert( char * name, int lineno, int loc );
int st_lookup ( char * name );
void printSymTab();
