#ifndef __AXIS_ANGLES_H__
#define __AXIS_ANGLES_H__

#include "core/fvector.h"

namespace core {

	struct DEFINE_EXPORT axisangle_t {
		fvec axis;
		float32 angle;
		
		axisangle_t();
		axisangle_t(fvec,float32); // explicit dump
		axisangle_t(float32,fvec); // ""
		axisangle_t(fvec,fvec); // angle between two vectors
		
		fvec simplify();
	};

	DEFINE_EXPORTED_FUNCTION( std::ostream&, operator<<, ( std::ostream&, const axisangle_t& ) );

}

#endif//__AXIS_ANGLES_H__
