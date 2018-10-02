#include "core/quaternion.h"
#include "core/methods.h"
#include "core/convert.h"
#include "core/axis_angle.h"

namespace core {

namespace {
	using namespace std;
}

quaternion_t::quaternion_t()
	: i(0),j(0),k(0),w(1) { ; }
quaternion_t::quaternion_t(float32 i_,float32 j_,float32 k_,float32 w_)
	: i(i_),j(j_),k(k_),w(w_) { ; }
	
quaternion_t quaternion_t::operator+ () const { return *this; }
quaternion_t quaternion_t::operator- () const { return quaternion_t(-i,-j,-k,-w); }

const fvec& quaternion_t::asVector() const { return *((fvec*)this); }
quaternion_t quaternion_t::inverse() const {
	return quaternion_t( -i, -j, -k, w );
}

fvec quaternion_t::operator* ( const fvec& v ) const {
	quaternion_t q = 
		Product( 
			Product( 
				*this, 
				asQuaternion( v )
			),
			inverse() 
		);
	return q.asVector();
}

const quaternion_t& asQuaternion(const fvec& v) {
	return *((const quaternion_t*)(&v));
}

float32 Dot( const quaternion_t& a, const quaternion_t& b ) { 
	return a.w*b.w + a.i*b.i + a.j*b.j + a.k*b.k;
}
quaternion_t Blend( const quaternion_t& a, const quaternion_t& b, float32 c ) {
	float32 d = Clamp(c),
			  qdot = Dot( a,b ),
			  c2 = 1. - d;
	
	if ( qdot < 0 ) {
		fvec v1 = a.asVector() * c2,
			 v2 = (-b).asVector() * c,
			 v3 = fvec::normalize( v1 + v2 );
		return asQuaternion(v3);
	} else {
		fvec v1 = a.asVector() * c2,
			 v2 = b.asVector() * c,
			 v3 = fvec::normalize( v1 + v2 );
		return asQuaternion(v3);
	}
}
quaternion_t Concatenate( const quaternion_t& a, const quaternion_t& b ) {
	fvec selfv( b.i,b.j,b.k ),
		 othev( a.i,a.j,a.k ),
		 fin = othev * b.w + selfv * a.w + fvec::cross( selfv,othev );
	return quaternion_t( fin.x(),fin.y(),fin.z(),b.w*a.w - fvec::dot( selfv,othev ) );
}
quaternion_t Integrate( const quaternion_t& a, fvec b, float32 c) {
	b /= 2.0f;
	float32 vmag = b.length(), w, pass;
	
	float32 vmag2 = vmag * vmag;
	float32 vmag4 = vmag2 * vmag2;
	if ( vmag4 / 24. < 0.0000001 ) {
		w = 1.f - vmag2 / 2.f;
		pass = 1.f - vmag2 / 6.f;
	} else {
		w = cos( vmag );
		pass = sin( vmag ) / vmag;
	}
	
	b *= pass;
	quaternion_t dq( b.x(),b.y(),b.z(),w );
	
	return Concatenate( dq,a );
}
quaternion_t Integrate( const quaternion_t& a, const quaternion_t& b, float32 c ) {
	return Integrate( a,toAxisAngle(b).simplify(),c );
}
quaternion_t Product( const quaternion_t& a, const quaternion_t& b ) {
	return quaternion_t(a.w*b.i + a.i*b.w + a.j*b.k - a.k*b.j,
					   	a.w*b.j - a.i*b.k + a.j*b.w + a.k*b.i,
					   	a.w*b.k + a.i*b.j - a.j*b.i + a.k*b.w,
					   	a.w*b.w - a.i*b.i - a.j*b.j - a.k*b.k);
}

ostream& operator<< ( ostream& out, const quaternion_t& q ) {
	out << "( " << q.w << " + " << q.i << "i + " << q.j << "j + " << q.k << "k )";
	return out;
}

}
