#ifndef __TIME_ACCESS_H__
#define __TIME_ACCESS_H__

#include "base/system.h"
#include "base/types.h"
#include "base/lib_export.h"

struct DEFINE_EXPORT time_accessor_t {
	
	time_accessor_t();
	time_accessor_t( uint32 );
	~time_accessor_t();
	
	float64 time() const;
	uint32 maxTimers() const;
	
	void setTimer( uint32 = 0 );
	float64 readTimer( uint32 = 0 );
	
private:
	
	time_accessor_t( const time_accessor_t& ) { }
	time_accessor_t& operator= ( const time_accessor_t& ) { return *this; }
	
	float64 frequency;
	float64 current_time;
	float64* timers;
	
};

DEFINE_EXPORTED_FUNCTION( time_accessor_t&, GlobalCurrentTime, () );
#define Time (GlobalCurrentTime()) 

struct DEFINE_EXPORT sleeper_t {
	
	// wait indefinitely
	void wait();
	
	// wait for a number of seconds
	// true if object signal, false if timeout
	bool wait( float64 );
	
	// stop the object if it is waiting
	void signal();
	
	sleeper_t();
	~sleeper_t();
	
	private:
#ifdef __WIN32__
		HANDLE ev;
#else
		volatile bool ev;
#endif
};

#endif//__INFI_TIME_H__
