#ifndef __LEXER_H__
#define __LEXER_H__

#include "idlib.h"
#include "Token.h"

// lexer flags
typedef enum {
	eLexfl_NoErrors					= BIT(0),	// don't print any errors
	eLexfl_NoWarnings					= BIT(1),	// don't print any warnings
	eLexfl_NoFatalErrors				= BIT(2),	// errors aren't fatal
	eLEXFL_NOSTRINGCONCAT				= BIT(3),	// multiple strings seperated by whitespaces are not concatenated
	eLEXFL_NOSTRINGESCAPECHARS			= BIT(4),	// no escape characters inside strings
	eLEXFL_NODOLLARPRECOMPILE			= BIT(5),	// don't use the $ sign for precompilation
	eLEXFL_NOBASEINCLUDES				= BIT(6),	// don't include files embraced with < >
	eLEXFL_ALLOWPATHNAMES				= BIT(7),	// allow path seperators in names
	eLEXFL_ALLOWNUMBERNAMES			= BIT(8),	// allow names to start with a number
	eLEXFL_ALLOWIPADDRESSES			= BIT(9),	// allow ip addresses to be parsed as numbers
	eLEXFL_ALLOWFLOATEXCEPTIONS			= BIT(10),	// allow float exceptions like 1.#INF or 1.#IND to be parsed
	eLEXFL_ALLOWMULTICHARLITERALS		= BIT(11),	// allow multi character literals
	eLEXFL_ALLOWBACKSLASHSTRINGCONCAT	= BIT(12),	// allow multiple strings seperated by '\' to be concatenated
	eLEXFL_ONLYSTRINGS					= BIT(13)	// parse as whitespace deliminated strings (quoted strings keep quotes)
} lexerFlags_t;

class Lexer
{
public:
	Lexer();
	~Lexer();

	void LoadMemory(const char* buffer);

	bool Lex(Token& result);

	int CurrentPos();

	char* SubStr(int start, int end);
private:

	void IncLineNumber();

	char CurrentAndNext();

	bool ReadNumber(Token& result);

	bool ReadName(Token& result);


private:
	const char* _buffer;
	const char* _bufferPtr;
	int _currentLine;
	int _lineNumber;
};


#endif