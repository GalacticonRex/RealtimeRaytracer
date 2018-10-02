#ifndef __FLOAT_VECTOR_FAST_H__
#define __FLOAT_VECTOR_FAST_H__

#include <xmmintrin.h>
#include <smmintrin.h>
#include <cmath>
#include <iostream>
#include "base/types.h"
#include "base/lib_export.h"

namespace core {
	
	struct DEFINE_EXPORT fvec {
		__m128 data;
		
		fvec();
		fvec( float32, float32, float32 );
		fvec( float32, float32, float32, float32 );
		fvec( const __m128& );
		fvec( const fvec& );
		
		float32 x() const { return ((__m128)data)[0]; }
		float32 y() const { return ((__m128)data)[1]; }
		float32 z() const { return ((__m128)data)[2]; }
		float32 w() const { return ((__m128)data)[3]; }
		
		fvec operator- () const;
		
		fvec operator+ ( const fvec& ) const;
		fvec operator- ( const fvec& ) const;
		fvec operator* ( const fvec& ) const;
		fvec operator/ ( const fvec& ) const;
		
		fvec& operator+= ( const fvec& );
		fvec& operator+= ( float32 );
		fvec& operator-= ( const fvec& );
		fvec& operator-= ( float32 );
		fvec& operator*= ( const fvec& );
		fvec& operator*= ( float32 );
		fvec& operator/= ( const fvec& );
		fvec& operator/= ( float32 );
		
		float32 length() const;
		void normalize();
		
		static float32 distance( const fvec&, const fvec& );
		static fvec normalize( const fvec& );
		static float32 dot( const fvec&, const fvec& );
		static fvec cross( const fvec&, const fvec& );
		static fvec reflect( const fvec&, const fvec& );
		static fvec refract( const fvec&, const fvec&, float32 );
		
		static float32 angle( const fvec&, const fvec& );
	};
	
	fvec operator+ ( float32, const fvec& );
	fvec operator+ ( const fvec&, float32 );
	fvec operator- ( float32, const fvec& );
	fvec operator- ( const fvec&, float32 );
	fvec operator* ( float32, const fvec& );
	fvec operator* ( const fvec&, float32 );
	fvec operator/ ( float32, const fvec& );
	fvec operator/ ( const fvec&, float32 );
	
	std::ostream& operator<< ( std::ostream&, const fvec& );
	
}

#endif//__FLOAT_VECTOR_FAST_H__