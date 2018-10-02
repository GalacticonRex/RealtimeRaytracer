#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include "core/fvector.h"

namespace core {

	struct DEFINE_EXPORT quaternion_t {
		float32 i,j,k,w;
		
		quaternion_t();
		quaternion_t(float32,float32,float32,float32);
		
		quaternion_t operator+ () const;
		quaternion_t operator- () const;
		
		const fvec& asVector() const;
		quaternion_t inverse() const;
		
		fvec operator* ( const fvec& ) const;
	};

	DEFINE_EXPORTED_FUNCTION( const quaternion_t&, asQuaternion, (const fvec&) );

	DEFINE_EXPORTED_FUNCTION( float32, Dot, (const quaternion_t&,const quaternion_t&) );
	
	// blend between two quaternions
	DEFINE_EXPORTED_FUNCTION( quaternion_t, Blend, (const quaternion_t&,const quaternion_t&,float32=.5) );
	
	// multiply two quaternions
	DEFINE_EXPORTED_FUNCTION( quaternion_t, Concatenate, (const quaternion_t&,const quaternion_t&) );
	
	// add two quaternions
	DEFINE_EXPORTED_FUNCTION( quaternion_t, Integrate, (const quaternion_t&,fvec,float32=1.) );
	DEFINE_EXPORTED_FUNCTION( quaternion_t, Integrate, (const quaternion_t&,const quaternion_t&,float32=1.) );
	
	DEFINE_EXPORTED_FUNCTION( quaternion_t, Product, (const quaternion_t&,const quaternion_t&) );

	DEFINE_EXPORTED_FUNCTION( std::ostream&, operator<<, ( std::ostream&, const quaternion_t& ) );

}

#endif//__QUATERNION_H__
