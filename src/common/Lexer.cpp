#include "lexer.h"

Lexer::Lexer( void ) 
{
}

Lexer::~Lexer()
{

}

bool Lexer::Lex( Token& result ) {
	result.Reset();

	for (;;) {
		char c = *_bufferPtr;
		switch (c) {
		case 0:
			return false;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			return ReadNumber(result);
		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
		case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
		case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':   /*'U'*/
		case 'V': case 'W': case 'X': case 'Y': case 'Z':
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
		case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
		case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':   /*'u'*/
		case 'v': case 'w': case 'x': case 'y': case 'z':
		case '_':
			return ReadName(result);
		case '=': {
				CurrentAndNext();
				c = *_bufferPtr;
				if (c == '=')
					result._type = TK_EQ;
				else 
					result._type = '=';
			 }
			 return true;
		case '{':
		case '}':{
			CurrentAndNext();
			result._type = c;
		}
			return true;
		default:
			_bufferPtr++;
			break;
		}
	}
	
	return true;
}

inline char Lexer::CurrentAndNext()
{
	return *_bufferPtr++;
}

bool Lexer::ReadNumber( Token& result )
{
	result._type = TK_NUMBER;
	int dot;
	char c, c2;

	c = CurrentAndNext(); 
	c2 = *_bufferPtr;

	if ( c == '0' && c2 != '.' ) {
		// check for a hexadecimal number
		if ( c2 == 'x' || c2 == 'X' ) {
		}
		// check for a binary number
		else if ( c2 == 'b' || c2 == 'B' ) {
		}
		// its an octal number
		else {
		}
	}
	else {
		// decimal integer or floating point number or ip address
		dot = 0;
		while( 1 ) {
			if ( c >= '0' && c <= '9' ) {
			}
			else if ( c == '.' ) {
				dot++;
			}
			else {
				break;
			}
			result.AppendData( c );
			c = CurrentAndNext();
		}
		if( c == 'e' && dot == 0) {
			//We have scientific notation without a decimal point
			dot++;
		}
	}

	return true;
}

bool Lexer::ReadName( Token& result )
{
	char c = CurrentAndNext();
	result._type = TK_NAME;
	do {
		result.AppendData(c);
		c = CurrentAndNext();
	} while ((c >= 'a' && c <= 'z') ||
				(c >= 'A' && c <= 'Z') ||
				(c >= '0' && c <= '9') ||
				c == '_');
	return true;
}

void Lexer::LoadMemory(const char* buffer )
{
	_bufferPtr = buffer;
	_buffer = buffer;
}

int Lexer::CurrentPos()
{
	return _bufferPtr - _buffer;
}

char* Lexer::SubStr( int start, int end )
{
	int len = end - start;
	char* text = new char[len+1];
	for (int i=0; i<len; ++i)
	{
		text[i] = _buffer[start+i];
	}
	text[len] = '\0';
	return text;
}
