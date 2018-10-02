#include "base/error_log.h"
#include "base/time.h"

#ifdef __WIN32__

	#include <iostream>
	
	time_accessor_t::time_accessor_t() :
		current_time( 0.0 ),
		timers( new float64[16] ) {
		LARGE_INTEGER li;
		QueryPerformanceFrequency( &li );
		frequency = (float64) li.QuadPart;
	}
	time_accessor_t::time_accessor_t( uint32 count ) :
		current_time( 0.0 ),
		timers( new float64[count] ) {
		LARGE_INTEGER li;
		QueryPerformanceFrequency( &li );
		frequency = (float64) li.QuadPart;
	}
	float64 time_accessor_t::time() const {
		LARGE_INTEGER li;
		QueryPerformanceCounter( &li );
		return (float64)li.QuadPart / frequency;
	}
	
	sleeper_t::sleeper_t() {
		ev = CreateEvent( NULL, TRUE, FALSE, "SleeperEvent" );
		ResetEvent( ev );
	}
	sleeper_t::~sleeper_t() { CloseHandle( ev ); }
	
	void sleeper_t::wait() {
		(void) WaitForSingleObject( ev, INFINITE );
		ResetEvent( ev );
	}
	
	bool sleeper_t::wait( float64 s ) {
		uint32 ms = s * 1000.0;
		uint32 tmp = WaitForSingleObject( ev, ms );
		ResetEvent( ev );
		return ( tmp == WAIT_OBJECT_0 );
	}
	
	void sleeper_t::signal() {
		SetEvent( ev );
	}
	
#else
	
	#include <ctime>
	#include <iostream>
	
	time_accessor_t::time_accessor_t() :
		frequency( 1000.0 ),
		current_time( 0.0 ),
		timers( new float64[16] ) { }
	time_accessor_t::time_accessor_t( uint32 count ) :
		frequency( 1000.0 ),
		current_time( 0.0 ),
		timers( new float64[count] ) { }
	float64 time_accessor_t::time() const {
		return (float64)std::time(NULL) / frequency;
	}
	
	sleeper_t::sleeper_t() : ev(false) {  }
	sleeper_t::~sleeper_t() { }
	
	void sleeper_t::wait() {
		ev = false;
		while ( !ev );
	}
	
	bool sleeper_t::wait( float64 s ) {
		float64 t = Time.time() + s;
		ev = false;
		while ( Time.time() < t && !ev );
		return ev;
	}
	
	void sleeper_t::signal() { ev = true; }
	
#endif//__WIN32__

time_accessor_t::~time_accessor_t() {
	delete[] timers;
}
uint32 time_accessor_t::maxTimers() const {
	return sizeof(*timers);
}

void time_accessor_t::setTimer( uint32 index ) {
	if ( index >= maxTimers() )
		Error.send( "time_accessor_t.setTimer",
					ERROR_TYPE::INDEX,
					"index %d index out of range of %d", index, maxTimers() );
	timers[index] = this->time();
}
float64 time_accessor_t::readTimer( uint32 index ) {
	if ( index >= maxTimers() )
		Error.send( "time_accessor_t.setTimer",
					ERROR_TYPE::INDEX,
					"index %d index out of range of %d", index, maxTimers() );
	return this->time() - timers[index];
}

time_accessor_t& GlobalCurrentTime() {
	static thread_local time_accessor_t internal;
	return internal;
}