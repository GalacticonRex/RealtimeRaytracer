#ifndef __CORE_CONTIGUOUS_H__
#define __CORE_CONTIGUOUS_H__

#include <cstring>
#include <iostream>	
#include "base/types.h"
#include "base/error_log.h"

namespace core {
	
	/*
	contiguous_t
		alloc();
		initialized();
		reference()
		user();
		is();
		compare();
		as();
		to();
		at_index();
		shift_left();
		shift_right();
		
	*/
	
	template<typename T,uint32 USER = 0>
	class contiguous_t {
	private:
		
		static const uint32 DATA_ALLOC  = 1; // how big is the memory allocated
		static const uint32 DATA_INIT   = 2; // how many objects have been _initialized
		static const uint32 DATA_REFER  = 3; // how many references to this object
		static const uint32 DATA_USER   = 4; // user specified data
		
		T* internal;
		
		template<typename T2>
		static uint32 __alloc_get( T2* input ) {
			return *((uint32*)input-DATA_ALLOC) / sizeof(T2);
		}
		template<typename T2>
		static uint32 __initialized_get( T2* input ) {
			return *((uint32*)input-DATA_INIT) / sizeof(T2);
		}
		template<typename T2>
		static void __alloc_set( T2* input, uint32 n ) {
			*((uint32*)input-DATA_ALLOC) = n * sizeof(T2);
		}
		template<typename T2>
		static void __initialized_set( T2* input, uint32 n ) {
			*((uint32*)input-DATA_INIT) = n * sizeof(T2);
		}
				
		static uint32& __reference( void* input ) {
			return *((uint32*)input-DATA_REFER);
		}
		static uint32* __user( void* input ) {
			return (uint32*)input-DATA_USER;
		}
		
		static T* __create( uint32 sz ) {
			uint32* ndata = (uint32*) operator new[]( sizeof(T) * sz + sizeof(uint32) * (DATA_REFER+USER) );
			
			ndata[USER+0] = 1; // set number of references to 1
			ndata[USER+1] = 0; // set init value to 0
			ndata[USER+2] = sz * sizeof(T); // set allocation size to <sz>
			
			T* ret = (T*)( ndata + DATA_REFER + USER );
			return ret;
		}
		static void __init_default_last( T* data ) {
			uint32 get = __initialized_get( data );
			if ( __alloc_get( data ) == get )
				return;
			new (data+get) T();
			__initialized_set( data, get + 1 );
		}
		static void __init_default( T* data, uint32 x ) {
			x = std::min( __alloc_get( data ), x );
			for ( uint32 i=__initialized_get(data);i<x;++i )
				new (data+i) T();
			if ( __initialized_get(data) < x )
				__initialized_set( data, x );
		}
		static void __init_proto_last( T* data, const T& proto ) {
			uint32 get = __initialized_get( data );
			if ( __alloc_get( data ) == get )
				return;
			new (data+get) T( proto );
			__initialized_set( data, get + 1 );
		}
		static void __init_proto( T* data, uint32 x, const T& proto ) {
			x = std::min( __alloc_get( data ), x );
			for ( uint32 i=__initialized_get(data);i<x;++i )
				new (data+i) T( proto );
			if ( __initialized_get(data) < x )
				__initialized_set( data, x );
		}
		static void __deinit( T* data, uint32 from_right = 0 ) {
			uint32 len = __initialized_get( data );
			for ( uint32 i=from_right;i<len;++i )
				data[i].~T();
		}
		static void __destroy( T* data, bool destruct ) {
			if ( destruct )
				__deinit( data );
			operator delete[]( (uint32*)data-(DATA_REFER+USER) );
		}
		
		static void __increment( T* data ) {
			++ __reference(data);
		}
		static void __decrement( T* data, bool destruct = true ) {
			if ( -- __reference(data) == 0 )
				__destroy( data, destruct );
		}
		
		uint32 _alloc_get() const {
			return __alloc_get( internal );
		}
		uint32 _initialized_get() const {
			return __initialized_get( internal );
		}
		void _alloc_set( uint32 n ) {
			__alloc_set( internal, n );
		}
		void _initialized_set( uint32 n ) {
			__initialized_set( internal, n );
		}
		
		uint32& _reference() {
			return __reference( internal );
		}
		
		void _datafork( uint32 needed ) {
			if ( _reference() != 1 || _alloc_get() < needed ) {
				T* ndata = __create( needed );
				__initialized_set( ndata, std::min( _initialized_get(), needed ) );
				
				uint32 al1 = __initialized_get( ndata );
				uint32 al2 = _initialized_get();
				memcpy( ndata, internal, std::min( al1, al2 ) * sizeof(T) );
				
				for ( uint32 i=0;i<USER;++i )
					*(__user(ndata)-i) = *(__user(internal)-i);
				
				__decrement( internal, false );
				internal = ndata;
			} else if ( _reference() == 1 && _initialized_get() > needed ) {
				__deinit( internal, needed );
				_initialized_set( needed );
			}
		}
		
	public:
		
		uint32 alloc() const {
			return _alloc_get();
		}
		uint32 initialized() const {
			return _initialized_get();
		}
		uint32 reference() const {
			return __reference( internal );
		}
		
		uint32& user( uint32 n ) {
			if ( n >= USER )
				Error.send( "contiguous_t.user",
							ERROR_TYPE::INDEX,
							"indexing location %d out of range of %d",
							n, USER );
			return *(__user(internal) - n);
		}
		const uint32& user( uint32 n ) const {
			if ( n >= USER )
				Error.send( "contiguous_t.user",
							ERROR_TYPE::INDEX,
							"indexing location %d out of range of %d",
							n, USER );
			return *(__user(internal) - n);
		}
		
		contiguous_t() :
			internal( __create(0) ) { ; }
			
		contiguous_t( uint32 req ) :
			internal( __create(req) ) { ; }
			
		contiguous_t( std::initializer_list<T> stdlist ) :
			internal( __create( stdlist.end() - stdlist.begin() ) ) {
			for ( auto i=stdlist.begin();i!=stdlist.end();++i )
				__init_proto_last( internal, *i );
			__increment( internal );
		}
		
		contiguous_t( const contiguous_t& copy ) :
			internal( copy.internal ) {
			__increment( internal );
		}
		
		virtual ~contiguous_t() {
			__decrement( internal );
		}
		
		template<typename T2>
		explicit contiguous_t( const contiguous_t<T2,USER>& copy ) :
			internal( (T*) copy.source() ) { 
			__increment( internal );
		}
		template<typename T2, uint32 USER2>
		explicit contiguous_t( const contiguous_t<T2,USER2>& copy ) :
			internal( __create( copy.alloc() ) ) { 
			splice( copy, copy.initialized() );
		}
		
		contiguous_t& operator= ( const contiguous_t& copy ) {
			__increment( copy.internal );
			__decrement( internal );
			internal = copy.internal;
			return *this;
		}
		
		template<typename T2,uint32 USER2>
		bool is( const contiguous_t<T2,USER2>& other ) const {
			return ( (void*) internal == (void*) other.source() );
		}
		bool is( const T* other ) const {
			return ( (void*)internal <= (void*)other &&
					 (void*)(internal-other) < (void*)__alloc_get( internal ) );
		}
		
		// -1 lt, 0 eq, 1 gt
		int compare( const contiguous_t<T,USER>& other ) const {
			if ( other.is( *this ) )
				return 0;
			else if ( initialized() < other.initialized() )
				return -1;
			else if ( initialized() > other.initialized() )
				return 1;
			else {
				for ( uint32 i=0;i<initialized();++i )
					if ( other.internal[i] != internal[i] )
						return ( other.internal[i] < internal[i] ) ? 1 : -1;
				return 0;
			}
		}
		int compare( const T* other, uint32 len ) const {
			if ( initialized() < len )
				return -1;
			else if ( initialized() > len )
				return 1;
			for ( uint32 i=0;i<initialized();++i )
				if ( other[i] != internal[i] )
					return ( other[i] < internal[i] ) ? 1 : -1;
			return 0;
		}
		
		
		bool operator== ( const contiguous_t<T,USER>& other ) const {
			if ( other.is( *this ) )
				return true;
			else if ( other.initialized() == initialized() ) {
				for ( uint32 i=0;i<other.initialized();++i )
					if ( !(internal[i] == other.internal[i]) )
						return false;
				return true;
			} else
				return false;
		}
		bool operator!= ( const contiguous_t<T,USER>& other ) const {
			if ( other.is( *this ) )
				return false;
			else if ( other.initialized() != initialized() )
				return true;
			else {
				for ( uint32 i=0;i<other.initialized();++i )
					if ( !(internal[i] == other.internal[i]) )
						return true;
				return false;
			}
		}
		
		bool operator< ( const contiguous_t<T,USER>& other ) const {
			if ( other.is( *this ) || other.initialized() < initialized() )
				return false;
			else {
				for ( uint32 i=0;i<initialized();++i )
					if ( internal[i] < other.internal[i] )
						return true;
				return false;
			}
		}
		bool operator<= ( const contiguous_t<T,USER>& other ) const {
			if ( other.is( *this ) )
				return true;
			else if ( other.initialized() < initialized() )
				return false;
			else {
				for ( uint32 i=0;i<initialized();++i )
					if ( internal[i] > other.internal[i] )
						return false;
				return true;
			}
		}
		bool operator> ( const contiguous_t<T,USER>& other ) const {
			if ( other.is( *this ) || other.initialized() > initialized() )
				return false;
			else {
				for ( uint32 i=0;i<other.initialized();++i )
					if ( internal[i] > other.internal[i] )
						return true;
				return false;
			}
		}
		bool operator>= ( const contiguous_t<T,USER>& other ) const {
			if ( other.is( *this ) )
				return true;
			else if ( other.initialized() > initialized() )
				return false;
			else {
				for ( uint32 i=0;i<other.initialized();++i )
					if ( internal[i] < other.internal[i] )
						return false;
				return true;
			}
		}
		
		template<typename T2>
		contiguous_t<T2,USER> as() const {
			return contiguous_t<T2,USER>( *this );
		}
		template<typename T2>
		contiguous_t<T2,USER> to() const {
			contiguous_t<T2,USER> ret( initialized() );
			ret.splice( *this, initialized() );
			return ret;
		}
		
		T& at_index( uint32 n ) {
			if ( n >= _alloc_get() )
				Error.send( "contiguous_t.at_index",
							ERROR_TYPE::INDEX,
							"indexing location %d out of range of %d",
							n, _alloc_get() );
			__init_default( internal, n + 1 );
			return internal[n];
		}
		T& at_index( uint32 n, const T& copy ) {
			if ( n >= _alloc_get() )
				Error.send( "contiguous_t.at_index_copy",
							ERROR_TYPE::INDEX,
							"indexing location %d out of range of %d",
							n, _alloc_get() );
			__init_proto( internal, n + 1, copy );
			return internal[n];
		}
		const T& at_index( uint32 n ) const {
			if ( n >= _initialized_get() )
				Error.send( "contiguous_t.at_index",
							ERROR_TYPE::INDEX,
							"const indexing location %d out of range of %d",
							n, _initialized_get() );
			return internal[n];
		}
		const T& operator[] ( uint32 n ) const {
			return this->at_index( n );
		}
		
		void shift_left( uint32 offset, uint32 amount ) {
			if ( amount > offset )
				Error.send( "contiguous_t.shift_left",
							ERROR_TYPE::UNDERFLOW,
							"cannot shift elements at %d by %d",
							offset, amount );
			
			_datafork( _alloc_get() );
			char* src = (char*) internal;
			
			uint32 tomove = sizeof(T) * _initialized_get();
			
			for ( uint32 i=offset * sizeof(T);i<tomove;++i )
				src[i-amount * sizeof(T)] = src[i];
			
			_initialized_set( _initialized_get() - amount );
		}
		void shift_right( uint32 offset, uint32 amount ) {
			if ( _initialized_get() + amount > _alloc_get() )
				_datafork( _initialized_get() + amount );
			else
				_datafork( _alloc_get() );
			
			char* src = (char*) internal;
			
			_initialized_set( _initialized_get() + amount );
			int32 tomove = sizeof(T) * _initialized_get();
			
			for ( uint32 i=tomove;i>(offset+amount)*sizeof(T);--i )
				src[i-1] = src[i-amount*sizeof(T)-1];
			
			for ( uint32 i=offset;i<offset+amount;++i )
				new (internal+i) T();
		}
		
		void initialize() {
			__init_default( internal, _alloc_get() );
		}
		void initialize_count( uint32 n ) {
			__init_default( internal, n );
		}
		
		void initialize( const T& proto ) {
			__init_proto( internal, _alloc_get(), proto );
		}
		void initialize_count( uint32 n, const T& proto ) {
			__init_proto( internal, n, proto );
		}
		
		void modify() {
			_datafork( _alloc_get() );
		}
		void require( uint32 n ) {
			_datafork(n);
		}
		
		template<typename T2>
		void splice( std::initializer_list<T2> stdlist, uint32 to ) {
			uint32 span = stdlist.end() - stdlist.begin();
			
			if ( to + span > _alloc_get() )
				_datafork( to + span );
			else
				_datafork( _alloc_get() );
			
			for ( uint32 j=to;j<_initialized_get();++j )
				internal[j].~T();
			
			auto iter = stdlist.begin();
			for ( uint32 i=to;iter!=stdlist.end();++i,++iter )
				new (internal+i) T(*iter);
			
			_initialized_set( std::max( _initialized_get(), to + span ) );
		}
		template<typename T2>
		void splice( std::initializer_list<T2> stdlist ) {
			splice( stdlist, 0 );
		}
		template<typename T2,uint32 USER2>
		void splice( const contiguous_t<T2,USER2>& input, uint32 from, uint32 to, uint32 span ) {
			if ( from + span > input.initialized() )
				Error.send( "contiguous_t.splice",
							ERROR_TYPE::INDEX,
							"cannot index input locations %d to %d from %d sized block",
							from, from+span, input.initialized() );
			
			if ( to + span > _alloc_get() )
				_datafork( to + span );
			else
				_datafork( _alloc_get() );
			
			uint32 termin = from + span;
			
			if ( input.is( *this ) ) {
				contiguous_t<T,USER> buffer( input );
				buffer.modify();
				
				for ( uint32 j=to;j<_initialized_get();++j )
					internal[j].~T();
				for ( uint32 i=from,j=to;i<termin;++i,++j )
					new (internal+j) T(buffer.internal[i]);
				
			} else {
				
				for ( uint32 j=to;j<_initialized_get();++j )
					internal[j].~T();
				for ( uint32 i=from,j=to;i<termin;++i,++j )
					new (internal+j) T(input.source()[i]);
				
			}
			
			_initialized_set( std::max( _initialized_get(), to + span ) );
		}
		template<typename T2,uint32 USER2>
		void splice( const contiguous_t<T2,USER2>& input, uint32 span ) {
			splice<T2,USER2>( input, 0, 0, span );
		}
		
		void splice( const T* input, uint32 to, uint32 span ) {
			if ( to + span > _alloc_get() )
				_datafork( to + span );
			else
				_datafork( _alloc_get() );
			
			if ( this->is( input ) ) {
				Error.send( "contiguous_t.splice",
							ERROR_TYPE::UNIMPLEMENTED,
							"cannot splice raw data into itself" );
			} else {
				for ( uint32 j=to;j<_initialized_get();++j )
					internal[j].~T();
				for ( uint32 i=0,j=to;i<span;++i,++j )
					new (internal+j) T(input[i]);
			}
			
			_initialized_set( std::max( _initialized_get(), to + span ) );
		}
		
		template<uint32 USER2>
		contiguous_t<T,USER> concat( const contiguous_t<T,USER2>& input ) {
			contiguous_t<T,USER> ret;
			
			uint32 i1 = _initialized_get();
			uint32 i2 = input.initialized();
			
			ret.require( i1 + i2 );
			
			ret.splice<USER>( *this, 0, 0, i1 );
			ret.splice<USER>( input, 0, i1, i2 );
			
			for ( uint32 i=0;i<USER;++i )
				ret.user(i) = *(__user(internal)-i);
			
			return ret;
		}
		
		contiguous_t<T,USER> concat_to_front( const T* input, uint32 len ) {
			contiguous_t<T,USER> ret;
			
			uint32 i1 = len;
			uint32 i2 = _initialized_get();
			
			ret.require( i1 + i2 );
			
			ret.splice<USER>( input, 0, i1 );
			ret.splice<USER>( *this, 0, i1, i2 );
			
			for ( uint32 i=0;i<USER;++i )
				ret.user(i) = *(__user(internal)-i);
			
			return ret;
		}
		contiguous_t<T,USER> concat( const T* input, uint32 len ) {
			contiguous_t<T,USER> ret;
			
			uint32 i1 = _initialized_get();
			uint32 i2 = len;
			
			ret.require( i1 + i2 );
			
			ret.splice<USER>( *this, 0, 0, i1 );
			ret.splice<USER>( input, i1, i2 );
			
			for ( uint32 i=0;i<USER;++i )
				ret.user(i) = *(__user(internal)-i);
			
			return ret;
		}
		
		template<uint32 USER2>
		void append( const contiguous_t<T,USER2>& input ) {
			uint32 i1 = _initialized_get();
			uint32 i2 = input.initialized();
			
			_datafork( i1 + i2 );
			this->splice( input, 0, i1, i2 );
		}
		void append( const T* input, uint32 len ) {
			uint32 i1 = _initialized_get();
			uint32 i2 = len;
			
			_datafork( i1 + i2 );
			this->splice( input, i1, i2 );
		}
		
		T* source() {
			return internal;
		}
		const T* source() const {
			return internal;
		}
	};
	
	template<typename T,uint32 USER = 0>
	std::ostream& operator<< ( std::ostream& ostr, const contiguous_t<T,USER>& c ) {
		ostr << "{ ";
		uint32 i;
		for ( i=0;i<c.initialized();++i )
			ostr << c.at_index(i) << " ";
		for ( ;i<c.alloc();++i )
			ostr << ". ";
		ostr << "} ";
		ostr << "(in " << c.initialized() << ") ";
		ostr << "(al " << c.alloc() << ") ";
		ostr << "(re " << c.reference() << ") ";
		for ( i=0;i<USER;++i )
			ostr << "(" << c.user(i) << ") ";
		return ostr;
	}
	
}

#endif//__CORE_CONTIGUOUS_H__