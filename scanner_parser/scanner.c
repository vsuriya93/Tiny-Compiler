/*
Purpose    : This is scanner, which returns token on call by parser() and match() function
written by : Suriyanarayanan
Rollnumber : 15563
*/

#include "header.h"

static int getNextChar()
{
if (! (linepos < bufsize))   // read the line into lineBuf
 {
  lineno++;
  if ( fgets(lineBuf,BUFLEN-1,source))
	{
	 bufsize=strlen(lineBuf);
	 linepos=0;
	 return lineBuf[linepos++];
	} 
  else                   // checks end of file
	{
	EOF_flag=TRUE;
	return EOF;
       }
 }
 else    // till there are char remaining in lineBuf, return them one by one.
 {
  return lineBuf[linepos++];
 }
}

static void ungetNextChar()
{
if (!EOF_flag)
	linepos--;
}

static TokenType reservedLookup(char *tokenString)
{;
int i;
for(i=0;i<MAXRESERVED;i++)
 { 
   if(! (strcmp(tokenString,reservedWords[i].str) ))
	return reservedWords[i].tok;
 }
 return ID;
}
TokenType getToken()
{
int tokenStringIndex=0; // Position in the string read from file
TokenType currentToken; // Return value of this function
StateType state=START;  // default START state
int save; 

while (state != DONE)
{
 int c = getNextChar();  // gets from file char by char
 save=TRUE;
 switch(state)
 {
	case START:

	if (c=='\t' || c=='\n' || c==' ')  // ignore blank spaces and comments
	{
	 save=FALSE;
	}
	else if (c == '{' ) // Comment is staring
	{
	  state=INCOMMENT;
	  save=FALSE;
	}
	else if (c == ':') // assignment statement, := wait for '=' symbol next
	{
	 state=INASSIGN;
	}
	else if (isdigit(c))
	{
	 state=INNUM;
	}
	else if (isalpha(c)) // Came across a char token, then may be an ID
	{
	 state=INID;
	}
	else if (c == '+')  // operators arithematic
	{
	 state=DONE;	
	 currentToken=PLUS;
	}	
	else if (c == '-')
	{
	 state=DONE;	
	 currentToken=MINUS;
	}	
	else if (c == '*')
	{
	 state=DONE;	
	 currentToken=TIMES;
	}	
	else if (c == '/')
	{
	 state=DONE;	
	 currentToken=OVER;
	}	 
	else if (c == '=')   // comparison operators
	{
	 state=DONE;	
	 currentToken=EQ;
	}	
	else if (c == '<')
	{
	 state=DONE;	
	 currentToken=LT;
	}	
	else if (c == '(')   // paren, and semicolen
	{
	 state=DONE;	
	 currentToken=LPAREN;
	}	
	else if (c == ')')
	{
	 state=DONE;	
	 currentToken=RPAREN;
	}	
	else if (c == ';')
	{
	 state=DONE;	
	 currentToken=SEMI;
	}	
	else if (c == EOF)
	{
	 state=DONE;	
	 currentToken=ENDFILE;
	 save=FALSE;
	}	
	else
	{
	 currentToken=ERROR;
	 save=FALSE;
	}
	break;
	
	case INCOMMENT:  // dealing with comments, ignore them
	save=FALSE;
	if (c == '}')
	{
	 state=START;
	}
	else
	{
	 state=INCOMMENT;
	}
	break;

	case INASSIGN: // check for assignment operator, = after :
	if (c == '=')
	{
	 state = DONE;
	 currentToken=ASSIGN;
	}
	else
	{
	 ungetNextChar();
  	 currentToken=ERROR;
	 save=FALSE; 
	}
	break;

	case INNUM:
	if(isdigit(c))
	{
	state=INNUM;
	}
	else
	{
	state=DONE;
	ungetNextChar();
	currentToken=NUM;
	save=FALSE;
	}
        break;
	
  	case INID:
	if(isalpha(c))	
	{
	state=INID;
	}
	else
	{
	state=DONE;
	ungetNextChar();
	currentToken=ID;
	save=FALSE;
	}
	case DONE: break;
        default: printf("I am screwed\n");

 } // end of switch
 if ( (save) && (tokenStringIndex<=MAXTOKENLEN))
  {
    tokenString[tokenStringIndex++] = (char) c;  // save till what has been scanned
  }
  tokenString[tokenStringIndex] = '\0';
  if (currentToken == ID)
  {
	currentToken=reservedLookup(tokenString);
  }
} //end of while 

fprintf(output,"%d : %s\n",currentToken,tokenString);
 return currentToken;

} //end of function

