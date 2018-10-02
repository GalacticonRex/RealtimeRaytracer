#ifndef __CONVERT_H__
#define __CONVERT_H__

#include "core/matrix.h"
#include "core/axis_angle.h"
#include "core/euler_angles.h"
#include "core/quaternion.h"

namespace core {
	
	// color conversion
	DEFINE_EXPORTED_FUNCTION( vec3, toHSV, ( const vec3& ) );
	DEFINE_EXPORTED_FUNCTION( vec3, toRGB, ( const vec3& ) );

	DEFINE_EXPORTED_FUNCTION( vec3, toHSV, ( float32, float32, float32 ) );
	DEFINE_EXPORTED_FUNCTION( vec3, toRGB, ( float32, float32, float32 ) );

	// convert polar and cartesian coordinates
	DEFINE_EXPORTED_FUNCTION( vec2, toCartesian, ( float32 length, float32 angle ) );
	DEFINE_EXPORTED_FUNCTION( vec2, toCartesian, ( vec2 polar ) );

	DEFINE_EXPORTED_FUNCTION( vec2, toPolar, ( float32 x, float32 y ) );
	DEFINE_EXPORTED_FUNCTION( vec2, toPolar, ( vec2 carte ) );

	// convert 2D rotations
	DEFINE_EXPORTED_FUNCTION( float32, toFloat, (const mat2&) );
	DEFINE_EXPORTED_FUNCTION( mat2, toMatrix, (float32) );

	// convert 3D rotations to different representations
	DEFINE_EXPORTED_FUNCTION( mat3, toMatrix, (const eulerangles_t&) );
	DEFINE_EXPORTED_FUNCTION( mat3, toMatrix, (const axisangle_t&) );
	DEFINE_EXPORTED_FUNCTION( mat3, toMatrix, (const quaternion_t&) );

	DEFINE_EXPORTED_FUNCTION( axisangle_t, toAxisAngle, (const eulerangles_t&) );
	DEFINE_EXPORTED_FUNCTION( axisangle_t, toAxisAngle, (const mat3&) );
	DEFINE_EXPORTED_FUNCTION( axisangle_t, toAxisAngle, (const quaternion_t&) );

	DEFINE_EXPORTED_FUNCTION( eulerangles_t, toEuler, (const mat3&) );
	DEFINE_EXPORTED_FUNCTION( eulerangles_t, toEuler, (const axisangle_t&) );
	DEFINE_EXPORTED_FUNCTION( eulerangles_t, toEuler, (const quaternion_t&) );

	DEFINE_EXPORTED_FUNCTION( quaternion_t, toQuaternion, (const mat3&) );
	DEFINE_EXPORTED_FUNCTION( quaternion_t, toQuaternion, (const axisangle_t&) );
	DEFINE_EXPORTED_FUNCTION( quaternion_t, toQuaternion, (const eulerangles_t&) );

}

#endif//__CONVERT_H__
