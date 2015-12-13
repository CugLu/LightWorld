#ifndef __TOKEN_H__
#define __TOKEN_H__

#include "String.h"
/*
===============================================================================

	idToken is a token read from a file or memory with idLexer or idParser

===============================================================================
*/

enum TokenType{
  TK_AND = 257, 
  TK_BREAK,
  TK_DO, 
  TK_ELSE, 
  TK_ELSEIF, 
  TK_END, 
  TK_FALSE, 
  TK_FOR, 
  TK_FUNCTION,
  TK_IF, 
  TK_IN, 
  TK_LOCAL, 
  TK_NIL, 
  TK_NOT, 
  TK_OR, 
  TK_REPEAT,
  TK_RETURN, 
  TK_THEN, 
  TK_TRUE, 
  TK_UNTIL, 
  TK_WHILE,
  /* other terminal symbols */
  TK_CONCAT, 
  TK_DOTS, 
  TK_EQ, 
  TK_GE, 
  TK_LE, 
  TK_NE, 
  TK_NUMBER,
  TK_NAME, 
  TK_STRING, 
  TK_UNKOWN
};

// string sub type is the length of the string
// literal sub type is the ASCII code
// punctuation sub type is the punctuation id
// name sub type is the length of the name
typedef lfStr idStr;

class Token
{
public:
	int	_type;
	idStr _data;

public:
	Token( void );
	Token( const Token *token );
	~Token( void );

	void Reset();

	const char* Name();

	void AppendData(char c);
};


#endif /* !__TOKEN_H__ */
