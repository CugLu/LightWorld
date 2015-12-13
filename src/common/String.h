#ifndef __STR_H__
#define __STR_H__

#include "precompiled.h"

#define 						__forceinline


#define INTSIGNBITNOTSET(i)		((~((const unsigned long)(i))) >> 31)


#define stricmp			lfStr::Icmp		// use_idStr_Icmp

#ifndef FILE_HASH_SIZE
#define FILE_HASH_SIZE		1024
#endif

// color escape character
const int C_COLOR_ESCAPE			= '^';
const int C_COLOR_DEFAULT			= '0';
const int C_COLOR_RED				= '1';
const int C_COLOR_GREEN				= '2';
const int C_COLOR_YELLOW			= '3';
const int C_COLOR_BLUE				= '4';
const int C_COLOR_CYAN				= '5';
const int C_COLOR_MAGENTA			= '6';
const int C_COLOR_WHITE				= '7';
const int C_COLOR_GRAY				= '8';
const int C_COLOR_BLACK				= '9';

// color escape string
#define S_COLOR_DEFAULT				"^0"
#define S_COLOR_RED					"^1"
#define S_COLOR_GREEN				"^2"
#define S_COLOR_YELLOW				"^3"
#define S_COLOR_BLUE				"^4"
#define S_COLOR_CYAN				"^5"
#define S_COLOR_MAGENTA				"^6"
#define S_COLOR_WHITE				"^7"
#define S_COLOR_GRAY				"^8"
#define S_COLOR_BLACK				"^9"

// make idStr a multiple of 16 bytes long
// don't make too large to keep memory requirements to a minimum
const int STR_ALLOC_BASE			= 20;
const int STR_ALLOC_GRAN			= 32;

typedef enum {
	MEASURE_SIZE = 0,
	MEASURE_BANDWIDTH
} Measure_t;

class lfStr {

public:
						lfStr( void );
						lfStr( const lfStr &text );
						lfStr( const lfStr &text, int start, int end );
						lfStr( const char *text );
						lfStr( const char *text, int start, int end );
						explicit lfStr( const bool b );
						explicit lfStr( const char c );
						explicit lfStr( const int i );
						explicit lfStr( const unsigned u );
						explicit lfStr( const float f );
						~lfStr( void );

	size_t				Size( void ) const;
	const char *		c_str( void ) const;
	operator			const char *( void ) const;
	operator			const char *( void );

	char				operator[]( int index ) const;
	char &				operator[]( int index );

	void				operator=( const lfStr &text );
	void				operator=( const char *text );

	friend lfStr		operator+( const lfStr &a, const lfStr &b );
	friend lfStr		operator+( const lfStr &a, const char *b );
	friend lfStr		operator+( const char *a, const lfStr &b );

	friend lfStr		operator+( const lfStr &a, const float b );
	friend lfStr		operator+( const lfStr &a, const int b );
	friend lfStr		operator+( const lfStr &a, const unsigned b );
	friend lfStr		operator+( const lfStr &a, const bool b );
	friend lfStr		operator+( const lfStr &a, const char b );

	lfStr &				operator+=( const lfStr &a );
	lfStr &				operator+=( const char *a );
	lfStr &				operator+=( const float a );
	lfStr &				operator+=( const char a );
	lfStr &				operator+=( const int a );
	lfStr &				operator+=( const unsigned a );
	lfStr &				operator+=( const bool a );

						// case sensitive compare
	friend bool			operator==( const lfStr &a, const lfStr &b );
	friend bool			operator==( const lfStr &a, const char *b );
	friend bool			operator==( const char *a, const lfStr &b );

						// case sensitive compare
	friend bool			operator!=( const lfStr &a, const lfStr &b );
	friend bool			operator!=( const lfStr &a, const char *b );
	friend bool			operator!=( const char *a, const lfStr &b );

						// case sensitive compare
	int					Cmp( const char *text ) const;
	int					Cmpn( const char *text, int n ) const;
	int					CmpPrefix( const char *text ) const;

						// case insensitive compare
	int					Icmp( const char *text ) const;
	int					Icmpn( const char *text, int n ) const;
	int					IcmpPrefix( const char *text ) const;

						// case insensitive compare ignoring color
	int					IcmpNoColor( const char *text ) const;

						// compares paths and makes sure folders come first
	int					IcmpPath( const char *text ) const;
	int					IcmpnPath( const char *text, int n ) const;
	int					IcmpPrefixPath( const char *text ) const;

	int					Length( void ) const;
	int					Allocated( void ) const;
	void				Empty( void );
	bool				IsEmpty( void ) const;
	void				Clear( void );
	void				Append( const char a );
	void				Append( const lfStr &text );
	void				Append( const char *text );
	void				Append( const char *text, int len );
	void				Insert( const char a, int index );
	void				Insert( const char *text, int index );
	void				ToLower( void );
	void				ToUpper( void );
	bool				IsNumeric( void ) const;
	bool				IsColor( void ) const;
	bool				HasLower( void ) const;
	bool				HasUpper( void ) const;
	int					LengthWithoutColors( void ) const;
	lfStr &				RemoveColors( void );
	void				CapLength( int );
	void				Fill( const char ch, int newlen );

	int					Find( const char c, int start = 0, int end = -1 ) const;
	int					Find( const char *text, bool casesensitive = true, int start = 0, int end = -1 ) const;
	bool				Filter( const char *filter, bool casesensitive ) const;
	int					Last( const char c ) const;						// return the index to the last occurance of 'c', returns -1 if not found
	const char *		Left( int len, lfStr &result ) const;			// store the leftmost 'len' characters in the result
	const char *		Right( int len, lfStr &result ) const;			// store the rightmost 'len' characters in the result
	const char *		Mid( int start, int len, lfStr &result ) const;	// store 'len' characters starting at 'start' in result
	lfStr				Left( int len ) const;							// return the leftmost 'len' characters
	lfStr				Right( int len ) const;							// return the rightmost 'len' characters
	lfStr				Mid( int start, int len ) const;				// return 'len' characters starting at 'start'
	void				StripLeading( const char c );					// strip char from front as many times as the char occurs
	void				StripLeading( const char *string );				// strip string from front as many times as the string occurs
	bool				StripLeadingOnce( const char *string );			// strip string from front just once if it occurs
	void				StripTrailing( const char c );					// strip char from end as many times as the char occurs
	void				StripTrailing( const char *string );			// strip string from end as many times as the string occurs
	bool				StripTrailingOnce( const char *string );		// strip string from end just once if it occurs
	void				Strip( const char c );							// strip char from front and end as many times as the char occurs
	void				Strip( const char *string );					// strip string from front and end as many times as the string occurs
	void				StripTrailingWhitespace( void );				// strip trailing white space characters
	lfStr &				StripQuotes( void );							// strip quotes around string
	void				Replace( const char *old, const char *nw );

	// file name methods
	int					FileNameHash( void ) const;						// hash key for the filename (skips extension)
	lfStr &				BackSlashesToSlashes( void );					// convert slashes
	lfStr &				SetFileExtension( const char *extension );		// set the given file extension
	lfStr &				StripFileExtension( void );						// remove any file extension
	lfStr &				StripAbsoluteFileExtension( void );				// remove any file extension looking from front (useful if there are multiple .'s)
	lfStr &				DefaultFileExtension( const char *extension );	// if there's no file extension use the default
	lfStr &				DefaultPath( const char *basepath );			// if there's no path use the default
	void				AppendPath( const char *text );					// append a partial path
	lfStr &				StripFilename( void );							// remove the filename from a path
	lfStr &				StripPath( void );								// remove the path from the filename
	void				ExtractFilePath( lfStr &dest ) const;			// copy the file path to another string
	void				ExtractFileName( lfStr &dest ) const;			// copy the filename to another string
	void				ExtractFileBase( lfStr &dest ) const;			// copy the filename minus the extension to another string
	void				ExtractFileExtension( lfStr &dest ) const;		// copy the file extension to another string
	bool				CheckExtension( const char *ext );

	// char * methods to replace library functions
	static int			Length( const char *s );
	static char *		ToLower( char *s );
	static char *		ToUpper( char *s );
	static bool			IsNumeric( const char *s );
	static bool			IsColor( const char *s );
	static bool			HasLower( const char *s );
	static bool			HasUpper( const char *s );
	static int			LengthWithoutColors( const char *s );
	static char *		RemoveColors( char *s );
	static int			Cmp( const char *s1, const char *s2 );
	static int			Cmpn( const char *s1, const char *s2, int n );
	static int			Icmp( const char *s1, const char *s2 );
	static int			Icmpn( const char *s1, const char *s2, int n );
	static int			IcmpNoColor( const char *s1, const char *s2 );
	static int			IcmpPath( const char *s1, const char *s2 );			// compares paths and makes sure folders come first
	static int			IcmpnPath( const char *s1, const char *s2, int n );	// compares paths and makes sure folders come first
	static void			Append( char *dest, int size, const char *src );
	static void			Copynz( char *dest, const char *src, int destsize );
	//static int			snPrintf( char *dest, int size, const char *fmt, ... ) id_attribute((format(printf,3,4)));
	static int			vsnPrintf( char *dest, int size, const char *fmt, va_list argptr );
	static int			FindChar( const char *str, const char c, int start = 0, int end = -1 );
	static int			FindText( const char *str, const char *text, bool casesensitive = true, int start = 0, int end = -1 );
	static bool			Filter( const char *filter, const char *name, bool casesensitive );
	static void			StripMediaName( const char *name, lfStr &mediaName );
	static bool			CheckExtension( const char *name, const char *ext );
	static const char *	FloatArrayToString( const float *array, const int length, const int precision );

	// hash keys
	static int			Hash( const char *string );
	static int			Hash( const char *string, int length );
	static int			IHash( const char *string );					// case insensitive
	static int			IHash( const char *string, int length );		// case insensitive

	// character methods
	static char			ToLower( char c );
	static char			ToUpper( char c );
	static bool			CharIsPrintable( int c );
	static bool			CharIsLower( int c );
	static bool			CharIsUpper( int c );
	static bool			CharIsAlpha( int c );
	static bool			CharIsNumeric( int c );
	static bool			CharIsNewLine( char c );
	static bool			CharIsTab( char c );
	static int			ColorIndex( int c );

	friend int			sprintf( lfStr &dest, const char *fmt, ... );
	friend int			vsprintf( lfStr &dest, const char *fmt, va_list ap );

	void				ReAllocate( int amount, bool keepold );				// reallocate string data buffer
	void				FreeData( void );									// free allocated string memory

						// format value in the given measurement with the best unit, returns the best unit
	int					BestUnit( const char *format, float value, Measure_t measure );
						// format value in the requested unit and measurement
	void				SetUnit( const char *format, float value, int unit, Measure_t measure );

	static void			InitMemory( void );
	static void			ShutdownMemory( void );
	static void			PurgeMemory( void );
	//static void			ShowMemoryUsage_f( const idCmdArgs &args );

	int					DynamicMemoryUsed() const;
	static lfStr		FormatNumber( int number );

protected:
	int					len;
	char *				data;
	int					alloced;
	char				baseBuffer[ STR_ALLOC_BASE ];

	void				Init( void );										// initialize string using base buffer
	void				EnsureAlloced( int amount, bool keepold = true );	// ensure string data buffer is large anough
};



#endif /* !__STR_H__ */
