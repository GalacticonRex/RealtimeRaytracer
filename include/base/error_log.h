#ifndef __ERROR_LOG_H__
#define __ERROR_LOG_H__

#include <cstdarg>
#include <iostream>
#include "base/lib_export.h"
#include "base/types.h"

enum class ERROR_TYPE {
	NONE_ERROR,
	MESSAGE,
	WARNING,
	IGNORED,
	UNIMPLEMENTED,
	SYSTEM,
	LIBRARY,
	GRAPHICS,
	FAILED,
	NULL_DEREF,
	OVERFLOW,
	UNDERFLOW,
	INDEX,
	TYPE,
	INIT,
	BINDING,
	NOT_FOUND,
	CANNOT_READ_FILE,
	UNSUPPORTED_TYPE,
	UNRESOLVED
};

struct DEFINE_EXPORT error_stack_t {
	uint32 count;
	char** data;
	
	error_stack_t();
	error_stack_t( uint32, const char** );
	error_stack_t( const error_stack_t& );
	~error_stack_t();
	
	error_stack_t& operator= ( const error_stack_t& );
};
struct DEFINE_EXPORT error_message_t {
	ERROR_TYPE type;
	error_stack_t func_stack;
	char* msg;
	
	error_message_t();
	error_message_t( ERROR_TYPE, const char* );
	error_message_t( const error_message_t& );
	~error_message_t();
	
	const char* id() const;
	const char* message() const;
	uint32 stackSize() const;
	const char* stack( uint32 ) const;
	
	error_message_t& operator= ( const error_message_t& );
};

class DEFINE_EXPORT error_log_t {
private:
	error_message_t* errbuffer;
	uint32 alloc;
	uint32 inuse;
	uint32 start;
	uint32 current;
	
	const char** errstack;
	uint32 stalloc;
	uint32 stinuse;
	
	void realloc_buffer();
	void realloc_stack();
	
	void push_error( const error_message_t& );
	void pop_error( error_message_t& );
	char* write_error( ERROR_TYPE, const char*, va_list );
	
public:
	error_log_t();
	error_log_t( const error_log_t& );
	~error_log_t();
	
	error_log_t& operator= ( const error_log_t& );
	
	void push( const char* );
	void pop();
	
	void send( const char*, ERROR_TYPE, const char*, ... );
	void send( ERROR_TYPE, const char*, ... );
	
	void log( const char*, ERROR_TYPE, const char*, ... );
	void log( ERROR_TYPE, const char*, ... );
	
	uint32 how_many() const;
	bool get( error_message_t& );
	bool peek( error_message_t& ) const;
	
	void clear();
};

DEFINE_EXPORTED_FUNCTION( const char*, ErrorString, ( ERROR_TYPE ) );
DEFINE_EXPORTED_FUNCTION( error_log_t&, GetCurrentErrorLog, () );

#define FUNCTION_RETURN( x ) GetCurrentErrorLog().pop(); return x;
#define Error (GetCurrentErrorLog())

DEFINE_EXPORTED_FUNCTION( std::ostream&, operator<<, (std::ostream&,const error_message_t&) );

#endif//__ERROR_LOG_H__