#include <cmath>
#include <cstdint>
#include <iostream>
#include "core/fvector.h"

namespace core {
	
	fvec::fvec() {
		data = _mm_setzero_ps();
	}
		
	fvec::fvec( float32 a, float32 b, float32 c ) {
		data = _mm_set_ps(0,c,b,a);
	}
		
	fvec::fvec( float32 a, float32 b, float32 c, float32 d ) {
		data = _mm_set_ps(d,c,b,a);
	}

	fvec::fvec( const __m128& cpy ) {
		data = cpy;
	}
		
	fvec::fvec( const fvec& cpy ) {
		data = cpy.data;
	}
	
	fvec fvec::operator- () const {
		return _mm_xor_ps(data, _mm_set1_ps(-0.0));;
	}
	
	fvec fvec::operator+ ( const fvec& other ) const {
		return _mm_add_ps( data, other.data );
	}
	fvec fvec::operator- ( const fvec& other ) const {
		return _mm_sub_ps( data, other.data );
	}
	fvec fvec::operator* ( const fvec& other ) const {
		return _mm_mul_ps( data, other.data );
	}
	fvec fvec::operator/ ( const fvec& other ) const {
		return _mm_div_ps( data, other.data );
	}

	fvec& fvec::operator+= ( const fvec& other ) {
		data = _mm_add_ps( data, other.data );
		return *this;
	}
	fvec& fvec::operator+= ( float32 sum ) {
		__m128 tmp = _mm_set1_ps( sum );
		data = _mm_add_ps( data, tmp );
		return *this;
	}
	fvec& fvec::operator-= ( const fvec& other ) {
		data = _mm_sub_ps( data, other.data );
		return *this;
	}
	fvec& fvec::operator-= ( float32 sub ) {
		__m128 tmp = _mm_set1_ps( sub );
		data = _mm_sub_ps( data, tmp );
		return *this;
	}
	fvec& fvec::operator*= ( const fvec& other ) {
		data = _mm_mul_ps( data, other.data );
		return *this;
	}
	fvec& fvec::operator*= ( float32 mul ) {
		__m128 tmp = _mm_set1_ps( mul );
		data = _mm_mul_ps( data, tmp );
		return *this;
	}
	fvec& fvec::operator/= ( const fvec& other ) {
		data = _mm_div_ps( data, other.data );
		return *this;
	}
	fvec& fvec::operator/= ( float32 d ) {
		__m128 tmp = _mm_set1_ps( d );
		data = _mm_div_ps( data, tmp );
		return *this;
	}

	float32 fvec::length() const {
  		return _mm_sqrt_ps(_mm_dp_ps(data, data, 0xFF))[0];
	}
	void fvec::normalize() {
		__m128 inverse_norm = _mm_rsqrt_ps(_mm_dp_ps(data, data, 0xFF));
  		data = _mm_mul_ps(data, inverse_norm);
	}

	float32 fvec::distance( const fvec& a, const fvec& b ) {
		__m128 c = _mm_sub_ps( b.data, a.data );
		return _mm_sqrt_ps(_mm_dp_ps(c, c, 0xFF))[0];
	}
	fvec fvec::normalize( const fvec& vec ) {
		__m128 inverse_norm = _mm_rsqrt_ps(_mm_dp_ps(vec.data, vec.data, 0xFF));
  		return _mm_mul_ps(vec.data, inverse_norm);
	}
	float32 fvec::dot( const fvec& a, const fvec& b ) {
		return _mm_dp_ps(a.data, b.data, 0xFF)[0];
	}
	fvec fvec::cross( const fvec& a, const fvec& b ) {
		return _mm_sub_ps(
			_mm_mul_ps(
				_mm_shuffle_ps(a.data, a.data, _MM_SHUFFLE(3, 0, 2, 1)),
				_mm_shuffle_ps(b.data, b.data, _MM_SHUFFLE(3, 1, 0, 2))
			),
			_mm_mul_ps(
				_mm_shuffle_ps(a.data, a.data, _MM_SHUFFLE(3, 1, 0, 2)),
				_mm_shuffle_ps(b.data, b.data, _MM_SHUFFLE(3, 0, 2, 1))
			)
		  );
	}
	fvec fvec::reflect( const fvec& input, const fvec& normal ) {
		float32 dot = _mm_dp_ps(input.data, normal.data, 0xFF)[0];
		__m128 with = _mm_mul_ps( normal.data, _mm_set1_ps(dot) );
		__m128 against = _mm_sub_ps( input.data, with );
		return _mm_sub_ps( against, with );
	}
	fvec fvec::refract( const fvec& input, const fvec& normal, float32 dIOR ) {
		float32 dot = _mm_dp_ps(input.data, normal.data, 0xFF)[0];
		__m128 with = _mm_mul_ps( normal.data, _mm_set1_ps(dot) );
		__m128 against = _mm_sub_ps( input.data, with );
		return normalize( _mm_add_ps( _mm_mul_ps( against, _mm_set1_ps(dIOR) ), with ) );
	}
	float32 fvec::angle( const fvec& a, const fvec& b ) {
		return std::acos( ( fvec::dot(a,b) ) / ( a.length() * b.length() ) );
	}

	fvec operator+ ( float32 a, const fvec& b ) {
		return _mm_add_ps( _mm_set1_ps(a), b.data );
	}
	fvec operator+ ( const fvec& a, float32 b ) {
		return _mm_add_ps( a.data, _mm_set1_ps(b) );
	}
	fvec operator- ( float32 a, const fvec& b ) {
		return _mm_sub_ps( _mm_set1_ps(a), b.data );
	}
	fvec operator- ( const fvec& a, float32 b ) {
		return _mm_sub_ps( a.data, _mm_set1_ps(b) );
	}
	fvec operator* ( float32 a, const fvec& b ) {
		return _mm_mul_ps( _mm_set1_ps(a), b.data );
	}
	fvec operator* ( const fvec& a, float32 b ) {
		return _mm_mul_ps( a.data, _mm_set1_ps(b) );
	}
	fvec operator/ ( float32 a, const fvec& b ) {
		return _mm_div_ps( _mm_set1_ps(a), b.data );
	}
	fvec operator/ ( const fvec& a, float32 b ) {
		return _mm_div_ps( a.data, _mm_set1_ps(b) );
	}
	
	std::ostream& operator<< ( std::ostream& ostr, const fvec& v ) {
		ostr << "<" << v.x() << "," << v.y() << "," << v.z() << "," << v.w() << ">";
		return ostr;
	}
	
}