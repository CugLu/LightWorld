#include "Token.h"

/* ORDER RESERVED */
const char *const tokens [] = {
    "and", "break", "do", "else", "elseif",
    "end", "false", "for", "function", "if",
    "in", "local", "nil", "not", "or", "repeat",
    "return", "then", "true", "until", "while",
    "..", "...", "==", ">=", "<=", "~=",
    "<number>", "<name>", "<string>", "<unkown>",
    NULL
};

Token::Token( void )
{

}

Token::Token( const Token *token )
{

}

Token::~Token( void )
{

}

void Token::Reset() 
{
	_type = TK_UNKOWN;
	_data.Clear();
}

void Token::AppendData( char c )
{
	_data += c;
}

const char* Token::Name()
{
	if (_type < TK_AND)
	{
		_data += (char)_type;	
		return _data.c_str();
	}
	else 
		return tokens[_type - TK_AND];
}
