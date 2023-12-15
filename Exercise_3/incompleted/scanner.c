#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "reader.h"
#include "charcode.h"
#include "token.h"
#include "error.h"


extern int lineNo;
extern int colNo;
extern int currentChar;

extern CharCode charCodes[];

/***************************************************************/

void skipBlank() {
	 while (currentChar != EOF && charCodes[currentChar] == CHAR_SPACE)
        readChar();
  // TODO
}

void skipComment() {
	int tmp = currentChar;
    readChar();
    while (!((charCodes[tmp] == CHAR_TIMES || charCodes[tmp] == CHAR_PERIOD) && charCodes[currentChar] == CHAR_RPAR)) {
        tmp = currentChar;
        readChar();
    }
    readChar();
  // TODO
}

Token* readIdentKeyword(void) {
	int i = 0;
    Token *token = makeToken(TK_IDENT, lineNo, colNo);
    char *str = token->string;
    while (charCodes[currentChar] == CHAR_LETTER || charCodes[currentChar] == CHAR_DIGIT) {
        str[i++] = currentChar;
        readChar();
    }
    str[i] = '\0';
 
    TokenType type = checkKeyword(str);
    if (type != TK_NONE) {
        token->tokenType = type;
    }
 
    while (charCodes[currentChar] == CHAR_LETTER || charCodes[currentChar] == CHAR_DIGIT)
        readChar();
 
    return token;
  //TODO
}

Token* readNumber(void) {
	 int i = 0;
    Token *token = makeToken(TK_NUMBER, lineNo, colNo);
    int check=2;
    char *str = token->string;
    while (check!=0 && i < MAX_IDENT_LEN && (charCodes[currentChar] == CHAR_DIGIT || charCodes[currentChar] == CHAR_PERIOD)) {
        str[i++] = currentChar;
        if(charCodes[currentChar] == CHAR_PERIOD) check--;
        readChar();
    }
    str[i] = '\0';
    if(check==1) token->tokenType=TK_FLOAT;
    while (charCodes[currentChar] == CHAR_DIGIT)
        readChar();
    return token;
  // TODO
}
Token* readString(void) {
 int i = 0;
 int flag=0;
    Token *token = makeToken(TK_STRING, lineNo, colNo);
    char *str = token->string;
    readChar();
    while (i < MAX_IDENT_LEN && charCodes[currentChar]!=CHAR_QUOTATION) {
        if(flag==0 && currentChar=='\n')
          error(ERR_INVALIDSYMBOL,lineNo,colNo);
        else
        if(charCodes[currentChar]==CHAR_BACKSLASH){
          flag=1;
          readChar();
          if(currentChar=='"'){     str[i++]= '"';   }
          else if(currentChar=='t'){     str[i++]= 9;    }
          else if(flag==1 && currentChar=='n'){   str[i++]=' ';  readChar(); }
          else if(flag==1 && currentChar=='\n')    {   str[i++]=' ';}
          else error(ERR_INVALIDSYMBOL,lineNo,colNo);
          flag=0;
          readChar();
        }else{
          str[i++] = currentChar;
          readChar();
       }
    }
    str[i] = '\0';
    while(charCodes[currentChar]!=CHAR_QUOTATION)
        readChar();
    readChar();
    return token;
}
Token* readConstChar(void) {
  Token*tmp;
  tmp=makeToken(TK_CHAR,lineNo,colNo);
  tmp->string[0]=(char)currentChar;
  tmp->string[1]='\0';
  readChar();
  return tmp;
  // TODO
}

Token* getToken(void) {
  Token *token;
 // int ln, cn;

  if (currentChar == EOF) 
    return makeToken(TK_EOF, lineNo, colNo);

  switch (charCodes[currentChar]) {
	case CHAR_SPACE: skipBlank(); return getToken();
	case CHAR_LETTER: return readIdentKeyword();
	case CHAR_DIGIT: return readNumber();
	case CHAR_PLUS: //'+'
	    token = makeToken(SB_PLUS, lineNo, colNo);
	    readChar(); 
	    return token;
	case CHAR_MINUS://"-"
	  	token = makeToken(SB_MINUS, lineNo, colNo);
	    readChar(); 
	    return token;
  case CHAR_MOD://"-"
      token = makeToken(SB_MOD, lineNo, colNo);
      readChar(); 
      return token;

	case CHAR_TIMES://'*'
	  	token = makeToken(SB_TIMES, lineNo, colNo);
	    readChar(); 
	    return token;
	case CHAR_SLASH://'/'
	  	token = makeToken(SB_SLASH, lineNo, colNo);
	    readChar(); 
	    return token;
	case CHAR_LT://'<'
	    token = makeToken(SB_LT, lineNo, colNo);
	    readChar(); 
	    return token;
	case CHAR_GT://'>'
	    token = makeToken(SB_GT, lineNo, colNo);
	    readChar(); 
	    return token;
	case CHAR_EXCLAIMATION://'!'
	    token = makeToken(TK_NONE, lineNo, colNo);
	    readChar(); 
	    if(charCodes[currentChar]==CHAR_EQ){
	    	token->tokenType=SB_NEQ;
	    	readChar();
	    }else{
	    	error(ERR_INVALIDSYMBOL,lineNo,colNo);
	    }
	    return token;
	case CHAR_EQ://'='
	    token = makeToken(SB_EQ, lineNo, colNo);
	    readChar(); 
	    return token;
	case CHAR_COMMA://','
	    token = makeToken(SB_COMMA, lineNo, colNo);
	    readChar(); 
	    return token;
	case CHAR_PERIOD://'.'
	    token = makeToken(SB_PERIOD, lineNo, colNo);
	    int i,j=0;
      char tmp[20];
      tmp[0]='0';
      tmp[1]='.';
      i=2;
      readChar();
      if(charCodes[currentChar]==CHAR_DIGIT){
      token = readNumber();
      token->tokenType=TK_FLOAT;
      while(token->string[j]!='\0')
        tmp[i++]=token->string[j++];
      tmp[i]='\0';
      for(i=0;i<MAX_IDENT_LEN;i++)
        token->string[i]=tmp[i];
	    return token;
    } else return token;
	case CHAR_COLON://':'
	    token = makeToken(SB_COLON, lineNo, colNo);
	    readChar(); 
	    if(charCodes[currentChar]==CHAR_EQ){
	    	token->tokenType=SB_ASSIGN;
	    	readChar();
	    }
	    return token;
	case CHAR_SEMICOLON://';'
	    token = makeToken(SB_SEMICOLON, lineNo, colNo);
	    readChar(); 
	    return token;
	case CHAR_SINGLEQUOTE://'\''
	  	return readConstChar();
  case CHAR_QUOTATION:
      return readString();

	case CHAR_LPAR://'('
	    token = makeToken(SB_LPAR, lineNo, colNo);
	    readChar();
	    if(charCodes[currentChar]==CHAR_TIMES || charCodes[currentChar]==CHAR_PERIOD){
	    	skipComment();
	    	//token->tokenType=TK_NONE;
	    	return getToken();
	    }
	    return token;
	case CHAR_RPAR://')'
	    token = makeToken(SB_RPAR, lineNo, colNo);
	    readChar(); 
	    return token;
	    // note sb_assign,SB_NEQ,SB_LE,SB_GE,SB_LSEL, SB_RSEL don't know what to do :))))
	    // ....
	    // TODO
	    // ....
	  default:
	    token = makeToken(TK_NONE, lineNo, colNo);
	    error(ERR_INVALIDSYMBOL, lineNo, colNo);
	    readChar(); 
	    return token;
  }
}


/******************************************************************/

void printToken(Token *token) {

  printf("%d-%d:", token->lineNo, token->colNo);

  switch (token->tokenType) {
  case TK_NONE: printf("TK_NONE\n"); break;
  case TK_IDENT: printf("TK_IDENT(%s)\n", token->string); break;
  case TK_FLOAT: printf("TK_FLOAT(%s)\n", token->string); break;
  case TK_NUMBER: printf("TK_NUMBER(%s)\n", token->string); break;
  case TK_CHAR: printf("TK_CHAR(\'%s\')\n", token->string); break;
  case TK_STRING:printf("TK_STRING(\'%s\')\n", token->string); break;
  case TK_EOF: printf("TK_EOF\n"); break;

  case KW_PROGRAM: printf("KW_PROGRAM\n"); break;
  case KW_CONST: printf("KW_CONST\n"); break;
  case KW_TYPE: printf("KW_TYPE\n"); break;
  case KW_VAR: printf("KW_VAR\n"); break;
  case KW_INTEGER: printf("KW_INTEGER\n"); break;
  case KW_CHAR: printf("KW_CHAR\n"); break;
  case KW_ARRAY: printf("KW_ARRAY\n"); break;
  case KW_OF: printf("KW_OF\n"); break;
  case KW_FUNCTION: printf("KW_FUNCTION\n"); break;
  case KW_PROCEDURE: printf("KW_PROCEDURE\n"); break;
  case KW_BEGIN: printf("KW_BEGIN\n"); break;
  case KW_FLOAT: printf("KW_FLOAT\n"); break;
  case KW_END: printf("KW_END\n"); break;
  case KW_CALL: printf("KW_CALL\n"); break;
  case KW_IF: printf("KW_IF\n"); break;
  case KW_THEN: printf("KW_THEN\n"); break;
  case KW_ELSE: printf("KW_ELSE\n"); break;
  case KW_WHILE: printf("KW_WHILE\n"); break;
  case KW_DO: printf("KW_DO\n"); break;
  case KW_FOR: printf("KW_FOR\n"); break;
  case KW_TO: printf("KW_TO\n"); break;

  case SB_SEMICOLON: printf("SB_SEMICOLON\n"); break;
  case SB_COLON: printf("SB_COLON\n"); break;
  case SB_PERIOD: printf("SB_PERIOD\n"); break;
  case SB_COMMA: printf("SB_COMMA\n"); break;
  case SB_ASSIGN: printf("SB_ASSIGN\n"); break;
  case SB_EQ: printf("SB_EQ\n"); break;
  case SB_NEQ: printf("SB_NEQ\n"); break;
  case SB_LT: printf("SB_LT\n"); break;
  case SB_LE: printf("SB_LE\n"); break;
  case SB_GT: printf("SB_GT\n"); break;
  case SB_GE: printf("SB_GE\n"); break;
  case SB_PLUS: printf("SB_PLUS\n"); break;
  case SB_MINUS: printf("SB_MINUS\n"); break;
  case SB_MOD: printf("SB_MOD\n"); break;
  case SB_TIMES: printf("SB_TIMES\n"); break;
  case SB_SLASH: printf("SB_SLASH\n"); break;
  case SB_LPAR: printf("SB_LPAR\n"); break;
  case SB_RPAR: printf("SB_RPAR\n"); break;
  case SB_LSEL: printf("SB_LSEL\n"); break;
  case SB_RSEL: printf("SB_RSEL\n"); break;
  }
}

int scan(char *fileName) {
  Token *token;

  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  token = getToken();
  while (token->tokenType != TK_EOF) {
    printToken(token);
    free(token);
    token = getToken();
  }

  free(token);
  closeInputStream();
  return IO_SUCCESS;
}

/******************************************************************/

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    printf("scanner: no input file.\n");
    return -1;
  }

  if (scan(argv[1]) == IO_ERROR) {
    printf("Can\'t read input file!\n");
    return -1;
  }
    
  return 0;
}



