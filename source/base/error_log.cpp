#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <thread>
#include "base/error_log.h"

static const char* no_message = "no message available";

error_stack_t::error_stack_t() :
	count( 0 ),
	data( NULL ) { ; }
	
error_stack_t::error_stack_t( uint32 c, const char** d ) :
	count( c ),
	data( new char*[c] ) {
	for ( uint32 i=0;i<count;++i ) {
		uint32 len = strlen( d[i] );
		data[i] = new char[len+1];
		strcpy( data[i], d[i] );
		data[i][len] = '\0';
	}
}
error_stack_t::error_stack_t( const error_stack_t& copy ) :
	count( copy.count ),
	data( new char*[count] ) {
	for ( uint32 i=0;i<count;++i ) {
		uint32 len = strlen(copy.data[i]);
		data[i] = new char[len+1];
		memcpy( data[i], copy.data[i], len+1 );
	}
}
error_stack_t::~error_stack_t() {
	for ( uint32 i=0;i<count;++i )
		delete[] data[i];
	delete[] data;
}

error_stack_t& error_stack_t::operator= ( const error_stack_t& copy ) {
	for ( uint32 i=0;i<count;++i )
		delete[] data[i];
	delete[] data;
	
	count = copy.count;
	data = new char*[count];
	for ( uint32 i=0;i<count;++i ) {
		uint32 len = strlen(copy.data[i]);
		data[i] = new char[len+1];
		memcpy( data[i], copy.data[i], len+1 );
	}
	
	return *this;
}

error_message_t::error_message_t() :
	type(ERROR_TYPE::NONE_ERROR) {
	uint32 len = strlen( no_message );
	msg = new char[len+1];
	strcpy( msg, no_message );
	msg[len] = '\0';
}

error_message_t::error_message_t( ERROR_TYPE id, const char* str ) :
	type( id ) {
	uint32 len = strlen(str);
	msg = new char[len+1];
	strcpy( msg, str );
	msg[len] = '\0';
}

error_message_t::error_message_t( const error_message_t& copy ) :
	type(copy.type),
	func_stack(copy.func_stack),
	msg( new char[strlen(copy.msg)+1] ) {
	strcpy( msg, copy.msg );
}

error_message_t& error_message_t::operator= ( const error_message_t& copy ) {
	delete[] msg;
	type = copy.type;
	func_stack = copy.func_stack;
	msg = new char[strlen(copy.msg)+1];
	strcpy( msg, copy.msg );
	return *this;
}

error_message_t::~error_message_t() {
	delete[] msg;
}

const char* error_message_t::id() const {
	return ErrorString( type );
}
const char* error_message_t::message() const {
	return msg;
}
uint32 error_message_t::stackSize() const {
	return func_stack.count;
}
const char* error_message_t::stack( uint32 i ) const {
	if ( i >= func_stack.count )
		return NULL;
	return func_stack.data[i];
}

void error_log_t::realloc_buffer() {
	uint32 old = alloc;
	alloc = ( alloc == 0 ) ? 8 : alloc << 1;
	
	error_message_t* nbuffer = new error_message_t[alloc];
	for ( uint32 i=0;i<inuse;++i )
		nbuffer[i] = errbuffer[(i+start)%old];
	
	start = 0;
	current = inuse;
	
	delete[] errbuffer;
	errbuffer = nbuffer;
}
void error_log_t::realloc_stack() {
	stalloc = ( stalloc == 0 ) ? 8 : stalloc << 1;
		
	uint32 len = sizeof(const char*) * stalloc;
	const char** nstack = (const char**) malloc( len );
	memcpy( nstack, errstack, sizeof(const char*) * stinuse );
	free( errstack );
	
	errstack = nstack;
}

void error_log_t::push_error( const error_message_t& err ) {
	if ( inuse == alloc )
		realloc_buffer();
	
	errbuffer[current] = err;
	current = (current + 1) % alloc;
	++ inuse;
}
void error_log_t::pop_error( error_message_t& err ) {
	err = errbuffer[start];
	start = (start + 1) % alloc;
	-- inuse;
}
char* error_log_t::write_error( ERROR_TYPE id, const char* format, va_list args ) {
	char* buffer = new char[256];
	vsprintf( buffer, format, args );
	
	error_message_t err( id, buffer );
	err.func_stack = error_stack_t( stinuse, errstack );
	
	push_error( err );
	
	return buffer;
}

error_log_t::error_log_t() :
	errbuffer( NULL ),
	alloc( 0 ),
	inuse( 0 ),
	start( 0 ),
	current( 0 ),
	errstack( NULL ),
	stalloc( 0 ),
	stinuse( 0 ) { ; }
	
error_log_t::error_log_t( const error_log_t& copy ) :
	errbuffer( new error_message_t[copy.inuse] ),
	alloc( copy.inuse ),
	inuse( copy.inuse ),
	start( 0 ),
	current( copy.inuse ),
	errstack( new const char*[stinuse] ),
	stalloc( copy.stinuse ),
	stinuse( copy.stinuse ) {
	for ( uint32 i=0;i<inuse;++i ) {
		uint32 j = (i + copy.start) % copy.alloc;
		errbuffer[i] = copy.errbuffer[j];
	}
	memcpy( errstack, copy.errstack, sizeof(const char*) * stinuse );
}
error_log_t::~error_log_t() {
	delete[] errbuffer;
	delete[] errstack;
}

error_log_t& error_log_t::operator= ( const error_log_t& copy ) {
	alloc = copy.inuse;
	inuse = copy.inuse;
	start = 0;
	current = copy.inuse;
	stalloc = copy.stinuse;
	stinuse = copy.stinuse;
	
	errbuffer = new error_message_t[copy.inuse];
	errstack = new const char*[stinuse];
	
	for ( uint32 i=0;i<inuse;++i ) {
		uint32 j = (i + copy.start) % copy.alloc;
		errbuffer[i] = copy.errbuffer[j];
	}
	memcpy( errstack, copy.errstack, sizeof(const char*) * stinuse );
	
	return *this;
}

void error_log_t::push( const char* fname ) {
	if ( stinuse == stalloc )
		realloc_stack();
	errstack[stinuse] = fname;
	++ stinuse;
}
void error_log_t::pop() {
	if ( stinuse != 0 )
		-- stinuse;
}

void error_log_t::send( const char* scope, ERROR_TYPE id, const char* msg, ... ) {
	this->push( scope );
	
	va_list args;
	va_start( args, msg );
	this->write_error( id, msg, args );
	va_end( args );
	
	std::cerr << msg << std::endl;
	throw id;
	
	this->pop();
}
void error_log_t::send( ERROR_TYPE id, const char* msg, ... ) {
	va_list args;
	va_start( args, msg );
	this->write_error( id, msg, args );
	va_end( args );
	
	std::cerr << msg << std::endl;
	throw id;
}
void error_log_t::log( const char* scope, ERROR_TYPE id, const char* msg, ... ) {
	this->push( scope );
	
	va_list args;
	va_start( args, msg );
	this->write_error( id, msg, args );
	va_end( args );
	
	this->pop();
}
void error_log_t::log( ERROR_TYPE id, const char* msg, ... ) {
	va_list args;
	va_start( args, msg );
	this->write_error( id, msg, args );
	va_end( args );
}

static bool no_error( error_message_t& out ) {
	out = error_message_t( ERROR_TYPE::NONE_ERROR,
						   no_message );
	return false;
}
uint32 error_log_t::how_many() const {
	return inuse;
}
bool error_log_t::get( error_message_t& out ) {
	if ( inuse == 0 )
		return no_error( out );
	pop_error( out );
	return true;
}
bool error_log_t::peek( error_message_t& out ) const {
	if ( inuse == 0 )
		return no_error( out );
	out = errbuffer[start];
	return true;
}

void error_log_t::clear() {
	start = 0;
	current = 0;
	delete[] errbuffer;
	errbuffer = NULL;
}

error_log_t& GetCurrentErrorLog() {
	static thread_local error_log_t internal;
	return internal;
}

const char* ErrorString( ERROR_TYPE type ) {
	switch( type ) {
		case ERROR_TYPE::NONE_ERROR:
			return "No Error"; break;
		
		case ERROR_TYPE::MESSAGE:
			return "Message"; break;
		case ERROR_TYPE::WARNING:
			return "Warning"; break;
		case ERROR_TYPE::IGNORED:
			return "Operation Ignored"; break;
		case ERROR_TYPE::UNIMPLEMENTED:
			return "Unimplemented Operation"; break;
		
		case ERROR_TYPE::SYSTEM:
			return "Local Library Error"; break;
		case ERROR_TYPE::LIBRARY:
			return "External Library Error"; break;
		case ERROR_TYPE::GRAPHICS:
			return "Graphics Library Error"; break;
		
		case ERROR_TYPE::FAILED:
			return "Operation Failed"; break;
		case ERROR_TYPE::NULL_DEREF:
			return "Null Pointer Dereference"; break;
		case ERROR_TYPE::OVERFLOW:
			return "Overflow Error"; break;
		case ERROR_TYPE::UNDERFLOW:
			return "Underflow Error"; break;
		case ERROR_TYPE::INDEX:
			return "Index Out Of Range"; break;
		case ERROR_TYPE::TYPE:
			return "Incompatible Types"; break;
		case ERROR_TYPE::INIT:
			return "Initialization Failed"; break;	
		case ERROR_TYPE::BINDING:
			return "Binding Failed"; break;	
			
		case ERROR_TYPE::NOT_FOUND:
			return "File Was Not Found"; break;
		case ERROR_TYPE::CANNOT_READ_FILE:
			return "File Is Corrupted"; break;
		case ERROR_TYPE::UNSUPPORTED_TYPE:
			return "File Type Is Not Supported"; break;
		case ERROR_TYPE::UNRESOLVED:
			return "File Operation is Unresolved"; break;
		
		default:
			return "Unknown Error";
	}
}

std::ostream& operator<<(std::ostream& ostr, const error_message_t& msg) {
	ostr << msg.id() << " : " << msg.message() << std::endl;
	for ( uint32 i=0;i<msg.stackSize();++i ) {
		ostr << " -- " << msg.stack(i) << std::endl;
	}
	return ostr;
}