#ifndef __CORE_ARRAY_H__
#define __CORE_ARRAY_H__

#include "core/indexable.h"
#include "core/contiguous.h"

namespace core {
	
	template<typename T>
	struct array_t : public contiguous_t<T,1>,
					 public indexable_t<T> {
	private:
		
		uint32& get_size() {
			return contiguous_t<T,1>::user(0);
		}
		const uint32& get_size() const {
			return contiguous_t<T,1>::user(0);
		}
		
		void realloc_array( uint32 need ) {
			Error.push( "array_t.realloc_array" );
			if ( get_size() + need > contiguous_t<T,1>::alloc() ) {
				contiguous_t<T,1>::require( 
					std::max( contiguous_t<T,1>::alloc() << 1, 8u ) );
			}
			Error.pop();
		}
		
	public:
		
		array_t() { get_size() = 0; }
		array_t( const array_t<T>& other )
			: contiguous_t<T,1>( other ) { ; }
			
		template<uint32 USER>
		explicit array_t( const contiguous_t<T,USER>& other )
			: contiguous_t<T,1>( other ) { 
			get_size() = other.initialized();
		}
		
		uint32 size() const {
			return get_size();
		}
		uint32 last() const {
			return ( get_size() == 0 ) ? 0 : get_size() - 1;
		}
		
		void resize( uint32 n ) {
			contiguous_t<T,1>::require( n );
			get_size() = n;
		}
		void push( const T& x ) {
			realloc_array( 1 );
			contiguous_t<T,1>::at_index( get_size() ++ ) = x;
		}
		void push( std::initializer_list<T> stdlist ) {
			Error.push( "array.push" );
			contiguous_t<T,1>::splice( stdlist, get_size() );
			get_size() += stdlist.end() - stdlist.begin();
		}
		void push( const array_t<T>& arr ) {
			Error.push( "array.push" );
			contiguous_t<T,1>::splice( arr, 0, get_size(), arr.size() );
			get_size() += arr.size();
		}
		void insert( uint32 index, const T& x ) {
			contiguous_t<T,1>::shift_right( index, 1 );
			contiguous_t<T,1>::at_index( index ) = x;
			++ get_size();
		}
		
		void pop() {
			if ( get_size() == 0 )
				Error.send( "array_t.remove",
							ERROR_TYPE::INDEX,
							"cannot remove element from empty array" );
			contiguous_t<T,1>::shift_left( -- get_size(), 1 );
		}
		void remove( uint32 index ) {
			if ( index >= get_size() )
				Error.send( "array_t.remove",
							ERROR_TYPE::INDEX,
							"Attempted to remove from location %d, maximum %d",
							index, get_size() );
			contiguous_t<T,1>::shift_left( index, 1 );
			-- get_size();
		}
		void clear() {
			get_size() = 0;
		}
		
		T& operator* () {
			if ( get_size() == 0 )
				Error.send( "*array_t",
							ERROR_TYPE::INDEX,
							"cannot dereference empty array" );
			return contiguous_t<T,1>::at_index( get_size()-1 );
		}
		const T& operator* () const {
			if ( get_size() == 0 )
				Error.send( "*array_t",
							ERROR_TYPE::INDEX,
							"cannot dereference empty array" );
			return contiguous_t<T,1>::at_index( get_size()-1 );
		}
		T& operator[] ( uint32 n ) {
			if ( n >= get_size() )
				Error.send( "array_t.[]",
							ERROR_TYPE::INDEX,
							"indexing location %d out of range of %d",
							n, get_size() );
			return contiguous_t<T,1>::at_index( n );
		}
		const T& operator[] ( uint32 n ) const {
			if ( n >= get_size() )
				Error.send( "array_t.[]",
							ERROR_TYPE::INDEX,
							"indexing location %d out of range of %d",
							n, get_size() );
			return contiguous_t<T,1>::at_index( n );
		}
		
	};
	
	template<typename T>
	std::ostream& operator<< ( std::ostream& ostr, const array_t<T>& c ) {
		uint32 i = 0;
		std::cerr << "[";
		for ( ;i<c.last();++i )
			std::cerr << c[i] << ", ";
		if ( i<c.size() )
			std::cerr << c[i];
		std::cerr << "]";
		return ostr;
	}
}

#endif//__CORE_ARRAY_H__